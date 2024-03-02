#pragma once
#include <vector>
#include <stdint.h>
#include "PtGPURegs.h"


// todo: we assume the mip number of the processed texture is 1 for now!
class CGnmTextureDetiler
{
public:
	void DeTileTexture(const CTextureResourceDesc* texDesc, std::vector<uint8_t>& outData);
private:
	void LoadThin1dThin(const CTextureResourceDesc* texDesc, std::vector<uint8_t>& outData);
	void LoadLinear(const CTextureResourceDesc* texDesc, std::vector<uint8_t>& outData);
	
};