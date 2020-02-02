#include "Matrix4.h"
#include "Vector3.h"

using namespace Engine;

Matrix4::Matrix4() : v{Vector4(), Vector4(), Vector4(), Vector4()}
{
}

Matrix4::Matrix4(const Vector4& v1) : v{v1, v1, v1, v1}
{
}

Matrix4::Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4) : v{v1, v2, v3, v4}
{
}

Matrix4::Matrix4(const float& f) : v{Vector4(f), Vector4(f), Vector4(f), Vector4(f)}
{
}

const Matrix4 Matrix4::Transpose() const
{
	// v0.x, v0.y, v1.x, v1.y 
	// v0.z, v0.w, v1.z, v1.w
	// v2.x, v2.y, v3.x, v3.y 
	// v2.z, v2.w, v3.z, v3.w 
	simd temp0 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(1, 0, 1, 0));
	simd temp1 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(3, 2, 3, 2));
	simd temp2 = simd_shuffle_ps(v[2].s, v[3].s, SHUFFLE_MASK(1, 0, 1, 0));
	simd temp3 = simd_shuffle_ps(v[2].s, v[3].s, SHUFFLE_MASK(3, 2, 3, 2));

	// 전치 행렬
	// v0.x, v1.x, v2.x, v3.x
	// v0.y, v1.y, v2.y, v3.y
	// v0.z, v1.z, v2.z, v3.z
	// v0.w, v1.w, v2.w, v3.w
	simd v0 = simd_shuffle_ps(temp0, temp2, SHUFFLE_MASK(2, 0, 2, 0));
	simd v1 = simd_shuffle_ps(temp0, temp2, SHUFFLE_MASK(3, 1, 3, 1));
	simd v2 = simd_shuffle_ps(temp1, temp3, SHUFFLE_MASK(2, 0, 2, 0));
	simd v3 = simd_shuffle_ps(temp1, temp3, SHUFFLE_MASK(3, 1, 3, 1));

	return { v0, v1, v2, v3 };
}

const Matrix4 Matrix4::Inverse() const
{
	return Matrix4();
}

const Matrix4 Matrix4::TransformInverse() const
{
	// v0.x, v0.y, v1.x, v1.y 
	// v0.z, v0.w, v1.z, v1.w
	simd temp0 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(1, 0, 1, 0));
	simd temp1 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(3, 2, 3, 2));

	// 4x4행렬에서 3x3소행렬의 Transpose를 구한다.
	// v0.x, v1.x, v2.x, v2.w(=0)
	// v0.y, v1.y, v2.y, v2.w(=0)
	// v0.z, v1.z, v2.z, v2.w(=0)
	simd r0 = simd_shuffle_ps(temp0, v[2].s, SHUFFLE_MASK(3, 0, 2, 0));
	simd r1 = simd_shuffle_ps(temp0, v[2].s, SHUFFLE_MASK(3, 1, 3, 1));
	simd r2 = simd_shuffle_ps(temp1, v[2].s, SHUFFLE_MASK(3, 2, 2, 0));

	// 마지막 행
	simd r3 = simd_set_ps(v[3].Dot(v[0]), v[3].Dot(v[1]), v[3].Dot(v[2]), 1.f);;

	// 제곱을 구해 모두 더하면, 각 요소들은 v0, v1, v2 길이의 제곱이 된다.
	// 그리고 4번째 요소가 0으로 나누는걸 방지하기 위해 0,0,0,1 을 더한다.
	simd sqr = (r0 * r0) + (r1 * r1) + (r2 * r2) + simd_set_ps(0.f, 0.f, 0.f, 1.f);
	simd sqrt = sqr * simd_set_ps(-1.f, -1.f, -1.f, 1.f);

	r0 = r0 / sqr;
	r1 = r1 / sqr;
	r2 = r2 / sqr;
	r3 = r3 / sqrt;

	return { r0, r1, r2, r3 };
}

