#pragma once
#include <stdint.h>
#include <xhash>
#include <string>

template<typename T, int N, typename R>
struct TVecN
{
	TVecN()
	{
		for (uint32_t index = 0; index < N; index++)
		{
			(((T*)this)[index]) = 0;
		}
	}

	inline T operator[](uint32_t index) const
	{
		return (((T*)this)[index]);
	}

	inline R operator-() const
	{
		R r;
		for (int i = 0; i < N; ++i)
			((T*)&r)[i] = -((T*)this)[i];
		return r;
	}

	inline R operator*(T s) const
	{
		R r;
		for (int i = 0; i < N; ++i)
			((T*)&r)[i] = ((T*)this)[i] * s;
		return r;
	}

	template<typename T2, typename R2>
	inline T Dot(const TVecN<T2, N, R2>& o) const
	{
		T s = ((T*)this)[0] * o[0];
		for (int i = 1; i < N; ++i)
			s = s + (((T*)this)[i] * o[i]);
		return s;
	}

	template<typename T2, typename R2>
	inline void Set(const TVecN<T2, N, R2>& o)
	{
		for (int i = 0; i < N; ++i)
			((T*)this)[i] = static_cast<T>(((T2*)&o)[i]);
	}

#define VEC_OPERATOR(op) \
		inline R operator op(const TVecN<T,N,R>& o) const \
		{ \
			R r; \
			for (int i = 0; i < N; ++i) \
				((T*)(&r))[i] = ((T*)(this))[i] op ((T*)(&o))[i]; \
			return r; \
		} \

	VEC_OPERATOR(+);
	VEC_OPERATOR(-);
	VEC_OPERATOR(*);
	VEC_OPERATOR(/ );
};

template<class F>
struct TVec2 : TVecN<F, 2, TVec2<F>>
{
	F x; F y;
	template<typename T2>
	TVec2(const TVec2<T2>& in) { TVecN<F, 2, TVec2<F>>::Set(in); }
	TVec2() :TVecN<F, 2, TVec2<F>>() {}
	TVec2(F x, F y) : x(x), y(y) {}
};

template<class F>
struct TVec3 : TVecN<F, 3, TVec3<F>>
{
	F x; F y; F z;
	template<typename T2>
	TVec3(const TVec3<T2>& in) { TVecN<F, 3, TVec3<F>>::Set(in); }
	TVec3() :TVecN<F, 3, TVec3<F>>() {}
	TVec3(F x, F y, F z) : x(x), y(y), z(z) {}
};

template<class F>
struct TVec4 : TVecN<F, 4, TVec4<F>>
{
	F x; F y; F z; F w;
	template<typename T2>
	TVec4(const TVec4<T2>& in) { TVecN<F, 4, TVec4<F>>::Set(in); }
	TVec4() :TVecN<F, 4, TVec4<F>>() {}
	TVec4(F x, F y, F z, F w) : x(x), y(y), z(z), w(w) {}
};

using Vec2 = TVec2<float>;
using Vec2i = TVec2<int>;
using Vec2ui = TVec2<uint32_t>;

using Vec3 = TVec3<float>;
using Vec3i = TVec3<int>;

using Vec4 = TVec4<float>;
using Vec4i = TVec4<int>;


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
