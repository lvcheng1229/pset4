#pragma once
#include <stdint.h>
#include <xhash>

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