#pragma once
#include "gui.h"
#include "valve/cusercmd.h"
#include "../ivpanel.h"
#include "../clientmode.h"

#include "../istudiorender.h"
#include "../ispatialquery.h"


namespace hooks
{
	void Setup();
	void Destroy() noexcept;
	void proxy();


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
	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd, bool& send_packet) noexcept;
	// PaintTraverse hook stuff
	
		// ListLeavesInBox hook
	using ListLeavesInBoxFn = int(__thiscall*)(ISpacialQuery*, const CVector&, const CVector&, std::uint16_t*, int) noexcept;
	inline ListLeavesInBoxFn ListLeavesInBoxOriginal = nullptr;
	int __stdcall ListLeavesInBox(const CVector& mins, const CVector& maxs, std::uint16_t* list, int listMax) noexcept;


	
	using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool, bool) noexcept;
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;

	using DrawModelFn = void(__thiscall*)(
		void*,
		void*,
		const CDrawModelInfo&,
		CMatrix3x4*,
		float*,
		float*,
		const CVector&,
		const std::int32_t
		) noexcept;
	inline DrawModelFn DrawModelOriginal = nullptr;
	void __stdcall DrawModel(
		void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWeights,
		const CVector& modelOrigin,
		const std::int32_t flags
	) noexcept;

	using DoPostScreenSpaceEffectsFn = int(__thiscall*)(void*, const ViewSetup*);
	inline DoPostScreenSpaceEffectsFn DoPostScreenSpaceEffectsOriginal = { nullptr };
	int __stdcall DoPostScreenSpaceEffects(const ViewSetup* view);


}



