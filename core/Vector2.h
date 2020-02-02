#pragma once
#include "MathCommon.h"

namespace Engine
{
	class Vector2
	{
	public:
		//
		// 생성자
		//
		Vector2();
		Vector2(const float& x);
		Vector2(const float& x, const float& y);
		Vector2(const float(&v)[2]);
		Vector2(const simd& V);
		Vector2(const Vector2& ref) = default;

		~Vector2() = default;

		//
		// 메소드
		//
		const float Length() const;
		const float Dot(const Vector2& v) const;
		const Vector2 Cross() const;
		const Vector2 Normalize() const;

		//
		// 연산자 오버로딩
		//
		/* Vector op Vector */
		const Vector2 operator+(const Vector2 & ref) const;
		const Vector2 operator-(const Vector2 & ref) const;
		const Vector2 operator*(const Vector2 & ref) const;
		const Vector2 operator/(const Vector2 & ref) const;

		Vector2& operator=(const Vector2 & ref);
		Vector2& operator+=(const Vector2 & ref);
		Vector2& operator-=(const Vector2 & ref);
		Vector2& operator*=(const Vector2 & ref);
		Vector2& operator/=(const Vector2 & ref);

		bool operator==(const Vector2 & ref);
		bool operator!=(const Vector2 & ref);

		/* Vector op num */
		const Vector2 operator+(const float& f) const;
		const Vector2 operator-(const float& f) const;
		const Vector2 operator*(const float& f) const;
		const Vector2 operator/(const float& f) const;

	public:
		union
		{
			float v[2];
			struct {
				union { float x, r; };
				union { float y, g; };
			};
		};
	};
}


