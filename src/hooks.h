#pragma once
#include "gui.h"
#include "valve/cusercmd.h"
#include "../iclientmodeshared.h"
#include "../ivpanel.h"
#include "../clientmode.h"
namespace hooks
{
	void Setup();
	void Destroy() noexcept;


	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
	{
		return (*static_cast<void***>(thisptr))[index];
	}


	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	using CreateMoveFunction = bool(__thiscall*)(void*, float, CUserCmd*) noexcept;
	inline CreateMoveFunction CreateMoveOriginal = nullptr;
	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd) noexcept;
	// PaintTraverse hook stuff
	using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool, bool) noexcept;
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;

	using DoPostScreenSpaceEffectsFn = int(__thiscall*)(void*, const ViewSetup*);
	inline DoPostScreenSpaceEffectsFn DoPostScreenSpaceEffectsOriginal = { nullptr };
	int __stdcall DoPostScreenSpaceEffects(const ViewSetup* view);


}
