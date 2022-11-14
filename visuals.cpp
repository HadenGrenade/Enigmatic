#include "visuals.h"
#include "iglowmanger.h"
#include "src/hooks.h"
#include "misc.h"
#include <string>
#include "src/hooks.h"
int x, y;

static bool WorldToScreen(const CVector& point, CVector& screen) noexcept
{

	// matrix
	const CMatrix4x4& matrix = interfaces::engine->WorldToScreenMatrix();


	float w = matrix.data[3][0] * point.x + matrix.data[3][1] * point.y + matrix.data[3][2] * point.z + matrix.data[3][3];
	float inverse = 1.f / w;
	if (w < 0.001f)
		return false;

	screen.x = (matrix.data[0][0] * point.x + matrix.data[0][1] * point.y + matrix.data[0][2] * point.z + matrix.data[0][3]) * inverse;
	screen.y = (matrix.data[1][0] * point.x + matrix.data[1][1] * point.y + matrix.data[1][2] * point.z + matrix.data[1][3]) * inverse;

	interfaces::engine->GetScreenSize(x, y);

	screen.x = (x * 0.5f) + (screen.x * x) * 0.5f;
	screen.y = (y * 0.5f) - (screen.y * y) * 0.5f;

	// if youre on screen, so to use the function you have to check if WorldToScreen is true. yw pysik <3
	return true;
}

void Visuals::Glow()
{
	if (!globals::localPlayer)
		return;

	for (auto i = 0; i < interfaces::glow->glowObjectDefinitions.Count(); ++i)
	{
		auto& glow = interfaces::glow->glowObjectDefinitions[i];

		if (glow.IsUnused())
			continue;

		if (!glow.entity)
			continue;

		const auto clientClass = glow.entity->GetClientClass();

		if (!clientClass)
			continue;

		switch (clientClass->classID)
		{
		case CClientClass::CCSPlayer:
		{
			if (!v::visuals.Player.first)
				break;
				///cplayer
			const auto player = static_cast<CEntity*>(glow.entity);

			if (player->GetTeam() == globals::localPlayer->GetTeam())
				break;
			if (!player->IsAlive())
				break;

			glow.SetColor(v::visuals.Player.second.data());
			break;
		}

		case CClientClass::CChicken:
			if (!v::visuals.Chicken.first)
				break;

			glow.SetColor(v::visuals.Chicken.second.data());
			break;

		case CClientClass::CC4:
		case CClientClass::CPlantedC4:
			if (!v::visuals.Bomb.first)
				break;

			glow.SetColor(v::visuals.Bomb.second.data());
			break;

		case CClientClass::CBaseCSGrenadeProjectile:
		case CClientClass::CBreachChargeProjectile:
		case CClientClass::CBumpMineProjectile:
		case CClientClass::CDecoyProjectile:
		case CClientClass::CMolotovProjectile:
		case CClientClass::CSensorGrenadeProjectile:
		case CClientClass::CSmokeGrenadeProjectile:
			if (!v::visuals.Grenade.first)
				break;

			glow.SetColor(v::visuals.Grenade.second.data());
			break;

		default:
			if (!v::visuals.Weapon.first)
				break;

			if (!glow.entity->IsWeapon())
				break;

			glow.SetColor(v::visuals.Weapon.second.data());
			break;
		}
	}
}


void Visuals::esp(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) {
	{
		if (vguiPanel == interfaces::engineVGui->GetPanel(PANEL_TOOLS))
		{
			if (interfaces::engine->IsInGame() && globals::localPlayer)
			{
				for (int i = 1; i <= interfaces::globals->maxClients; ++i)
				{
					// get the player
					CEntity* player = interfaces::entityList->GetEntityFromIndex(i);

					// make sure player is valid
					if (!player)
						continue;

					if (player->IsDormant() || !player->IsAlive())
						continue;

					if (player->GetTeam() == globals::localPlayer->GetTeam())
						continue;

					if (!globals::localPlayer->IsAlive())
						if (globals::localPlayer->GetObserverTarget() == player)
							continue;

					// player's bone matrix
					CMatrix3x4 bones[128];
					if (!player->SetupBones(bones, 128, 0x7FF00, interfaces::globals->currentTime))
						continue;

					// screen position of head
					// we add 11.f here because we want the box ABOVE their head
					CVector top;
					if (!WorldToScreen(bones[8].Origin() + CVector{ 0.f, 0.f, 11.f }, top))
						continue;

					// screen position of feet
					// we subtract 9.f here because we want the box BELOW their feet
					CVector bottom;
					if (!WorldToScreen(player->GetAbsOrigin() - CVector{ 0.f, 0.f, 9.f }, bottom))
						continue;

					// the height of the box is the difference between
					// the bottom (larger number) and the top
					const float h = bottom.y - top.y;

					// we can use the height to determine a width
					const float w = h * 0.3f;

					const auto left = static_cast<int>(top.x - w);
					const auto right = static_cast<int>(top.x + w);
					if (v::visuals.box) {
						interfaces::surface->DrawSetColor(255, 255, 255, 255);
						interfaces::surface->DrawOutlinedRect(left, top.y, right, bottom.y);

						// set the color to black for outlines
						interfaces::surface->DrawSetColor(0, 0, 0, 255);

						interfaces::surface->DrawOutlinedRect(left - 1, top.y - 1, right + 1, bottom.y + 1);
						interfaces::surface->DrawOutlinedRect(left + 1, top.y + 1, right - 1, bottom.y - 1);

						
					}

					const float healthFrac = player->GetHealth() * 0.01f;

					// set the color of the health bar to a split between red / green
					
					if (v::visuals.health)
					{
						interfaces::surface->DrawOutlinedRect(left - 6, top.y - 1, left - 3, bottom.y + 1);
						// set the color of the health bar to a split between red / green
						interfaces::surface->DrawSetColor((1.f - healthFrac) * 255, 255 * healthFrac, 0, 255);
						interfaces::surface->DrawFilledRect(left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);
					}


				}
				if (v::aim.legitaim)
				{

					if (!v::aim.aimbot)
						return;

					int x, y;
					interfaces::engine->GetScreenSize(x, y);
					const auto radius(v::aim.fov * 10); //make this the actual fov, I had issues with painttraverse doing it tho 
					if (v::aim.drawfov)
					{
						interfaces::surface->DrawSetColor(255, 255, 255, 255);
						interfaces::surface->DrawOutlinedCircle(x / 2, y / 2, radius, 50);
					}
				}
			}

		}

	}
}

