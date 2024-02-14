#pragma once
#include <stdint.h>
#include "vulkan\vulkan.hpp"

enum class EShaderBinaryType : uint8_t
{
	ePixelShader = 0,  ///< PS stage shader.
	eVertexShader = 1,  ///< VS stage shader
	eExportShader = 2,
	eLocalShader = 3,
	eComputeShader = 4,  ///< CS stage shader.
	eGeometryShader = 5,
	eUnknown = 6,
	eHullShader = 7,  ///< HS stage shader.
	eDomainShader = 8,  ///< DS stage shader with embedded CS stage frontend shader.
};

struct SShaderBinaryInfo
{
	uint8_t m_signature[7];  // 'OrbShdr'
	uint8_t m_version;       // ShaderBinaryInfoVersion

	unsigned int m_pssl_or_cg : 1;   // 1 = PSSL / Cg, 0 = IL / shtb
	unsigned int m_cached : 1;       // 1 = when compile, debugging source was cached.  May only make sense for PSSL=1
	uint32_t     m_type : 4;         // See enum ShaderBinaryType
	uint32_t     m_source_type : 2;  // See enum ShaderSourceType
	unsigned int m_length : 24;      // Binary code length (does not include this structure or any of its preceding associated tables)

	uint8_t m_chunkUsageBaseOffsetInDW;  // in DW, which starts at ((uint32_t*)&ShaderBinaryInfo) - m_chunkUsageBaseOffsetInDW; max is currently 7 dwords (128 T# + 32 V# + 20 CB V# + 16 UAV T#/V#)
	uint8_t m_numInputUsageSlots;        // Up to 16 user data reg slots + 128 extended user data dwords supported by CUE; up to 16 user data reg slots + 240 extended user data dwords supported by Gnm::InputUsageSlot
	uint8_t m_isSrt : 1;                 // 1 if this shader uses shader resource tables and has an SrtDef table embedded below the input usage table and any extended usage info
	uint8_t m_isSrtUsedInfoValid : 1;    // 1 if SrtDef::m_isUsed=0 indicates an element is definitely unused; 0 if SrtDef::m_isUsed=0 indicates only that the element is not known to be used (m_isUsed=1 always indicates a resource is known to be used)
	uint8_t m_isExtendedUsageInfo : 1;   // 1 if this shader has extended usage info for the InputUsage table embedded below the input usage table
	uint8_t m_reserved2 : 5;             // For future use
	uint8_t m_reserved3;                 // For future use

	uint32_t m_shaderHash0;  // Association hash first 4 bytes
	uint32_t m_shaderHash1;  // Association hash second 4 bytes
	uint32_t m_crc32;        // crc32 of shader + this struct, just up till this field
};

class SGcnBinaryInfo
{
public:
	SGcnBinaryInfo(const void* shaderCode);
	uint32_t GetCombinedHash();
	VkShaderStageFlagBits GetStage() const;
private:
	const SShaderBinaryInfo* m_sShaderBinInfo;
};

class CGcnHeader
{
public:
	CGcnHeader(const uint8_t* code);
	inline uint32_t GetLength()const { return m_sShaderBinInfo.m_length; }

private:
	SShaderBinaryInfo m_sShaderBinInfo = {};
};
