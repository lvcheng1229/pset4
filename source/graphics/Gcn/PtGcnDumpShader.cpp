#include <filesystem>
#include <fstream>
#include <assert.h>

#include "graphics\Gnm\GnmStructure.h"
#include "graphics\Gnm\PtGPURegs.h"

#include "PtGcnDumpShader.h"

#include "core\PtUtil.h"

#include "graphics\Gcn\GcnShaderDecoder.h"

struct SSaveWord
{
	uint16_t REG;
	uint16_t COUNT;//count of uint32
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

SUSER_DATA_USEAGE GetUsage(void* data, uint32_t* USER_DATA)
{
	SUSER_DATA_USEAGE dataUsage;
	memset(&dataUsage, 0, sizeof(SUSER_DATA_USEAGE));

	CGsISAProcessor isaProcessor;
	isaProcessor.Init(data);

	SInputUsageSlot* slots = isaProcessor.GetShaderSlot();
	if (slots != nullptr)
	{
		for (uint32_t index = 0; index < GetShaderInfo(data)->m_numInputUsageSlots; index++)
		{
			switch (slots[index].m_usageType)
			{
			case kShaderInputUsageSubPtrFetchShader:
			{
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

void WriteUserData(std::ofstream& file, void* data, uint16_t REG, uint32_t* USER_DATA, CGsISAProcessor& isaProcessor)
{
	SUSER_DATA_USEAGE dataUsage = GetUsage(data, USER_DATA);
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
				uint32_t size = isaProcessor.ParseSize(buffer,true);
				WriteBlock(file, REG + index, buffer, size);
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

void SaveGcnVS()
{
	return;
	void* base = GetCodeAddress(GetGpuRegs()->SPI.VS.LO, GetGpuRegs()->SPI.VS.HI);
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
		std::string fileDir = std::string(PSET_ROOT_DIR) + "/save";

		if (!std::filesystem::exists(fileDir))
		{
			std::filesystem::create_directory(fileDir);
		}

		std::ofstream file(filePath, std::ios::binary);

		CGsISAProcessor isaProcessor;
		isaProcessor.Init(base);
		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_LO_VS, base, isaProcessor.ParseSize(base));

		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC1_VS, &GetGpuRegs()->SPI.VS.RSRC1, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC2_VS, &GetGpuRegs()->SPI.VS.RSRC2, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_PGM_RSRC3_VS, &GetGpuRegs()->SPI.VS.RSRC3, sizeof(uint32_t));
		
		WriteBlock(file, PtGfx::mmSPI_VS_OUT_CONFIG, &GetGpuRegs()->SPI.VS.OUT_CONFIG, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmSPI_SHADER_POS_FORMAT, &GetGpuRegs()->SPI.VS.POS_FORMAT, sizeof(uint32_t));
		WriteBlock(file, PtGfx::mmPA_CL_VS_OUT_CNTL, &GetGpuRegs()->SPI.VS.OUT_CNTL, sizeof(uint32_t));

		uint32_t USER_DATA[16];
		memset(USER_DATA, 0, sizeof(uint32_t) * 16);
		USER_DATA[0] = 1073741924;
		USER_DATA[1] = 9;
		USER_DATA[2] = 3238057276;
		USER_DATA[3] = 8;
		//WriteUserData(file, base, PtGfx::mmSPI_SHADER_USER_DATA_VS_0, GetGpuRegs()->SPI.VS.USER_DATA);
		WriteUserData(file, base, PtGfx::mmSPI_SHADER_USER_DATA_VS_0, USER_DATA, isaProcessor);

		WriteBlock(file, PtGfx::mmVGT_NUM_INSTANCES, &GetGpuRegs()->VGT_NUM_INSTANCES, sizeof(uint32_t));
		file.close();
	}
}

void SaveGcnPS()
{
}
