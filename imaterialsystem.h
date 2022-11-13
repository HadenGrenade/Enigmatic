#pragma once

#include "ckeyvalues.h"

class IMaterial
{
public:
	enum EMaterialVarFlag : std::int32_t
	{
		IGNOREZ = 1 << 15,
		WIREFRAME = 1 << 28
	};

	virtual const char* getname() const = 0;

	constexpr void SetMaterialVarFlag(const std::int32_t flag, const bool on) noexcept
	{
		memory::Call<void>(this, 29, flag, on);
	}

	constexpr void IncrementReferenceCount() noexcept
	{
		memory::Call<void>(this, 14);
	}

	constexpr void AlphaModulate(float alpha) noexcept
	{
		memory::Call<void>(this, 27, alpha);
	}

	constexpr void ColorModulate(float r, float g, float b) noexcept
	{
		memory::Call<void>(this, 28, r, g, b);
	}
};

class IMaterialSystem
{
public:
	constexpr IMaterial* CreateMaterial(const char* name, CKeyValues* kv) noexcept
	{
		return memory::Call<IMaterial*>(this, 83, name, kv);
	}

	constexpr IMaterial* FindMaterial(const char* name, const char* textureGroupName = nullptr, bool complain = true, const char* complainPrefix = nullptr) noexcept
	{
		return memory::Call<IMaterial*>(this, 84, name, nullptr, true, nullptr);
	}

	constexpr uint16_t FirstMaterial()
	{
		return memory::Call<short>(this, 86);
	}

	constexpr uint16_t NextMaterial(short handle)
	{
		return memory::Call<short>(this, 87, handle);
	}

	constexpr uint16_t InvalidMaterial()
	{
		return memory::Call<short>(this, 88);
	}

	constexpr IMaterial* GetMaterial(short handle)
	{
		return memory::Call<IMaterial*>(this, 89, handle);
	}


};