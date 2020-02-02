#pragma once
#include "MathCommon.h"

namespace Engine
{
	__declspec(align(16)) class Vector4
	{
	public:
		//
		// 생성자
		//
		Vector4();
		Vector4(const simd& v);
		Vector4(const float& x);
		Vector4(const float& x, const float& y, const float& z, const float& w);
		Vector4(const float(&v)[4]);
		Vector4(const Vector4& ref) = default;

		~Vector4() = default;

		//
		// 메소드
		//
		const float Length() const;
		const float Dot(const Vector4& v) const;
		//Vector4 Vector4Cross(const Vector4* v1) const;
		const Vector4 Normalize() const;

		//
		// 연산자 오버로딩
		//
		/* Vector op Vector */
		const Vector4 operator+(const Vector4& ref) const;
		const Vector4 operator-(const Vector4& ref) const;
		const Vector4 operator*(const Vector4& ref) const;
		const Vector4 operator/(const Vector4& ref) const;

		Vector4& operator=(const Vector4& ref);
		Vector4& operator+=(const Vector4& ref);
		Vector4& operator-=(const Vector4& ref);
		Vector4& operator*=(const Vector4& ref);
		Vector4& operator/=(const Vector4& ref);

		bool operator==(const Vector4& ref);
		bool operator!=(const Vector4& ref);

		/* Vector op num */
		const Vector4 operator+(const float& f) const;
		const Vector4 operator-(const float& f) const;
		const Vector4 operator*(const float& f) const;
		const Vector4 operator/(const float& f) const;



	public:
		union
		{
			float v[4];
			struct {
				union { float x, r; };
				union { float y, g; };
				union { float z, b; };
				union { float w, a; };
			};
			simd s;
		};
	};
}

