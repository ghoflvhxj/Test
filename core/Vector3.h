#pragma once
#include "MathCommon.h"

namespace Engine
{
	class Vector3
	{
	public:
		//
		// 생성자
		//
		Vector3();
		Vector3(const simd& v);
		Vector3(const float& x);
		Vector3(const float& x, const float& y, const float& z);
		Vector3(const float(&v)[3]);
		Vector3(const Vector3& ref) = default;

		~Vector3() = default;

		//
		// 메소드
		//
		const float Length() const;
		const float Dot(const Vector3& v) const;
		const Vector3 Cross(const Vector3& v) const;
		const Vector3 Normalize() const;

		//
		// 연산자 오버로딩
		//
		/* Vector op Vector */
		const Vector3 operator+(const Vector3& ref) const;
		const Vector3 operator-(const Vector3& ref) const;
		const Vector3 operator*(const Vector3& ref) const;
		const Vector3 operator/(const Vector3& ref) const;

		Vector3& operator=(const Vector3& ref);
		Vector3& operator+=(const Vector3& ref);
		Vector3& operator-=(const Vector3& ref);
		Vector3& operator*=(const Vector3& ref);
		Vector3& operator/=(const Vector3& ref);

		bool operator==(const Vector3& ref);
		bool operator!=(const Vector3& ref);

		/* Vector op num */
		const Vector3 operator+(const float& f) const;
		const Vector3 operator-(const float& f) const;
		const Vector3 operator*(const float& f) const;
		const Vector3 operator/(const float& f) const;


	public:
		union
		{
			float v[3];
			struct {
				union { float x, r; };
				union { float y, g; };
				union { float z, b; };
			};
		};
	};
}