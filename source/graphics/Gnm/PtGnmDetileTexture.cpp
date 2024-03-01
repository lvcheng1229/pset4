#include <assert.h>
#include "PtGnmDefs.h"
#include "PtGnmDetileTexture.h"

const uint32_t kMicroTileWidth = 8;
const uint32_t kMicroTileHeight = 8;
const uint32_t kNumMicroTilePixels = kMicroTileWidth * kMicroTileHeight;

static uint32_t GetElementIndex(uint32_t x, uint32_t y, uint32_t z, uint32_t bitsPerElement, PtGfx::MicroTileMode microTileMode, PtGfx::ArrayMode arrayMode)
{
	uint32_t elem = 0;
	if (microTileMode == PtGfx::ADDR_SURF_THIN_MICRO_TILING || microTileMode == PtGfx::ADDR_SURF_DEPTH_MICRO_TILING)
	{
		elem |= ((x >> 0) & 0x1) << 0;
		elem |= ((y >> 0) & 0x1) << 1;
		elem |= ((x >> 1) & 0x1) << 2;
		elem |= ((y >> 1) & 0x1) << 3;
		elem |= ((x >> 2) & 0x1) << 4;
		elem |= ((y >> 2) & 0x1) << 5;

		switch (arrayMode)
		{
		case PtGfx::ARRAY_1D_TILED_THIN1:break;
		default:assert(false);
		}

		return elem;
	}
	else
	{
		assert(false); // unimplement
	}
	return 0;
};

struct STiler1D
{
	uint32_t m_minGpuMode;
	uint32_t m_tileMode;
	PtGfx::ArrayMode m_arrayMode;
	uint32_t m_linearWidth;
	uint32_t m_linearHeight;
	uint32_t m_linearDepth;
	uint32_t m_paddedWidth;
	uint32_t m_paddedHeight;
	uint32_t m_paddedDepth;
	uint32_t m_bitsPerElement;
	uint32_t m_linearSizeBytes;
	uint32_t m_tiledSizeBytes;

	PtGfx::MicroTileMode m_microTileMode;
	uint32_t m_tileThickness;
	uint32_t m_tileBytes;
	uint32_t m_tilesPerRow;
	uint32_t m_tilesPerSlice;

	inline uint32_t GetTiledElementBitOffset(uint32_t x, uint32_t y, uint32_t z)
	{
		//inverse morton code
		uint32_t elementIndex = GetElementIndex(x, y, z, m_bitsPerElement, m_microTileMode, m_arrayMode);
		uint32_t sliceOffset = (z / m_tileThickness) * m_tilesPerSlice * m_tileBytes;

		uint32_t tileRowIndex = y / kMicroTileHeight;
		uint32_t tileColIndex = y / kMicroTileWidth;
		uint32_t tileOffset = (tileRowIndex * m_tilesPerRow + tileColIndex) * m_tileBytes;

		uint32_t elementOffset = elementIndex * m_bitsPerElement;
		uint32_t final_offset = (sliceOffset + tileOffset) * 8 + elementOffset;

		return final_offset;
	}
};

static const STiler1D gTexture2D_default_32 =
{
	0,13,
	PtGfx::ArrayMode(2),
	8,8,1,8,8,1,32,256,256,
	PtGfx::MicroTileMode(1),
	1,256,1,1
};

void CGnmTextureDetiler::DeTileTexture(const CTextureResourceDesc* texDesc, std::vector<uint8_t>& outData)
{
	switch (texDesc->m_imageSrd.word3.bitfields.TILING_INDEX)
	{
	case kTileModeDepth_1dThin:
	{
		LoadThin1dThin(texDesc, outData);
		return;
	}
	default:
		assert(false);// unimplemented
	}
}

void CGnmTextureDetiler::LoadThin1dThin(const CTextureResourceDesc* texDesc, std::vector<uint8_t>& outData)
{
	assert(texDesc->m_imageSrd.word1.bitfields.DATA_FORMAT < PtGfx::IMG_DATA_FORMAT_BC1);//unimplemented
	uint8_t* srcData = (uint8_t*)texDesc->GetBaseAddress();

	STiler1D tiler = gTexture2D_default_32;

	tiler.m_linearDepth = 1;
	tiler.m_linearWidth = texDesc->m_imageSrd.word2.bitfields.WIDTH + 1u;
	tiler.m_linearHeight = texDesc->m_imageSrd.word2.bitfields.HEIGHT + 1u;
	tiler.m_bitsPerElement = GetImageDataFormatSizeInByte(PtGfx::IMG_DATA_FORMAT(texDesc->m_imageSrd.word1.bitfields.DATA_FORMAT)) * 8;
	
	PtGfx::GB_TILE_MODE0 tileMode = *(PtGfx::GB_TILE_MODE0*)(&gTileMode[texDesc->m_imageSrd.word3.bitfields.TILING_INDEX]);
	uint32_t microTileModeUint = tileMode.bitfields.MICRO_TILE_MODE_NEW;;
	PtGfx::MicroTileMode microTileMode = PtGfx::MicroTileMode(microTileModeUint);
	uint32_t arrayModeUint = tileMode.bitfields.ARRAY_MODE;
	PtGfx::ArrayMode arrayMode = PtGfx::ArrayMode(arrayModeUint);

	tiler.m_microTileMode = microTileMode;
	tiler.m_arrayMode = arrayMode;
	
	//assert(isTexFormat);
	tiler.m_paddedWidth = tiler.m_linearWidth;
	tiler.m_paddedHeight = tiler.m_linearHeight;

	tiler.m_tilesPerRow = tiler.m_paddedWidth / kMicroTileWidth;
	tiler.m_tilesPerSlice = tiler.m_tilesPerRow * (tiler.m_paddedHeight / kMicroTileHeight);

	tiler.m_tileBytes = (kMicroTileWidth * kMicroTileHeight * tiler.m_tileThickness * tiler.m_bitsPerElement + 7) / 8;
	
	uint32_t sliceSize = tiler.m_linearWidth * tiler.m_linearHeight;
	uint32_t bytesPerElement = tiler.m_bitsPerElement / 8;
	
	tiler.m_linearSizeBytes = tiler.m_linearWidth * tiler.m_linearHeight * tiler.m_linearDepth * bytesPerElement;
	outData.resize(tiler.m_linearSizeBytes);
	
	for (int32_t z = 0; z < tiler.m_linearDepth; z++)
	{
		for (int32_t y = 0; y < tiler.m_linearHeight; y++)
		{
			for (int32_t x = 0; x < tiler.m_linearWidth; x++)
			{
				uint32_t  bitOffset = tiler.GetTiledElementBitOffset(x, y, z);
				uint32_t byteIndex = bitOffset / 8;

				uint8_t src = srcData[byteIndex];
				uint32_t destByteIndex = (z * sliceSize + y * tiler.m_linearWidth + x) * bytesPerElement;
				outData[destByteIndex] = src;
			}
		}
	}
}
