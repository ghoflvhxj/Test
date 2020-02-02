#include <string>
#include "Vector3.h"

using namespace Engine;

Vector3::Vector3() : x(0.f), y(0.f), z(0.f)
{
}

Vector3::Vector3(const float& x) : x(x), y(x), z(x)
{
}

Vector3::Vector3(const float& x, const float& y, const float& z) : x(x), y(y), z(z)
{
}

Vector3::Vector3(const float(&v)[3])
{
	memcpy(this->v, v, sizeof(Vector3));
}

Vector3::Vector3(const simd& V)
{
	memcpy(this, &V, sizeof(Vector3));
}


const float Vector3::Length() const
{
	// 벡터에 일치하는 simd 생성 
	simd s1 = simd_set_ps(x, y, z, 0.f);

	// simd 제곱(s1^2)에서 x, y, z 추출
	simd splatedX = s1 * s1;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));
	simd splatedZ = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(2, 2, 2, 2));

	// 추출한 x, y 를 더한 후 제곱근해 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY + splatedZ);

	return sqrtf(result);
}

const float Vector3::Dot(const Vector3& v) const
{
	// 벡터에 일치하는 simd 생성 
	simd s1 = simd_set_ps(x, y, z, 0.f);
	simd s2 = simd_set_ps(v.x, v.y, v.z, 0.f);

	// simd 곱(s1*s2)에서 x, y, z 추출
	simd splatedX = s1 * s2;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));
	simd splatedZ = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(2, 2, 2, 2));

	// 추출한 x, y, z 를 더해서 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY + splatedZ);

	return result;
}

const Vector3 Vector3::Cross(const Vector3& v) const
{
	// y * v.z, z * v.x, x * v.y
	simd s1 = simd_set_ps(y, z, x, 0.f);
	simd s2 = simd_set_ps(v.z, v.x, v.y, 0.f);
	simd temp = s1 * s2;

	// z * v.y, x * v.z, y * v.x
	s1 = simd_set_ps(z, x, y, 0.f);
	s2 = simd_set_ps(v.y, v.z, v.x, 0.f);

	return temp - (s1 * s2);
}

const Vector3 Vector3::Normalize() const
{
	float len = Length();
	return { x / len, y / len, z / len };
}

const Vector3 Vector3::operator+(const Vector3& ref) const
{
	simd a = simd_set_ps(x, y, z, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, ref.z, 0.f);

	return a / b;
}

const Vector3 Vector3::operator-(const Vector3& ref) const
{
	simd a = simd_set_ps(x, y, z, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, ref.z, 0.f);

	return a / b;
}

const Vector3 Vector3::operator*(const Vector3& ref) const
{
	simd a = simd_set_ps(x, y, z, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, ref.z, 0.f);

	return a * b;
}

const Vector3 Vector3::operator/(const Vector3& ref) const
{
	simd a = simd_set_ps(x, y, z, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, ref.z, 0.f);

	return a / b;
}

Vector3& Vector3::operator=(const Vector3& ref)
{
	memcpy(this, &ref, sizeof(Vector3));
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& ref)
{
	*this = *this + ref;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& ref)
{
	*this = *this - ref;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& ref)
{
	*this = *this * ref;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& ref)
{
	*this = *this / ref;
	return *this;
}

bool Vector3::operator==(const Vector3& ref)
{
	simd a = simd_set_ps(x, y, z, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, ref.z, 0.f);

	__m128 equal = (a == b);
	if (_mm_movemask_ps(equal) == 0xffff)
		return true;
	else
		return false;
}

bool Vector3::operator!=(const Vector3& ref)
{
	return not operator==(ref);
}

const Vector3 Vector3::operator+(const float& f) const
{
	simd b = simd_set_ps1(x, y, z, 0.f);
	simd a = simd_set_ps1(f);
	return a + b;
}

const Vector3 Vector3::operator-(const float& f) const
{
	simd b = simd_set_ps1(x, y, z, 0.f);
	simd a = simd_set_ps1(f);
	return a - b;
}

const Vector3 Vector3::operator*(const float& f) const
{
	simd b = simd_set_ps1(x, y, z, 0.f);
	simd a = simd_set_ps1(f);
	return a * b;
}

const Vector3 Vector3::operator/(const float& f) const
{
	simd b = simd_set_ps1(x, y, z, 0.f);
	simd a = simd_set_ps1(f);
	return a / b;
}