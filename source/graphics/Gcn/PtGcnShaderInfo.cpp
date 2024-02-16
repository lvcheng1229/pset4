#include <assert.h>
#include "core\PtUtil.h"
#include "PtGcnShaderInfo.h"

#include <fstream>

SGcnBinaryInfo::SGcnBinaryInfo(const void* shaderCode)
{
	const uint32_t* token = reinterpret_cast<const uint32_t*>(shaderCode);
	const uint32_t  tokenMovVccHi = 0xBEEB03FF;//s_mov_b32 vcc_hi, #imm
	assert(token[0] == tokenMovVccHi);
	m_sShaderBinInfo = reinterpret_cast<const SShaderBinaryInfo*>(token + (token[1] + 1) * 2);


	uint32_t sizeTT = (reinterpret_cast<uintptr_t>(m_sShaderBinInfo) - reinterpret_cast<uintptr_t>(shaderCode)) + m_sShaderBinInfo->m_length;
	std::ofstream file("binary_file.bin", std::ios::out | std::ios::binary);
	file.write(reinterpret_cast<const char*>(shaderCode), sizeTT);
	file.close();
}

uint32_t SGcnBinaryInfo::GetCombinedHash()
{
	size_t binHash = 42;
	THashCombine(binHash, m_sShaderBinInfo->m_shaderHash0);
	THashCombine(binHash, m_sShaderBinInfo->m_crc32);
	THashCombine(binHash, GetStage());
	return binHash;
}

VkShaderStageFlagBits SGcnBinaryInfo::GetStage() const
{
	VkShaderStageFlagBits result = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	EShaderBinaryType binType = static_cast<EShaderBinaryType>(m_sShaderBinInfo->m_type);
	switch (binType)
	{
	case EShaderBinaryType::ePixelShader:
		result = VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	case EShaderBinaryType::eVertexShader:
		result = VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case EShaderBinaryType::eComputeShader:
		result = VK_SHADER_STAGE_COMPUTE_BIT;
		break;
	case EShaderBinaryType::eGeometryShader:
		result = VK_SHADER_STAGE_GEOMETRY_BIT;
		break;
	case EShaderBinaryType::eHullShader:
		result = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		break;
	case EShaderBinaryType::eDomainShader:
		result = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		break;
	case EShaderBinaryType::eLocalShader:
	case EShaderBinaryType::eExportShader:
	case EShaderBinaryType::eUnknown:
	default:
		assert(false);
		break;
	}
	return result;
}

CGcnHeader::CGcnHeader(const uint8_t* code)
{

}
