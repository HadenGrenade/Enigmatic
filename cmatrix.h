#pragma once
#include "Vector3d.h"
#include "src/valve/cvector.h"
class CMatrix3x4
{
public:
	constexpr float* operator[](size_t index) noexcept
	{
		return data[index];
	}

	 CVector Origin1() noexcept
	{
		return { data[0][3], data[1][3], data[2][3] };
	}


	 vec3_t Origin2() noexcept
	 {
		 return { data[0][3], data[1][3], data[2][3] };
	 }

	float data[3][4];
};

class CMatrix4x4
{
public:
	float data[4][4];
};
