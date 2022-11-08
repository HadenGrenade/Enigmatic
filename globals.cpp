#include "src/semicore/globals.h"

#include "src/semicore/interfaces.h"

void globals::UpdateLocalPlayer() noexcept
{

	const std::int32_t localPlayerIndex = interfaces::engine->GetLocalPlayerIndex();

	localPlayer = interfaces::entityList->GetEntityFromIndex(localPlayerIndex);
}