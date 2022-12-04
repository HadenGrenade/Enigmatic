#pragma once
#include <numbers>
#include <cstdint>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

constexpr float Deg2Rad(const float deg) noexcept
{
	return deg * (std::numbers::pi_v<float> / 180.f);
}

class CVector
{
public:


	constexpr CVector operator+(const CVector& other) const noexcept
	{
		return { x + other.x, y + other.y, z + other.z };
	}
	

	constexpr CVector operator-(const CVector& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	constexpr CVector operator*(const CVector& other) const noexcept
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	constexpr CVector operator/(const CVector& other) const noexcept
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	constexpr CVector Scale(float factor) const noexcept
	{
		return { x * factor, y * factor, z * factor };
	}

	constexpr auto notNull() const noexcept
	{
		return x || y || z;
	}

	constexpr auto null() const noexcept
	{
		return !notNull();
	}
	
	inline CVector clamp(void) {
		return{
		x = std::clamp(x, -89.0f, 89.0f),
		y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f),
		z = std::clamp(z, -50.0f, 50.0f),
		};
	}
	inline CVector normalize() {
		return{
		x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f,
		y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f,

		};
	}
	// convert angles -> vector
	CVector ToVector() const noexcept
	{
		return {
			std::cos(Deg2Rad(x)) * std::cos(Deg2Rad(y)),
			std::cos(Deg2Rad(x)) * std::sin(Deg2Rad(y)),
			-std::sin(Deg2Rad(x))
		};
	}

	inline CVector ToAngle() const noexcept
	{
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	float x{ }, y{ }, z{ };
};

__declspec(align(16)) class CVectorAligned : public CVector
{
public:
	constexpr CVectorAligned operator-(const CVectorAligned& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	float w{ };
};

class Vector2D
{
public:
	constexpr Vector2D(float x = 0.f, float y = 0.f) :
		x(x), y(y) { }

	[[nodiscard]] bool IsZero() const
	{
		return (std::fpclassify(this->x) == FP_ZERO &&
			std::fpclassify(this->y) == FP_ZERO);
	}

public:
	float x, y;
};

//anything past here is 100% pasted
class Vector
{
public:
	constexpr Vector(float x = 0.f, float y = 0.f, float z = 0.f) :
		x(x), y(y), z(z) { }

	constexpr Vector(const float* vec) :
		x(vec[0]), y(vec[1]), z(vec[2]) { }

	constexpr Vector(const Vector2D& vec) :
		x(vec.x), y(vec.y), z(0.0f) { }

	[[nodiscard]] bool IsValid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}


	constexpr void Invalidate()
	{
		this->x = this->y = this->z = std::numeric_limits<float>::infinity();
	}

	[[nodiscard]] float* data()
	{
		return reinterpret_cast<float*>(this);
	}

	[[nodiscard]] const float* data() const
	{
		return reinterpret_cast<float*>(const_cast<Vector*>(this));
	}


	float& operator[](const std::size_t i)
	{
		return this->data()[i];
	}

	const float& operator[](const std::size_t i) const
	{
		return this->data()[i];
	}

	bool operator==(const Vector& vec) const
	{
		return this->IsEqual(vec);
	}

	bool operator!=(const Vector& vec) const
	{
		return !this->IsEqual(vec);
	}

	constexpr Vector& operator=(const Vector& vec)
	{
		this->x = vec.x; this->y = vec.y; this->z = vec.z;
		return *this;
	}

	constexpr Vector& operator=(const Vector2D& vec2D)
	{
		this->x = vec2D.x; this->y = vec2D.y; this->z = 0.0f;
		return *this;
	}

	constexpr Vector& operator+=(const Vector& vec)
	{
		this->x += vec.x; this->y += vec.y; this->z += vec.z;
		return *this;
	}

	constexpr Vector& operator-=(const Vector& vec)
	{
		this->x -= vec.x; this->y -= vec.y; this->z -= vec.z;
		return *this;
	}

	constexpr Vector& operator*=(const Vector& vec)
	{
		this->x *= vec.x; this->y *= vec.y; this->z *= vec.z;
		return *this;
	}

	constexpr Vector& operator/=(const Vector& vec)
	{
		this->x /= vec.x; this->y /= vec.y; this->z /= vec.z;
		return *this;
	}

	constexpr Vector& operator+=(const float add)
	{
		this->x += add; this->y += add; this->z += add;
		return *this;
	}

	constexpr Vector& operator-=(const float subtract)
	{
		this->x -= subtract; this->y -= subtract; this->z -= subtract;
		return *this;
	}

	constexpr Vector& operator*=(const float multiply)
	{
		this->x *= multiply; this->y *= multiply; this->z *= multiply;
		return *this;
	}

	constexpr Vector& operator/=(const float divide)
	{
		this->x /= divide; this->y /= divide; this->z /= divide;
		return *this;
	}

	Vector operator+(const Vector& add) const
	{
		return Vector(this->x + add.x, this->y + add.y, this->z + add.z);
	}

	Vector operator-(const Vector& subtract) const
	{
		return Vector(this->x - subtract.x, this->y - subtract.y, this->z - subtract.z);
	}

	Vector operator*(const Vector& multiply) const
	{
		return Vector(this->x * multiply.x, this->y * multiply.y, this->z * multiply.z);
	}

	Vector operator/(const Vector& divide) const
	{
		return Vector(this->x / divide.x, this->y / divide.y, this->z / divide.z);
	}

	Vector operator+(const float add) const
	{
		return Vector(this->x + add, this->y + add, this->z + add);
	}

	Vector operator-(const float subtract) const
	{
		return Vector(this->x - subtract, this->y - subtract, this->z - subtract);
	}

	Vector operator*(const float multiply) const
	{
		return Vector(this->x * multiply, this->y * multiply, this->z * multiply);
	}

	Vector operator/(const float divide) const
	{
		return Vector(this->x / divide, this->y / divide, this->z / divide);
	}

	[[nodiscard]] bool IsEqual(const Vector& equal, const float errorMargin = std::numeric_limits<float>::epsilon()) const
	{
		return (std::fabsf(this->x - equal.x) < errorMargin &&
			std::fabsf(this->y - equal.y) < errorMargin &&
			std::fabsf(this->z - equal.z) < errorMargin);
	}

	[[nodiscard]] bool IsZero() const
	{
		return (std::fpclassify(this->x) == FP_ZERO &&
			std::fpclassify(this->y) == FP_ZERO &&
			std::fpclassify(this->z) == FP_ZERO);
	}

	[[nodiscard]] Vector2D ToVector2D() const
	{
		return Vector2D(this->x, this->y);
	}

	[[nodiscard]] float Length() const
	{
		return std::sqrtf(this->LengthSqr());
	}

	[[nodiscard]] constexpr float LengthSqr() const
	{
		return DotProduct(*this);
	}

	[[nodiscard]] float Length2D() const
	{
		return std::sqrtf(this->Length2DSqr());
	}

	[[nodiscard]] constexpr float Length2DSqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	[[nodiscard]] float DistTo(const Vector& end) const
	{
		return (*this - end).Length();
	}

	[[nodiscard]] constexpr float DistToSqr(const Vector& end) const
	{
		return (*this - end).LengthSqr();
	}

	[[nodiscard]] Vector Normalized() const
	{
		Vector vecOut = *this;
		vecOut.NormalizeInPlace();
		return vecOut;
	}

	float NormalizeInPlace()
	{
		const float flLength = this->Length();
		const float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

		this->x *= flRadius;
		this->y *= flRadius;
		this->z *= flRadius;

		return flLength;
	}

	[[nodiscard]] constexpr float DotProduct(const Vector& dot) const
	{
		return (this->x * dot.x + this->y * dot.y + this->z * dot.z);
	}

	[[nodiscard]] constexpr Vector CrossProduct(const Vector& cross) const
	{
		return Vector(this->y * cross.z - this->z * cross.y, this->z * cross.x - this->x * cross.z, this->x * cross.y - this->y * cross.x);
	}

	static constexpr auto Deg2Rad(float degrees) noexcept
	{
		return degrees * (std::numbers::pi_v<float> / 180.0f);
	}

	static constexpr auto Rad2Deg(float radians) noexcept
	{
		return radians * (180.0f / std::numbers::pi_v<float>);
	}

	Vector constexpr ToAngle() const noexcept
	{
		return Vector{ Rad2Deg(std::atan2(-z, std::hypot(x, y))),
					   Rad2Deg(std::atan2(y, x)),
					   0.0f };
	}

	static Vector FromAngle(const Vector& angle) noexcept
	{
		return Vector{ std::cos(Deg2Rad(angle.x)) * std::cos(Deg2Rad(angle.y)),
					   std::cos(Deg2Rad(angle.x)) * std::sin(Deg2Rad(angle.y)),
					  -std::sin(Deg2Rad(angle.x)) };
	}

	static inline Vector RelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles)
	{
		return ((destination - source).ToAngle() - viewAngles);
	}

public:
	float x, y, z;
};

