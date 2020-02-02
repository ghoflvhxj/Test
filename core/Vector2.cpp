#include "Vector2.h"

using namespace Engine;

Vector2::Vector2() : x(0.f), y(0.f)
{
	int a, b, c;
}

Vector2::Vector2(const float& x) : x(x), y(x)
{
}

Vector2::Vector2(const float& x, const float& y) : x(x), y(y)
{
}

Vector2::Vector2(const float(&v)[2])
{
	memcpy(this->v, v, sizeof(Vector2));
}

Vector2::Vector2(const simd& V)
{
	memcpy(this, &V, sizeof(Vector2));
}

const float Vector2::Length() const
{
	// 벡터에 일치하는 simd 생성 
	simd s1 = simd_set_ps(x, y, 0.f, 0.f);

	// simd 제곱(s1^2)에서 x, y 추출
	simd splatedX = s1 * s1;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));

	// 추출한 x, y 를 더한 후 제곱근해 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY);

	return sqrtf(result);
}

const float Vector2::Dot(const Vector2& v) const
{
	// 벡터에 일치하는 simd 생성 
	simd s1 = simd_set_ps(x, y, 0.f, 0.f);
	simd s2 = simd_set_ps(v.x, v.y, 0.f, 0.f);

	// simd 곱(s1*s2)에서 x, y 추출
	simd splatedX = s1 * s2;
	simd splatedY = simd_shuffle_ps(splatedX, splatedX, SHUFFLE_MASK(1, 1, 1, 1));

	// 추출한 x, y 를 더해서 반환
	float result = 0.f;
	simd_store_ss(&result, splatedX + splatedY);

	return result;
}

const Vector2 Vector2::Cross() const
{
	return { -y, x };
}

const Vector2 Vector2::Normalize() const
{
	float len = Length();
	return { x / len, y / len };
}


const Vector2 Vector2::operator+(const Vector2& ref) const
{
	simd a = simd_set_ps(x, y, 0.f, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, 0.f, 0.f);

	return a + b;
}

const Vector2 Vector2::operator-(const Vector2& ref) const
{
	simd a = simd_set_ps(x, y, 0.f, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, 0.f, 0.f);

	return a - b;
}

const Vector2 Vector2::operator*(const Vector2& ref) const
{
	simd a = simd_set_ps(x, y, 0.f, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, 0.f, 0.f);

	return a * b;
}

const Vector2 Vector2::operator/(const Vector2& ref) const
{
	simd a = simd_set_ps(x, y, 0.f, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, 0.f, 0.f);

	return a / b;
}

Vector2& Vector2::operator=(const Vector2& ref)
{
	memcpy(this, &ref, sizeof(Vector2));
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& ref)
{
	*this = *this + ref;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& ref)
{
	*this = *this - ref;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& ref)
{
	*this = *this * ref;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& ref)
{
	*this = *this / ref;
	return *this;
}

bool Vector2::operator==(const Vector2& ref)
{
	simd a = simd_set_ps(x, y, 0.f, 0.f);
	simd b = simd_set_ps(ref.x, ref.y, 0.f, 0.f);

	__m128 equal = (a == b);
	if (_mm_movemask_ps(equal) == 0xffff)
		return true;
	else
		return false;
}

bool Vector2::operator!=(const Vector2& ref)
{
	return not operator==(ref);
}

const Vector2 Vector2::operator+(const float& f) const
{
	simd b = simd_set_ps1(x, y, 0.f, 0.f);
	simd a = simd_set_ps1(f);
	return a + b;
}

const Vector2 Vector2::operator-(const float& f) const
{
	simd b = simd_set_ps1(x, y, 0.f, 0.f);
	simd a = simd_set_ps1(f);
	return a - b;
}

const Vector2 Vector2::operator*(const float& f) const
{
	simd b = simd_set_ps1(x, y, 0.f, 0.f);
	simd a = simd_set_ps1(f);
	return a * b;
}

const Vector2 Vector2::operator/(const float& f) const
{
	simd b = simd_set_ps1(x, y, 0.f, 0.f);
	simd a = simd_set_ps1(f);
	return a / b;
}