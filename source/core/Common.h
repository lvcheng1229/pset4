#pragma once

#define PSET_SYSV_ABI __attribute__((sysv_abi))
#define PSET_NORETURN __attribute__((noreturn))
#define PSET_NAKED __attribute__((naked))
#define PSET_NOINLINE __attribute__((noinline))
#define PSET_DEPRECATED __attribute__((deprecated))
#define PSET_UNUSED __attribute__((unused))
#define PSET_ALIGN(n) __attribute__((__aligned__(n)))

#define PSET_OK 0
#define SCE_ERROR_ERROR_FLAG           0x80000000



#define DEFINE_PSET_ENUM_FLAG_OPERATORS(ENUMTYPE) \
	inline           ENUMTYPE& operator|=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator&=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator^=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator| (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator& (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator^ (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr bool  operator! (ENUMTYPE  E)             { return !(__underlying_type(ENUMTYPE))E; } \
	inline constexpr ENUMTYPE  operator~ (ENUMTYPE  E)             { return (ENUMTYPE)~(__underlying_type(ENUMTYPE))E; }




