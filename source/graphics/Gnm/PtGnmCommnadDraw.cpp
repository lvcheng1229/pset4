#include <string>
#include <unordered_map>
#include <filesystem>

#include "PtGnmTranslator.h"
#include "PtGPURegs.h"

#include "core\PtUtil.h"

#include "graphics\Gcn\GcnShaderDecoder.h"
#include "graphics\RHI\DynamicRHI.h"

static std::unordered_map<std::string, std::shared_ptr<CRHIVertexShader>> gRHIVertexShaders;
static std::unordered_map<std::string, std::shared_ptr<CRHIPixelShader>> gRHIPixelShaders;

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

void CPtGnmTranslator::ProcessGnmPrivateOpDrawIndex(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)pm4Hdr;

	std::shared_ptr<CRHIVertexShader> pVertexShader = CreateVertexShader();
	std::shared_ptr<CRHIPixelShader> pPixelShader = CreatePixelShader();
	

	

}