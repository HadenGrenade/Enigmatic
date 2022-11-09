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
			if (!v::visuals.player.first)
				break;
				///cplayer
			const auto player = static_cast<CEntity*>(glow.entity);

			if (player->GetTeam() == globals::localPlayer->GetTeam())
				break;
			if (!player->IsAlive())
				break;

			glow.SetColor(v::visuals.player.second.data());
			break;
		}

		case CClientClass::CChicken:
			if (!v::visuals.chicken.first)
				break;

			glow.SetColor(v::visuals.chicken.second.data());
			break;

		case CClientClass::CC4:
		case CClientClass::CPlantedC4:
			if (!v::visuals.bomb.first)
				break;

			glow.SetColor(v::visuals.bomb.second.data());
			break;

		case CClientClass::CBaseCSGrenadeProjectile:
		case CClientClass::CBreachChargeProjectile:
		case CClientClass::CBumpMineProjectile:
		case CClientClass::CDecoyProjectile:
		case CClientClass::CMolotovProjectile:
		case CClientClass::CSensorGrenadeProjectile:
		case CClientClass::CSmokeGrenadeProjectile:
			if (!v::visuals.grenade.first)
				break;

			glow.SetColor(v::visuals.grenade.second.data());
			break;

		default:
			if (!v::visuals.weapon.first)
				break;

			if (!glow.entity->IsWeapon())
				break;

			glow.SetColor(v::visuals.weapon.second.data());
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

						interfaces::surface->DrawOutlinedRect(left - 6, top.y - 1, left - 3, bottom.y + 1);
					}

					const float healthFrac = player->GetHealth() * 0.01f;

					// set the color of the health bar to a split between red / green
					interfaces::surface->DrawSetColor((1.f - healthFrac) * 255, 255 * healthFrac, 0, 255);
					if (v::visuals.health)
						interfaces::surface->DrawFilledRect(left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);

					int x, y;
					interfaces::engine->GetScreenSize(x, y);
					const auto radius = std::tan(Deg2Rad((v::aim.fov))); //huh
					if (v::aim.drawfov)
					{
						interfaces::surface->DrawSetColor(0, 0, 0, 255);
						interfaces::surface->DrawOutlinedCircle(x / 2, y / 2, radius, 20);
					}


				}
			}
		}

	}
}/*
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

}*/