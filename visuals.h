#pragma once
#include "src/semicore/interfaces.h"

class Visuals
{
public:

	void Glow();

	void esp(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce);

};