class Vector4D
{
public:
	constexpr Vector4D(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f) :
		x(x), y(y), z(z), w(w) { }

public:
	float x, y, z, w;
};
using Matrix3x3 = float[3][3];
struct Matrix3x4
{
	Matrix3x4() = default;

	constexpr Matrix3x4(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		data[0][0] = m00; data[0][1] = m01; data[0][2] = m02; data[0][3] = m03;
		data[1][0] = m10; data[1][1] = m11; data[1][2] = m12; data[1][3] = m13;
		data[2][0] = m20; data[2][1] = m21; data[2][2] = m22; data[2][3] = m23;
	}

	constexpr Matrix3x4(const Vector& x, const Vector& y, const Vector& z, const Vector& origin)
	{
		Init(x, y, z, origin);
	}

	constexpr void Init(const Vector& forward, const Vector& left, const Vector& up, const Vector& origin)
	{
		SetForward(forward);
		SetLeft(left);
		SetUp(up);
		SetOrigin(origin);
	}

	constexpr void SetForward(const Vector& forward)
	{
		this->data[0][0] = forward.x;
		this->data[1][0] = forward.y;
		this->data[2][0] = forward.z;
	}

	constexpr void SetLeft(const Vector& left)
	{
		this->data[0][1] = left.x;
		this->data[1][1] = left.y;
		this->data[2][1] = left.z;
	}

	constexpr void SetUp(const Vector& up)
	{
		this->data[0][2] = up.x;
		this->data[1][2] = up.y;
		this->data[2][2] = up.z;
	}

	constexpr void SetOrigin(const Vector& origin)
	{
		this->data[0][3] = origin.x;
		this->data[1][3] = origin.y;
		this->data[2][3] = origin.z;
	}

	constexpr void Invalidate()
	{
		for (auto& subData : data)
		{
			for (auto& number : subData)
				number = std::numeric_limits<float>::infinity();
		}
	}

	float* operator[](const int index)
	{
		return data[index];
	}

	const float* operator[](const int index) const
	{
		return data[index];
	}

	[[nodiscard]] constexpr Vector at(const int index) const
	{
		return Vector(data[0][index], data[1][index], data[2][index]);
	}

	float* Base()
	{
		return &data[0][0];
	}

	[[nodiscard]] const float* Base() const
	{
		return &data[0][0];
	}

	float data[3][4] = { };
};