#pragma once

#define DEFINE_PSET_ENUM_FLAG_OPERATORS(ENUMTYPE) \
	inline           ENUMTYPE& operator|=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator&=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator^=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator| (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator& (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator^ (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr bool  operator! (ENUMTYPE  E)             { return !(__underlying_type(ENUMTYPE))E; } \
	inline constexpr ENUMTYPE  operator~ (ENUMTYPE  E)             { return (ENUMTYPE)~(__underlying_type(ENUMTYPE))E; }

#define PS4API __attribute__((sysv_abi))


template <typename T1, typename T2>
inline T1 AlignUp(T1 size, T2 align)
{
	return (size / static_cast<T1>(align) + ((size % static_cast<T1>(align)) ? 1 : 0)) * static_cast<T1>(align);
}