const Matrix4 Matrix4::TransformInverseNoScale() const
{
	// v0.x, v0.y, v1.x, v1.y 
	// v0.z, v0.w, v1.z, v1.w
	simd temp0 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(1, 0, 1, 0));
	simd temp1 = simd_shuffle_ps(v[0].s, v[1].s, SHUFFLE_MASK(3, 2, 3, 2));

	// 4x4행렬에서 3x3소행렬의 Transpose를 구한다.
	// v0.x, v1.x, v2.x, v2.w(=0)
	// v0.y, v1.y, v2.y, v2.w(=0)
	// v0.z, v1.z, v2.z, v2.w(=0)
	simd r0 = simd_shuffle_ps(temp0, v[2].s, SHUFFLE_MASK(3, 0, 2, 0));
	simd r1 = simd_shuffle_ps(temp0, v[2].s, SHUFFLE_MASK(3, 1, 3, 1));
	simd r2 = simd_shuffle_ps(temp1, v[2].s, SHUFFLE_MASK(3, 2, 2, 0));

	// 4x4행렬에서 마지막 행
	simd r3 = simd_set_ps(-v[3].Dot(v[0]), -v[3].Dot(v[1]), -v[3].Dot(v[2]), 1.f);

	return { r0, r1, r2, r3 };
}

const Matrix4 Engine::Matrix4Identity()
{
	return Matrix4(IDENTITY_R0, IDENTITY_R1, IDENTITY_R2, IDENTITY_R3);
}

const Matrix4 Engine::Matrix4LookAtLH(const Vector3& EyePosition, const Vector3& FocusPosition, const Vector3& UpDirection)
{
	//Vector3 eye, target, up;
	Vector3 w, u, v;

	w = (FocusPosition - EyePosition).Normalize();
	u = (UpDirection.Cross(w)).Normalize();
	v = w.Cross(u).Normalize();

	Vector4 r0{ u.x, u.y, u.z, 0.f };
	Vector4 r1{ v.x, v.y, v.z, 0.f };
	Vector4 r2{ w.x, w.y, w.z, 0.f };
	Vector4 r3{ EyePosition.x, EyePosition.y, EyePosition.z, 1.f };

	return Matrix4(r0, r1, r2, r3).TransformInverseNoScale();
}

const Matrix4 Engine::Matrix4PerspectiveFovLH(const float FovAngleY, const float AspectRatio, const float NearZ, const float FarZ)
{
	Vector4 r0(1.f / (AspectRatio * tanf(FovAngleY / 2.f)), 0.f, 0.f, 0.f);
	Vector4 r1(0.f, 1.f / tanf(FovAngleY / 2.f), 0.f, 0.f);
	Vector4 r2(0.f, 0.f, FarZ / (FarZ - NearZ), 1.f);
	Vector4 r3(0.f, 0.f, -(FarZ * NearZ) / (FarZ - NearZ), 0.f);

	return { r0, r1, r2, r3 };
}

const Matrix4 Matrix4::operator+(const Matrix4& ref) const
{
	return { v[0] + ref.v[0], v[1] + ref.v[1], v[2] + ref.v[2], v[3] + ref.v[3] };
}

const Matrix4 Matrix4::operator-(const Matrix4& ref) const
{
	return { v[0] - ref.v[0], v[1] - ref.v[1], v[2] - ref.v[2], v[3] - ref.v[3] };
}

const Matrix4 Matrix4::operator*(const Matrix4& ref) const
{
	Matrix4 mat;
	Matrix4 tp = ref.Transpose();
	//벡터 곱셈한 후, 각 요소를 합해야한다.
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			Vector4 temp = v[i] * tp.v[j];
			// s = {s1+s2, s1+s2, s3+s2, s1+s0}
			temp.s = simd_hadd_ps(temp.s, temp.s);
			// s = {s2+s3+s1+s0, s3+s2+s1+s0, s3+s2+s1+s0, s3+s2+s1+s0}
			temp.s = simd_hadd_ps(temp.s, temp.s);
			simd_store_ss(&mat.m[i][j], temp.s);
		}
	}

	return mat;
}

Matrix4& Matrix4::operator=(const Matrix4& ref)
{
	v[0] = ref.v[0];
	v[1] = ref.v[1];
	v[2] = ref.v[2];
	v[3] = ref.v[3];

	return *this;
}