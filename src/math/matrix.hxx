#pragma once

#include "../../includes.hxx"

class Matrix3x4 {
public:
	constexpr float* operator[](size_t index) noexcept
	{
		return data[index];
	}

	constexpr Vector3<float> Origin() noexcept
	{
		return { data[0][3], data[1][3], data[2][3] };
	}

	float data[3][4];
};