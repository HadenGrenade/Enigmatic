#pragma once

class CEntity;
namespace globals
{
	// global local player pointer
	inline CEntity* localPlayer = nullptr;
	//bool send_packet = true;
	void UpdateLocalPlayer() noexcept;
}
