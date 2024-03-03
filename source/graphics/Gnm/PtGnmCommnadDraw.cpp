#include <string>
#include <unordered_map>
#include <filesystem>
#include <assert.h>
#include <array>
#include <vector>

#include "PtGnmDetileTexture.h"
#include "PtGnmDefs.h"
#include "PtGnmTranslator.h"
#include "PtGPURegs.h"


#include "graphics\Spirv\SpirvParser.h"

#include "core\PtUtil.h"

#include "graphics\Gcn\PtGcnDumpShader.h"
#include "graphics\Gcn\GcnShaderDecoder.h"
#include "graphics\Gcn\GcnDefs.h"

#include "graphics\RHI\RHI.h"
#include "graphics\RHI\DynamicRHI.h"
#include "graphics\RHI\RHICommandList.h"

static std::unordered_map<std::string, std::shared_ptr<CRHIVertexShader>> gRHIVertexShaders;
static std::unordered_map<std::string, std::shared_ptr<CRHIPixelShader>> gRHIPixelShaders;
static std::unordered_map<void*, std::shared_ptr<CRHIBuffer>> gRHIBuffers;
static std::unordered_map<void*, std::shared_ptr<CRHITexture2D>> gRHITextures;

static std::shared_ptr<CRHIVertexShader> CreateVertexShader()
{
	std::shared_ptr<CRHIVertexShader> pVertexShader;

	void* vsCodeAddr = GetCodeAddress(GetGpuRegs()->SPI.VS.LO, GetGpuRegs()->SPI.VS.HI);
	std::string vsShaderName = std::to_string(GetShaderInfo(vsCodeAddr)->m_crc32);
	auto iter = gRHIVertexShaders.find(vsShaderName);
	if (iter != gRHIVertexShaders.end())
	{
		pVertexShader = iter->second;
	}
	else
	{
		SaveGcnVS();
		std::string fileName = std::string(PSET_ROOT_DIR) + "/save/o_" + vsShaderName + ".spv";
		if (std::filesystem::exists(fileName))
		{
			std::vector<uint8_t> vsShaderCode;
			PtReadFile(fileName, vsShaderCode);
			pVertexShader = RHICreateVertexShader(vsShaderCode);
			CGsISAProcessor isaProcessor;
			isaProcessor.Init(vsCodeAddr);
			pVertexShader->m_numCbv = isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmConstBuffer);
			pVertexShader->m_cbvStartRegIdnex = isaProcessor.GetUsageRegisterIndex(kShaderInputUsageImmConstBuffer);
			assert(isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmResource) == 0);
			assert(isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmSampler) == 0);
			assert(isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmRwResource) == 0);
			gRHIVertexShaders[vsShaderName] = pVertexShader;
		}
	}
	return pVertexShader;
}

static std::shared_ptr<CRHIPixelShader> CreatePixelShader()
{
	std::shared_ptr<CRHIPixelShader> pPixelShader;
	
	void* psCodeAddr = GetCodeAddress(GetGpuRegs()->SPI.PS.LO, GetGpuRegs()->SPI.PS.HI);
	std::string psShaderName = std::to_string(GetShaderInfo(psCodeAddr)->m_crc32);
	auto iter = gRHIPixelShaders.find(psShaderName);
	if (iter != gRHIPixelShaders.end())
	{
		pPixelShader = iter->second;
	}
	else
	{
		
		std::string fileName = std::string(PSET_ROOT_DIR) + "/save/o_" + psShaderName + ".spv";
		if (std::filesystem::exists(fileName))
		{
			std::vector<uint8_t> psShaderCode;
			PtReadFile(fileName, psShaderCode);
			
			CPtSpirvParser spvParser;
			spvParser.SetSpvSource(psShaderCode.data(), psShaderCode.size());
			spvParser.ModifyFragmentShaderSpirvSet();

			pPixelShader = RHICreatePixelShader(psShaderCode);
			
			CGsISAProcessor isaProcessor;
			isaProcessor.Init(psCodeAddr);
			pPixelShader->m_numPushConst = isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmConstBuffer);
			pPixelShader->m_pushCtStartRegIdnex = isaProcessor.GetUsageRegisterIndex(kShaderInputUsageImmConstBuffer);

			pPixelShader->m_numSrv = isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmResource);
			pPixelShader->m_srvStartIndex = isaProcessor.GetUsageRegisterIndex(kShaderInputUsageImmResource);

			pPixelShader->m_numSampler = isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmSampler);
			pPixelShader->m_samplerStartIndex = isaProcessor.GetUsageRegisterIndex(kShaderInputUsageImmSampler);
			
			if (pPixelShader->m_numPushConst > 0)
			{
				int32_t immPushConstRegIndex = isaProcessor.GetUsageRegisterIndex(EShaderInputUsage::kShaderInputUsageImmConstBuffer);
				for (uint32_t index = 0; index < pPixelShader->m_numPushConst; index++)
				{
					const uint32_t* pushConstData = (&GetGpuRegs()->SPI.PS.USER_DATA[immPushConstRegIndex + index * ShaderConstantDwordSize::kDwordSizeConstantBuffer]);
					const CBufferResourceDesc* pushConstantDesc = reinterpret_cast<const CBufferResourceDesc*>(pushConstData);

					uint32_t pushConstSize = pushConstantDesc->GetSize();
					pPixelShader->m_pushConstSizes.push_back(pushConstSize);
				}
			}
			assert(isaProcessor.GetUsageRegisterNum(kShaderInputUsageImmRwResource) == 0);
			gRHIPixelShaders[psShaderName] = pPixelShader;
		}
	}
	return pPixelShader;
}

