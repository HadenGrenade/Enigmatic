#pragma once
#include "src/semicore/ainterfaces.h"

class Visuals
{
public:

	void Glow();

	void esp(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce);

	void chams(void* results, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags) noexcept;

	void SpreadCrosshair();

};
