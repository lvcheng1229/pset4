#include <filesystem>
#include <fstream>
#include <assert.h>

#include "PtGnmShader.h"
#include "PtGcnDumpShader.h"
#include "core\PtUtil.h"

#include "graphics\AMD\PtGPURegs.h"

enum EShaderInputUsage
{
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

struct SSaveWord
{
	uint16_t REG;
	uint16_t COUNT;//count of uint32
};

struct SUSER_DATA_USEAGE
{
	uint8_t data[16];
};

struct SInputUsageSlotSub
{
	uint8_t m_registerCount : 1;///< If 0, count is 4DW; if 1, count is 8DW. Other sizes are defined by the usage type.
	uint8_t m_resourceType : 1;	///< If 0, resource type <c>V#</c>; if 1, resource type <c>T#</c>, in case of a Gnm::kShaderInputUsageImmResource.
	uint8_t m_reserved : 2;		///< Unused; must be set to zero.
	uint8_t m_chunkMask : 4;	///< Internal usage data.
};

struct SInputUsageSlot
{
	uint8_t m_usageType; ///< From Gnm::ShaderInputUsageType.
	uint8_t m_apiSlot; ///< API slot or chunk ID.
	uint8_t m_startRegister; ///< User data slot.
	union
	{
		SInputUsageSlotSub slotSub;
		uint8_t m_srtSizeInDWordMinusOne;
	};
};

void WriteBlock(std::ofstream& file, uint16_t REG, void* data, uint64_t size)
{
	constexpr uint32_t MAX_SIZE = (0xFFFF + 1) * 4;
	if (size > MAX_SIZE)
	{
		return;
	}

	SSaveWord w;
	w.REG = REG;
	w.COUNT = (uint64_t(size + 3) / 4u) - 1;

	file.write((const char*)&w, sizeof(SSaveWord));
	file.write((const char*)data, AlignUp(size, 4));
}

SInputUsageSlot* GetShaderSlot(void* data)
{
	SInputUsageSlot* inputUsageSlot = nullptr;
	if (GetShaderInfo(data)->m_numInputUsageSlots > 0)
	{
		uint8_t* usageMasks = (uint8_t*)data - (GetShaderInfo(data)->m_chunkUsageBaseOffsetInDW * 4);
		inputUsageSlot = (SInputUsageSlot*)usageMasks - (GetShaderInfo(data)->m_numInputUsageSlots);
	}
	return inputUsageSlot;
}

SUSER_DATA_USEAGE GetUsage(void* data, uint32_t* USER_DATA)
{
	SUSER_DATA_USEAGE dataUsage;
	SInputUsageSlot* slots = GetShaderSlot(data);
	if (slots != nullptr)
	{
		for (uint32_t index = 0; index < GetShaderInfo(data)->m_numInputUsageSlots; index++)
		{
			switch (slots[index].m_usageType)
			{
			case kShaderInputUsageSubPtrFetchShader:
			{
				assert(false);
				uint8_t r = slots[index].m_startRegister;
				assert(r < 15);
				dataUsage.data[r] = 2;
				dataUsage.data[r + 1] = 1;
				break;
			}
			case kShaderInputUsagePtrResourceTable:
			case kShaderInputUsagePtrInternalResourceTable:
			case kShaderInputUsagePtrSamplerTable:
			case kShaderInputUsagePtrConstBufferTable:
			case kShaderInputUsagePtrVertexBufferTable:
			case kShaderInputUsagePtrSoBufferTable:
			case kShaderInputUsagePtrRwResourceTable:
			case kShaderInputUsagePtrInternalGlobalTable:
			case kShaderInputUsagePtrExtendedUserData:
			case kShaderInputUsagePtrIndirectResourceTable:
			case kShaderInputUsagePtrIndirectInternalResourceTable:
			case kShaderInputUsagePtrIndirectRwResourceTable:
			{
				uint8_t r = slots[index].m_startRegister;
				assert(r < 15);
				dataUsage.data[r] = 3;
				dataUsage.data[r + 1] = 1;
				break;
			}
			}
		}
	}

	for (uint32_t index = 0; index < 16; index++)
	{
		if (dataUsage.data[index] == 0 && USER_DATA[index] == 0)
		{
			dataUsage.data[index] = 1;
		}
	}
	return dataUsage;
}

void WriteUserData(std::ofstream& file, void* data, uint16_t REG, PtGfx::SPI_SHADER_USER_DATA_VS_0* USER_DATA)
{
	SUSER_DATA_USEAGE dataUsage = GetUsage(data, (uint32_t*)USER_DATA);
	for (uint16_t index = 0; index < 16; index++)
	{
		switch (dataUsage.data[index])
		{
		case 0:
		{
			WriteBlock(file, REG + index, &dataUsage.data[index], sizeof(uint32_t));
			break;
		}
		case 2:
		{
			void* buffer = GetFetchAddress(dataUsage.data[index], dataUsage.data[index + 1]);
			if (buffer != nullptr)
			{
				assert(false);
				WriteBlock(file, REG + index, buffer, 0);
			}
		}
		case 3:
		{
			void* buffer = GetFetchAddress(dataUsage.data[index], dataUsage.data[index + 1]);
			if (buffer != nullptr)
			{
				WriteBlock(file, REG + index, buffer, 256);
			}
		}
		}
	}
}

void SaveGcnVS(VsStageRegisters* vsRegs)
{
	void* base = vsRegs->getCodeAddress();
	if (base != nullptr)
	{
		size_t hashSeed = 42;
		THashCombine(hashSeed, GetShaderInfo(base)->m_length);
		THashCombine(hashSeed, GetShaderInfo(base)->m_shaderHash0);
		THashCombine(hashSeed, GetShaderInfo(base)->m_crc32);

		std::string filePath = std::string(PSET_ROOT_DIR) + "/save/" + std::to_string(hashSeed) + ".dump";

		if (std::filesystem::exists(filePath))
		{
			return;
		}

		std::ofstream file("data.bin", std::ios::binary);

		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_LO_VS, base, GetShaderInfo(base)->m_length);

		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC1_VS, &GetGpuRegs()->SPI.VS.RSRC1, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC2_VS, &GetGpuRegs()->SPI.VS.RSRC2, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC3_VS, &GetGpuRegs()->SPI.VS.RSRC3, sizeof(uint32_t));
		
		WriteBlock(file, PtGfx::mmSPI_VS_OUT_CONFIG, &GetGpuRegs()->SPI.VS.OUT_CONFIG, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_POS_FORMAT, &GetGpuRegs()->SPI.VS.POS_FORMAT, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmPA_CL_VS_OUT_CNTL, &GetGpuRegs()->SPI.VS.OUT_CNTL, sizeof(uint32_t));

		WriteUserData(file, base, PtGfx::mmSPI_SHADER_USER_DATA_VS_0, GetGpuRegs()->SPI.VS.USER_DATA);

		WriteBlock(file, PtGfx::mmVGT_NUM_INSTANCES, &GetGpuRegs()->VGT_NUM_INSTANCES, sizeof(uint32_t));
	}
}

void SaveGcnPS(PsStageRegisters* psRegs)
{
}