static bool IsSingleVertexBinding(const uint32_t* vtxTable, const uint32_t semanticCount)
{
	struct VertexElement
	{
		void* data;
		uint32_t stride;
	};

	std::array<VertexElement, kMaxVertexBufferCount> vtxData;
	for (uint32_t i = 0; i != semanticCount; ++i)
	{
		uint32_t offsetInDwords = i * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
		const CBufferResourceDesc* vtxBuffer = reinterpret_cast<const CBufferResourceDesc*>(vtxTable + offsetInDwords);
		vtxData[i].data = vtxBuffer->GetBaseAddress();
		vtxData[i].stride = vtxBuffer->GetStride();
	}

	void* firstVertextStart = vtxData[0].data;
	void* firstVertextEnd = reinterpret_cast<uint8_t*>(firstVertextStart) + vtxData[0].stride;
	bool isSingleBinding = true;
	for (uint32_t i = 1; i != semanticCount; ++i)
	{
		void* vertex = vtxData[i].data;
		isSingleBinding &= (vertex > firstVertextStart && vertex < firstVertextEnd);
	}
	return isSingleBinding;
}

void CPtGnmTranslator::ProcessGnmPrivateOpDrawIndex(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)pm4Hdr;

	// TODO: refactor this cope
	{
		void* bufferAddres = (void*)(param->indexAddr);
		auto iter = gRHIBuffers.find(bufferAddres);
		if (iter == gRHIBuffers.end())
		{
			uint32_t idxSize = GetGpuRegs()->VGT_DMA.INDEX_TYPE == PtGfx::VGT_INDEX_16 ? sizeof(uint16_t) : sizeof(uint32_t);
			gRHIBuffers[bufferAddres] = RHICreateBuffer(bufferAddres, param->indexCount, idxSize, RHIBU_IB);
		}
	}

	// vertex/pixel shader
	std::shared_ptr<CRHIVertexShader> pVertexShader = CreateVertexShader();
	std::shared_ptr<CRHIPixelShader> pPixelShader = CreatePixelShader();

	// vertex buffer
	std::vector<SVertexBinding> vertexBindings;
	std::vector<SVertexElement> vertexEllemts;
	CGsISAProcessor vsIsaProcessor;
	vsIsaProcessor.Init(GetCodeAddress(GetGpuRegs()->SPI.VS.LO, GetGpuRegs()->SPI.VS.HI));
	int32_t fetchRegIndex = vsIsaProcessor.GetUsageRegisterIndex(EShaderInputUsage::kShaderInputUsageSubPtrFetchShader);
	assert(fetchRegIndex != -1);

	CGsISAProcessor fetchShaderProcessor;
	const PtPointer fsCode = *reinterpret_cast<PtPointer const*>(&GetGpuRegs()->SPI.VS.USER_DATA[fetchRegIndex]);
	uint32_t semanticNum = fetchShaderProcessor.ParserFetchShader(fsCode);

	size_t   firstAttributeOffset = 0;
	int32_t vbRegIndex = vsIsaProcessor.GetUsageRegisterIndex(EShaderInputUsage::kShaderInputUsagePtrVertexBufferTable);

	{
		const uint32_t* vertexTable = *reinterpret_cast<uint32_t* const*>(&GetGpuRegs()->SPI.VS.USER_DATA[vbRegIndex]);
		bool singleBinding = IsSingleVertexBinding(vertexTable, semanticNum);
		assert(singleBinding == true);

		SVertexBinding veretxBinding;

		for (uint32_t index = 0; index < semanticNum; index++)
		{
			uint32_t offsetInDwords = index * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
			const CBufferResourceDesc* vtxBuffer = reinterpret_cast<const CBufferResourceDesc*>(vertexTable + offsetInDwords);

			if (firstAttributeOffset == 0) { firstAttributeOffset = reinterpret_cast<size_t>(vtxBuffer->GetBaseAddress()); }

			SVertexElement vtxElement;
			vtxElement.m_vertexNumFormat = PtGfx::BUF_NUM_FORMAT(vtxBuffer->m_bufferSrd.word3.bitfields.NUM_FORMAT);
			vtxElement.m_vertexDataFormat = PtGfx::BUF_DATA_FORMAT(vtxBuffer->m_bufferSrd.word3.bitfields.DATA_FORMAT);

			vtxElement.binding = singleBinding ? 0 : index;
			vtxElement.location = index;
			vtxElement.offset = singleBinding ? reinterpret_cast<size_t>(vtxBuffer->GetBaseAddress()) - firstAttributeOffset : 0;

			vertexEllemts.push_back(vtxElement);

			if (index == 0) { veretxBinding.m_stride = vtxBuffer->GetStride(); }
		}
		
		vertexBindings.push_back(veretxBinding);

		uint32_t boundingCount = singleBinding ? 1 : semanticNum;
		for (uint32_t index = 0; index < boundingCount; index++)
		{
			uint32_t offsetInDwords = index * ShaderConstantDwordSize::kDwordSizeVertexBuffer;
			const CBufferResourceDesc* vtxBuffer = reinterpret_cast<const CBufferResourceDesc*>(vertexTable + offsetInDwords);
			void* bufferAddres = vtxBuffer->GetBaseAddress();
			auto iter = gRHIBuffers.find(bufferAddres);
			if (iter == gRHIBuffers.end())
			{
				gRHIBuffers[bufferAddres] = RHICreateBuffer(bufferAddres, vtxBuffer->GetSize(), 1, RHIBU_VB);
			}

			gRHICommandList.RHISetVertexBuffer(gRHIBuffers[bufferAddres].get(), index, 0);
		}
	}
	
	// vertex shader constant buffer
	if (pVertexShader->m_numCbv > 0)
	{
		for (uint32_t index = 0; index < pVertexShader->m_numCbv; index++)
		{
			const uint32_t* cbvDesc = reinterpret_cast<uint32_t*>(&GetGpuRegs()->SPI.VS.USER_DATA[pVertexShader->m_cbvStartRegIdnex + index * ShaderConstantDwordSize::kDwordSizeConstantBuffer]);
			const CBufferResourceDesc* cbDesc = reinterpret_cast<const CBufferResourceDesc*>(cbvDesc);
			if (pVertexShader->m_pConstantBuffers[index])
			{
				RHIUpdateBuffer(pVertexShader->m_pConstantBuffers[index].get(), (uint8_t*)cbDesc->GetBaseAddress(), cbDesc->GetSize());
			}
			else
			{
				pVertexShader->m_pConstantBuffers[index] = RHICreateBuffer(cbDesc->GetBaseAddress(), cbDesc->GetSize(), 1, RHIBU_CB);
			}
			//set buffers
			gRHICommandList.RHISetConstantBuffer(pVertexShader->m_pConstantBuffers[index].get(), index);
		}
	}

	//pixel shader texture
	if (pPixelShader->m_numSrv > 0)
	{
		for (uint32_t index = 0; index < pPixelShader->m_numSrv; index++)
		{
			const uint32_t* srvDesc = reinterpret_cast<uint32_t*>(&GetGpuRegs()->SPI.PS.USER_DATA[pPixelShader->m_srvStartIndex + index * ShaderConstantDwordSize::kDwordSizeResource]);
			const CTextureResourceDesc* srDesc = reinterpret_cast<const CTextureResourceDesc*>(srvDesc);
			void* imgData = srDesc->GetBaseAddress();

			if (gRHITextures[imgData] == nullptr)
			{
				ETileMode tileMode = ETileMode(srDesc->m_imageSrd.word3.bitfields.TILING_INDEX);
				bool isTile = tileMode != kTileModeDisplay_LinearAligned && tileMode != kTileModeDisplay_LinearGeneral;
				assert(isTile);

				std::vector<uint8_t> detiledTexture;
				CGnmTextureDetiler texDetiler;
				texDetiler.DeTileTexture(srDesc, detiledTexture);

				gRHITextures[imgData] = RHICreateTexture2D(detiledTexture.data(), srDesc->GetTextureWidth(), srDesc->GetTextureHeight(), PtGfx::IMG_DATA_FORMAT(srDesc->m_imageSrd.word1.bitfields.DATA_FORMAT), PtGfx::IMG_NUM_FORMAT(srDesc->m_imageSrd.word1.bitfields.NUM_FORMAT), TexCreate_ShaderResource);
			}
			gRHICommandList.RHISetTexture2D(gRHITextures[imgData].get(), index);
		}
	}
	// pixel shader push constant
	//TODO: which slot
	if (pPixelShader->m_numPushConst > 0)
	{
		uint32_t pushCtStartReg = pPixelShader->m_pushCtStartRegIdnex;
		for (uint32_t index = 0; index < pPixelShader->m_numPushConst; index++)
		{
			const uint32_t* pushConstData = (&GetGpuRegs()->SPI.PS.USER_DATA[pushCtStartReg + index * ShaderConstantDwordSize::kDwordSizeConstantBuffer]);
			const CBufferResourceDesc* pushConstantDesc = reinterpret_cast<const CBufferResourceDesc*>(pushConstData);
			gRHICommandList.RHIPixelShaderSetPushConstatnt(index, pushConstantDesc->GetSize(), (uint8_t*)pushConstantDesc->GetBaseAddress());
		}
	}

	// depth stencil state
	CRHIDepthStencilState depthStencilState;
	depthStencilState.bDepthTestEnable = GetGpuRegs()->DEPTH.DEPTH_CONTROL.bitfields.Z_ENABLE;
	depthStencilState.bDepthWriteEnable = GetGpuRegs()->DEPTH.DEPTH_CONTROL.bitfields.Z_WRITE_ENABLE;
	depthStencilState.zFunc = GetGpuRegs()->DEPTH.DEPTH_CONTROL.bitfields.ZFUNC;

	uint32_t rtNum = 0;
	for (uint32_t index = 0; index < 8; index++)
	{
		if (GetGpuRegs()->RENDER_TARGET[index].BASE.bitfields.BASE_256B != 0 &&
			GetGpuRegs()->RENDER_TARGET[index].INFO.bitfields.FORMAT != 0 &&
			((uint32_t(*(uint32_t*)(&GetGpuRegs()->TARGET_MASK)) >> (index * 4)) & 0xFF) != 0)
		{
			rtNum++;
		}
	}

	// gfx pipeline state
	CRHIGraphicsPipelineStateInitDesc graphicsPsoInitDesc;
	graphicsPsoInitDesc.m_vertexElements = vertexEllemts;
	graphicsPsoInitDesc.m_vertexBindings = vertexBindings;
	graphicsPsoInitDesc.m_pVertexShader = pVertexShader.get();
	graphicsPsoInitDesc.m_pPixelShader = pPixelShader.get();
	graphicsPsoInitDesc.m_dsState = depthStencilState;
	graphicsPsoInitDesc.m_pipelineColorBlendControl = GetGpuRegs()->CB_COLOR_CONTROL;

	for (uint32_t index = 0; index < Pal::MaxColorTargets; index++)
	{
		graphicsPsoInitDesc.m_colorBelndControls[index] = GetGpuRegs()->CB_BLEND_CONTROL[index];
		graphicsPsoInitDesc.renderTargets[index] = GetGpuRegs()->RENDER_TARGET[index];
	}

	graphicsPsoInitDesc.m_rtNum = rtNum;
	
	CRHIRenderPassInfo rhiRenderPassInfo;
	rhiRenderPassInfo.m_rtNum = rtNum;
	rhiRenderPassInfo.m_dsState = depthStencilState;
	for (uint32_t index = 0; index < Pal::MaxColorTargets; index++)
	{
		rhiRenderPassInfo.renderTargets[index] = GetGpuRegs()->RENDER_TARGET[index];
	}

	std::shared_ptr<CRHIRenderPass> renderPass = RHICreateRenderPass(rhiRenderPassInfo);

	// is context changed ? 
	if (m_setCtxCount != m_lastSetCtxCount)
	{
		m_lastSetCtxCount = m_setCtxCount;
		gRHICommandList.RHIEndRenderPass();
		gRHICommandList.RHIBeginRenderPass(renderPass.get(), RHIGetDeviceDefaultTexture(EDeviceDefaultTex::DDT_BackBuffer).get(), graphicsPsoInitDesc.m_rtNum, RHIGetDeviceDefaultTexture(EDeviceDefaultTex::DDT_DepthStencil).get());
	}
	graphicsPsoInitDesc.m_rhiRenderPass = renderPass.get();

	std::shared_ptr<CRHIGraphicsPipelineState> graphicsPipelineState = RHICreateGraphicsPipelineState(graphicsPsoInitDesc);
	gRHICommandList.RHISetGraphicsPipelineState(graphicsPipelineState);
	

	{
		void* idxBufferAddres = (void*)(param->indexAddr);
		auto idxIter = gRHIBuffers.find(idxBufferAddres);
		assert(idxIter != gRHIBuffers.end());
		gRHICommandList.RHIDrawIndexedPrimitive(idxIter->second.get(), param->indexCount, 1, 0, 0, 0);
	}
	
}