#include <fstream>

#include "PtUtil.h"

void PtReadFile(const std::string& filePath, std::vector<uint8_t>& outData)
{
    std::ifstream fin(filePath, std::ios::binary | std::ios::in);
    fin.seekg(0, std::ios::end);
    size_t size = fin.tellg();
    outData.resize(size);
    fin.seekg(0, std::ios::beg);
    fin.read((char*)outData.data(), size);
    fin.close();
}
