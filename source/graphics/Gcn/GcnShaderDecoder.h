#pragma once
#include <stdint.h>
#include <memory>

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

const SShaderBinaryInfo* GetShaderInfo(const void* code);

class CGsCodeReader
{
public:
	CGsCodeReader() :m_ptr(nullptr), m_end(nullptr) {}
	CGsCodeReader(const uint32_t* ptr, const uint32_t* end) :
		m_ptr(ptr), m_end(end), m_beg(ptr)
	{
	}

	inline uint32_t operator[](uint32_t index) { return m_ptr[index]; }
	inline uint32_t ReadU32() { return *(m_ptr++); };
	inline uint64_t ReadU64() { uint64_t value = *(uint64_t*)m_ptr; m_ptr += 2; return value; };
	inline bool IsEnd() { return m_ptr >= m_end; };
	inline uintptr_t GetPos() { return uintptr_t(m_ptr); };
	inline const uint32_t* GetPtr() { return m_ptr; };
	inline uintptr_t GetBeg() { return uintptr_t(m_beg); };
private:
	const uint32_t* m_beg = nullptr;
	const uint32_t* m_ptr = nullptr;
	const uint32_t* m_end = nullptr;
};


enum EShaderInputUsage
{
	kShaderInputUsageImmResource = 0x00, ///< Immediate read-only buffer/texture descriptor.
	kShaderInputUsageImmSampler = 0x01, ///< Immediate sampler descriptor.
	kShaderInputUsageImmConstBuffer = 0x02, ///< Immediate constant buffer descriptor.
	kShaderInputUsageImmVertexBuffer = 0x03, ///< Immediate vertex buffer descriptor.
	kShaderInputUsageImmRwResource = 0x04, ///< Immediate read/write buffer/texture descriptor.
	kShaderInputUsageImmAluFloatConst = 0x05, ///< Immediate float const (scalar or vector).
	kShaderInputUsageImmAluBool32Const = 0x06, ///< 32 immediate Booleans packed into one UINT.
	kShaderInputUsageImmGdsCounterRange = 0x07, ///< Immediate UINT with GDS address range for counters (used for append/consume buffers).
	kShaderInputUsageImmGdsMemoryRange = 0x08, ///< Immediate UINT with GDS address range for storage.
	kShaderInputUsageImmGwsBase = 0x09, ///< Immediate UINT with GWS resource base offset.
	kShaderInputUsageImmShaderResourceTable = 0x0A, ///< Pointer to read/write resource indirection table.
	kShaderInputUsageImmLdsEsGsSize = 0x0D, ///< Immediate LDS ESGS size used in on-chip GS
	// Skipped several items here...
	kShaderInputUsageSubPtrFetchShader = 0x12, ///< Immediate fetch shader subroutine pointer.
	kShaderInputUsagePtrResourceTable = 0x13, ///< Flat resource table pointer.
	kShaderInputUsagePtrInternalResourceTable = 0x14, ///< Flat internal resource table pointer.
	kShaderInputUsagePtrSamplerTable = 0x15, ///< Flat sampler table pointer.
	kShaderInputUsagePtrConstBufferTable = 0x16, ///< Flat const buffer table pointer.
	kShaderInputUsagePtrVertexBufferTable = 0x17, ///< Flat vertex buffer table pointer.
	kShaderInputUsagePtrSoBufferTable = 0x18, ///< Flat stream-out buffer table pointer.
	kShaderInputUsagePtrRwResourceTable = 0x19, ///< Flat read/write resource table pointer.
	kShaderInputUsagePtrInternalGlobalTable = 0x1A, ///< Internal driver table pointer.
	kShaderInputUsagePtrExtendedUserData = 0x1B, ///< Extended user data pointer.
	kShaderInputUsagePtrIndirectResourceTable = 0x1C, ///< Pointer to resource indirection table.
	kShaderInputUsagePtrIndirectInternalResourceTable = 0x1D, ///< Pointer to internal resource indirection table.
	kShaderInputUsagePtrIndirectRwResourceTable = 0x1E, ///< Pointer to read/write resource indirection table.
};


struct SUSER_DATA_USEAGE
{
	uint8_t data[16];
};

struct alignas(4) SInputUsageSlot
{
	uint8_t m_usageType; ///< From Gnm::ShaderInputUsageType.
	uint8_t m_apiSlot; ///< API slot or chunk ID.
	uint8_t m_startRegister; ///< User data slot.
	union
	{
		struct
		{
			uint8_t m_registerCount : 1;  ///< If 0, count is 4DW; if 1, count is 8DW. Other sizes are defined by the usage type.
			uint8_t m_resourceType : 1;   ///< If 0, resource type <c>V#</c>; if 1, resource type <c>T#</c>, in case of a Gnm::kShaderInputUsageImmResource.
			uint8_t m_reserved : 2;       ///< Unused; must be set to zero.
			uint8_t m_chunkMask : 4;      ///< Internal usage data.
		};
		uint8_t m_srtSizeInDWordMinusOne;
	};
};


class CGsISAProcessor
{
public:
	void Init(void* base);
	inline void Reset(void* base) { Init(base); };

	SInputUsageSlot* GetShaderSlot();
	uint32_t ParseSize(const void* start, bool bStePc = false);
	int32_t GetUsageRegisterIndex(EShaderInputUsage usgae);
	int32_t GetUsageRegisterNum(EShaderInputUsage usgae);
	uint32_t ParserFetchShader(const void* base);
private:
	uint32_t ParseInstruction(CGsCodeReader& codeReader);
private:

	const SShaderBinaryInfo* m_shaderInfo;
	const uint32_t* parseEnd;
	void* m_base;
	CGsCodeReader m_codeReader;
};


