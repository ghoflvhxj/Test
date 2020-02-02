#pragma once

#include <stdint.h>
#include <string>

namespace Engine
{
	//
	// SIMD 정의
	//
	#if defined(__ANDROID__)
		#define NEON
	#else
		#define SSE
	#endif

	#if defined SSE
		// 헤더
		#include <xmmintrin.h>
		#include <pmmintrin.h>

		typedef __m128 simd;
		//ss: 4개의 요소중 첫번째만 사용
		#define simd_store_ss(v, a)				_mm_store_ss(v, a)
		#define	simd_add_ss(a, b)				_mm_add_ss(a, b)
		//ps: 4개의 요소 전부 사용
		#define simd_load_ps(v)					_mm_load_ps(v)
		#define simd_store_ps(v, a)				_mm_store_ps(v, a)
		#define simd_set_ps(x, y, z, w)			_mm_set_ps(w, z, y, x)
		#define simd_set_ps1(x)					_mm_set_ps1(x) 

		#define simd_add_ps(a, b)				_mm_add_ps(a, b)
		#define simd_sub_ps(a, b)				_mm_sub_ps(a, b)
		#define simd_mul_ps(a, b)				_mm_mul_ps(a, b)
		#define simd_div_ps(a, b)				_mm_div_ps(a, b)
		#define simd_hadd_ps(a, b)				_mm_hadd_ps(a, b)

		#define simd_cmpeq_ps(a, b)				_mm_cmpeq_ps(a, b)
		#define simd_cmpneq_ps(a, b)			_mm_cmpneq_ps(a, b)
		#define simd_shuffle_ps(a, b, shuffle)	_mm_shuffle_ps(a, b, shuffle)
		//셔플용
		#define SHUFFLE_MASK(z, y, x, w) (z<<6)|(y<<4)|(x<<2)|(w)
	#else
		// 헤더 및 NEON 명령어
	#endif

	//
	// simd 연산자 오버로딩
	// 
	__forceinline const simd operator+(const simd& ref1, const simd& ref2)
	{
		return simd_add_ps(ref1, ref2);
	}
	__forceinline const simd operator-(const simd& ref1, const simd& ref2)
	{
		return simd_sub_ps(ref1, ref2);
	}
	__forceinline const simd operator*(const simd& ref1, const simd& ref2)
	{
		return simd_mul_ps(ref1, ref2);
	}
	__forceinline const simd operator/(const simd& ref1, const simd& ref2)
	{
		return simd_div_ps(ref1, ref2);
	}
	__forceinline const simd operator==(const simd& ref1, const simd& ref2)
	{
		return simd_cmpeq_ps(ref1, ref2);
	}
	__forceinline const simd operator!=(const simd& ref1, const simd& ref2)
	{
		return simd_cmpneq_ps(ref1, ref2);
	}

	//
	// 기타 상수
	//
	#define SMALL_NUMBER (1.e-8f)	//거의 0에 가까운 수. 0.00000001
	constexpr float PI = 3.141592654f;

	#define IDENTITY_R0 simd_set_ps(1.f, 0.f, 0.f, 0.f)
	#define IDENTITY_R1 simd_set_ps(0.f, 1.f, 0.f, 0.f)
	#define IDENTITY_R2 simd_set_ps(0.f, 0.f, 1.f, 0.f)
	#define IDENTITY_R3 simd_set_ps(0.f, 0.f, 0.f, 1.f)
}
