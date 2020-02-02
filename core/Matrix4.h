#pragma once

#include "Vector4.h"

namespace Engine
{
	class Vector3;

	__declspec(align(16)) class Matrix4
	{
	public:
		//
		// 생성자
		//
		Matrix4();
		Matrix4(const float& f);
		Matrix4(const Vector4& v1);
		Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
		Matrix4(const Matrix4& ref) = default;

		//
		// 메소드
		//
		const Matrix4 Transpose() const;
		const Matrix4 Inverse() const;
		const Matrix4 TransformInverse() const;
		const Matrix4 TransformInverseNoScale() const;

		//
		//	연산자 오버로딩
		//
		const Matrix4 operator+(const Matrix4& ref) const;
		const Matrix4 operator-(const Matrix4& ref) const;
		const Matrix4 operator*(const Matrix4& ref) const;

		Matrix4& operator=(const Matrix4& ref);

	public:
		union
		{
			Vector4 v[4];
			float m[4][4];
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};

	};

	const Matrix4 Matrix4Identity();
	const Matrix4 Matrix4LookAtLH(const Vector3& EyePosition, const Vector3& FocusPosition, const Vector3& UpDirection);
	const Matrix4 Matrix4PerspectiveFovLH(const float FovAngleY, const float AspectRatio, const float NearZ, const float FarZ);
}
