#pragma once
#include <stdint.h>
#include <xhash>
#include <string>

struct Vec2i
{
	int x;
	int y;
};

struct Vec2ui
{
	uint32_t x;
	uint32_t y;
};

template <class T>
inline void THashCombine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T1, typename T2>
inline T1 AlignUp(T1 size, T2 align)
{
	return (size / static_cast<T1>(align) + ((size % static_cast<T1>(align)) ? 1 : 0)) * static_cast<T1>(align);
}
void PtReadFile(const std::string& filePath, std::vector<uint8_t>& outData);
