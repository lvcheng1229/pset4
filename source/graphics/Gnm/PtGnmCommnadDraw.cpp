#include <string>
#include <unordered_map>
#include <filesystem>
#include <assert.h>
#include <array>
#include <vector>

#include "PtGnmTranslator.h"
#include "PtGPURegs.h"

#include "core\PtUtil.h"

#include "graphics\Gcn\GcnShaderDecoder.h"
#include "graphics\Gcn\GcnDefs.h"

#include "graphics\RHI\RHI.h"
#include "graphics\RHI\DynamicRHI.h"

static std::unordered_map<std::string, std::shared_ptr<CRHIVertexShader>> gRHIVertexShaders;
static std::unordered_map<std::string, std::shared_ptr<CRHIPixelShader>> gRHIPixelShaders;

void* GetSharpByPatch()
{

}

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
		std::string fileName = std::string(PSET_ROOT_DIR) + "/save/o_" + vsShaderName + ".spv";
		if (std::filesystem::exists(fileName))
		{
			std::vector<uint8_t> vsShaderCode;
			PtReadFile(fileName, vsShaderCode);
			pVertexShader = RHICreateVertexShader(vsShaderCode);
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
			pPixelShader = RHICreatePixelShader(psShaderCode);
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

	std::shared_ptr<CRHIVertexShader> pVertexShader = CreateVertexShader();
	std::shared_ptr<CRHIPixelShader> pPixelShader = CreatePixelShader();

	CGsISAProcessor isaProcessor;
	isaProcessor.Init(GetCodeAddress(GetGpuRegs()->SPI.VS.LO, GetGpuRegs()->SPI.VS.HI));
	int32_t fetchRegIndex = isaProcessor.GetUsageRegisterIndex(EShaderInputUsage::kShaderInputUsageSubPtrFetchShader);
	assert(fetchRegIndex != -1);
	
	CGsISAProcessor fetchShaderProcessor;
	const PtPointer fsCode = *reinterpret_cast<PtPointer const*>(&GetGpuRegs()->SPI.VS.USER_DATA[fetchRegIndex]);
	uint32_t semanticNum = fetchShaderProcessor.ParserFetchShader(fsCode);

	size_t   firstAttributeOffset = 0;
	int32_t vbRegIndex = isaProcessor.GetUsageRegisterIndex(EShaderInputUsage::kShaderInputUsagePtrVertexBufferTable);
	const uint32_t* vertexTable = *reinterpret_cast<uint32_t* const*>(&GetGpuRegs()->SPI.VS.USER_DATA[vbRegIndex]);
	bool singleBinding = IsSingleVertexBinding(vertexTable, semanticNum);

	std::vector<SVertexBinding>veretxBindings;
	std::vector<SVertexElement> vertexEllemts;
	
	if (singleBinding)
	{
		veretxBindings.push_back(SVertexBinding{ 0 });
	}
	
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

		if (singleBinding)
		{
			veretxBindings[0].m_stride += vtxBuffer->GetStride();
		}
		else
		{
			veretxBindings.push_back(SVertexBinding{ vtxBuffer->GetStride() });
		}
	}

	CRHIGraphicsPipelineStateInitDesc graphicsPsoInitDesc;
	graphicsPsoInitDesc.m_vertexElements = vertexEllemts;
	graphicsPsoInitDesc.m_vertexBindings = veretxBindings;
	graphicsPsoInitDesc.m_pVertexShader = pVertexShader.get();
	graphicsPsoInitDesc.m_pPixelShader = pPixelShader.get();

	RHICreateGraphicsPipelineState(graphicsPsoInitDesc);
	

}