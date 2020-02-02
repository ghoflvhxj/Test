#include <string>
#include "Vector4.h"

using namespace Engine;

Vector4::Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
{
}

Vector4::Vector4(const float& x) : x(x), y(x), z(x), w(x)
{
}

Vector4::Vector4(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(const float(&v)[4])
{
	memcpy(this->v, v, sizeof(Vector4));
}

Vector4::Vector4(const simd& V)
{
	memcpy(this, &V, sizeof(Vector4));
}

const float Vector4::Length() const
{
	// simd 제곱(s^2)에서 x, y, z 추출
	simd splatedX = s * s;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));
	simd splatedZ = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(2, 2, 2, 2));
	simd splatedW = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(3, 3, 3, 3));

	// 추출한 x, y 를 더한 후 제곱근해 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY + splatedZ + splatedW);

	return sqrtf(result);
}

const float Vector4::Dot(const Vector4& v) const
{
	// simd 곱(s1*s2)에서 x, y, z, w 추출
	simd splatedX = s * v.s;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));
	simd splatedZ = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(2, 2, 2, 2));
	simd splatedW = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(3, 3, 3, 3));

	// 추출한 x, y, z, w 를 더해서 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY + splatedZ + splatedW);

	return result;
}

const Vector4 Vector4::Normalize() const
{
	float len = Length();
	return { x / len, y / len, z / len, w / len };
}

const Vector4 Vector4::operator+(const Vector4& ref) const
{
	return s + ref.s;
}

const Vector4 Vector4::operator-(const Vector4& ref) const
{
	return s - ref.s;
}

const Vector4 Vector4::operator*(const Vector4& ref) const
{
	return s * ref.s;
}

const Vector4 Vector4::operator/(const Vector4& ref) const
{
	return s / ref.s;
}

Vector4& Vector4::operator=(const Vector4& ref)
{
	memcpy(this, &ref, sizeof(Vector4));
	return *this;
}

Vector4& Vector4::operator+=(const Vector4& ref)
{
	*this = *this + ref;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& ref)
{
	*this = *this - ref;
	return *this;
}

Vector4& Vector4::operator*=(const Vector4& ref)
{
	*this = *this * ref;
	return *this;
}

Vector4& Vector4::operator/=(const Vector4& ref)
{
	*this = *this / ref;
	return *this;
}

bool Vector4::operator==(const Vector4& ref)
{
	__m128 equal = (s == ref.s);
	if (_mm_movemask_ps(equal) == 0xffff)
		return true;
	else
		return false;
}

bool Vector4::operator!=(const Vector4& ref)
{
	return not operator==(ref);
}


const Vector4 Vector4::operator+(const float& f) const
{
	simd s = simd_set_ps1(f);
	return this->s + s;
}

const Vector4 Vector4::operator-(const float& f) const
{
	simd s = simd_set_ps1(f);
	return this->s - s;
}

const Vector4 Vector4::operator*(const float& f) const
{
	simd s = simd_set_ps1(f);
	return this->s * s;
}

const Vector4 Vector4::operator/(const float& f) const
{
	simd s = simd_set_ps1(f);
	return this->s / s;
}