void Visuals::chams(void* results, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags) noexcept {
	if(!v::visuals.chams)
		return;
	if (globals::localPlayer && info.renderable)
	{
		// get the base entity
		CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

		// make sure they are a valid enemy player
		if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam())
		{
			// create our custom material
			static IMaterial* material = interfaces::materialSystem->CreateMaterial(
				"pearlescent",
				CKeyValues::FromString(
					"VertexLitGeneric",
					"$phong 1 "
					"$basemapalphaphongmask 1 "
					"$pearlescent 2" 
				)
			);
		//	static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

			// float arrays to hold our chams colors
			// put these in globals:: to modify with a menu
			constexpr float hidden[3] = { 0.f, 1.f, 0.f };
			constexpr float visible[3] = { 1.f, 0.f, 1.f };

			// alpha modulate (once in my case)
			interfaces::studioRender->SetAlphaModulation(1.f);

			// show through walls
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(hidden);
			interfaces::studioRender->ForcedMaterialOverride(material);
			hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
			if (v::visuals.chams1) {
				// do not show through walls
				material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
				interfaces::studioRender->SetColorModulation(visible);
				interfaces::studioRender->ForcedMaterialOverride(material);
				hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
			}
			// reset the material override + return from hook
			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}
	}

}

void Visuals::SpreadCrosshair() {


	// get active weapon.
	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();
	if (!activeWeapon)
		return;

	const int weaponType = activeWeapon->GetWeaponType();
	if (!weaponType)
		return;

	// do not do this on: bomb, knife and nades.
	if (weaponType == WEAPON_KNIFE || weaponType == WEAPON_C4 || weaponType == WEAPON_FRAG_GRENADE || weaponType == WEAPON_FIREBOMB || weaponType == WEAPON_MOLOTOV || weaponType == WEAPON_SMOKEGRENADE)
		return;

	// moneybot spread circle... change later to normal cuz fps drops if it on or just get_random_float less
	if (activeWeapon) {
		int screen_w, screen_h;
		interfaces::engine->GetScreenSize(screen_w, screen_h);
		int cross_x = screen_w / 2, cross_y = screen_h / 2;

		float recoil_step = screen_h / 90.f;

		CVector aimPunch{ };
		globals::localPlayer->GetAimPunch(aimPunch);
		CVector currentPunch = aimPunch;
		cross_x -= (int)(currentPunch.y * recoil_step);
		cross_y += (int)(currentPunch.x * recoil_step);

		activeWeapon->UpdateAccuracyPenalty();
		float inaccuracy = activeWeapon->GetInaccuracy();
		float spread = activeWeapon->GetSpread();

		float cone = inaccuracy * spread;
		cone *= screen_h * 0.7f;
		cone *= 90.f / 90.f;

		//generate a random float function
		auto random_float = [](float min, float max) -> float
		{
			using random_float_t = float(__cdecl*)(float, float);
			static random_float_t random_float_fn = reinterpret_cast<random_float_t>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat"));
			return random_float_fn(min, max);
		};
		

		for (int seed{ }; seed < 256; ++seed) { //draw 256 dot things
			//render::set_random_seed(math::random_number(0, 255) + 1);
			float rand_a = random_float(0.f, 1.0f);
			float pi_rand_a = random_float(0.f, 2.0f * M_PI);
			float rand_b = random_float(0.0f, 1.0f);
			float pi_rand_b = random_float(0.f, 2.f * M_PI);

			float spread_x = cos(pi_rand_a) * (rand_a * inaccuracy) + cos(pi_rand_b) * (rand_b * spread);
			float spread_y = sin(pi_rand_a) * (rand_a * inaccuracy) + sin(pi_rand_b) * (rand_b * spread);

			float max_x = cos(pi_rand_a) * cone + cos(pi_rand_b) * cone;
			float max_y = sin(pi_rand_a) * cone + sin(pi_rand_b) * cone;

			float step = screen_h /90.f * 90.f;
			int screen_spread_x = (int)(spread_x * step * 0.7f);
			int screen_spread_y = (int)(spread_y * step * 0.7f);

			float percentage = (rand_a * inaccuracy + rand_b * spread) / (inaccuracy + spread);

			interfaces::surface->DrawFilledRect(cross_x + screen_spread_x, cross_y + screen_spread_y, 1, 1);
		}
	}
}
