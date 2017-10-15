#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "GlowManager.h"

DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);

#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif


void CEsp::Init()
{
	BombCarrier = nullptr;
}

void CEsp::Move(CUserCmd *pCmd,bool &bSendPacket) 
{

}

void CEsp::Draw()
{
	IClientEntity *pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
		{
			if (Menu::Window.VisualsTab.OtherRadar.GetState())
			{
				DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
				*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
			}

			if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{
				DrawPlayer(pEntity, pinfo);
			}

			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CBaseCSGrenadeProjectile)
					DrawHE(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CMolotovProjectile)
					DrawMolotov(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CDecoyProjectile)
					DrawDecoy(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CSensorGrenadeProjectile)
					DrawMolotov(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CSmokeGrenadeProjectile)
					DrawSmoke(pEntity, cClass);
			}

			if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
			{
				DrawDrop(pEntity, cClass);
			}

			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					DrawBombPlanted(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CC4)
					DrawBomb(pEntity, cClass);
			}

			if (Menu::Window.VisualsTab.FiltersChickens.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
					DrawChicken(pEntity, cClass);
			}
		}
	}

	if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
	{
		DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
		*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = 0;
	}

	if (Menu::Window.VisualsTab.OptionsGlow.GetState())
	{
		DrawGlow();
	}
	if (Menu::Window.VisualsTab.EntityGlow.GetState())
	{
		EntityGlow();
	}

	if (Menu::Window.VisualsTab.GrenadeTrace.GetState())
	{
		GrenadeTrace();
	}
}

void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	ESPBox Box;
	Color Color;

	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;

	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		switch (Menu::Window.VisualsTab.OptionsBox.GetIndex())
		{
		case 0:
			break;
		case 1:
			DrawBox(Box, Color);
			break;
		case 2:
			FilledBox(Box, Color);
			break;
		case 3:
			Corners(Box, Color, pEntity);
			break;
		}

		switch (Menu::Window.VisualsTab.OptionsWeapon.GetIndex())
		{
		case 0:
			break;
		case 1:
			DrawWeapon(pEntity, Box);
			break;
		case 2:
			DrawIcon(pEntity, Box);
			break;
		}

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);

		if (Menu::Window.VisualsTab.OptionsHealth.GetState())
			DrawHealth(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsInfo.GetState())
			DrawInfo(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsArmor.GetState())
			Armor(pEntity, Box);

		if (Menu::Window.VisualsTab.Barrels.GetState())
			Barrel(Box, Color, pEntity);

		if (Menu::Window.VisualsTab.OptionsDefusing.GetState())
			IsPlayerDefusing(pinfo, Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
			DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);

		if (Menu::Window.VisualsTab.Money.GetState())
			DrawMoney(pEntity, Box);

		if (Menu::Window.VisualsTab.Distance.GetState())
			DrawDistance(Box, pEntity);

			Info(pEntity, Box);

	}
}

bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.TColorNoVisR.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255);
	}
	else
	{
		if (IsVis)
			color = Color(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255);
		else
			color = Color(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255);
	}

	return color;
}

void CEsp::Corners(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.30f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(0, 0, 0, 255));

	Render::Clear(size.x - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, HorzLine, Color(0, 0, 0, 255));
	Render::Clear(size.x + size.w - 1, size.y + size.h - HorzLine, 1, HorzLine, Color(0, 0, 0, 255));

	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
}

void CEsp::FilledBox(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, 1, HorzLine, color);
	Render::Clear(size.x, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, HorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - HorzLine, 1, HorzLine, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
}

void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	Render::Outline(size.x, size.y, size.w, size.h, color);
	Render::Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
	Render::Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
}

void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


	y = ScreenHeight;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(0, 255, 0, 255));
}

void CEsp::DrawWeapon(IClientEntity* pEntity, CEsp::ESPBox size)
{
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 1 && pWeapon)
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pWeapon->GetpWeaponName());
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 8,
			Color(255, 255, 255, 255), Render::Fonts::ESP, pWeapon->GetpWeaponName());
	}
}

void CEsp::DrawIcon(IClientEntity* pEntity, CEsp::ESPBox size)
{
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetIndex() == 2 && pWeapon)
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::Icon, pWeapon->GetGunIcon());
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y + size.h + 8,
			Color(255, 255, 255, 255), Render::Fonts::Icon, pWeapon->GetGunIcon());
	}
}

void CEsp::DrawGlow()
{
	int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 35:
			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
			{
				if (!Menu::Window.VisualsTab.FiltersPlayers.GetState() && !(Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;
				if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
					break;

				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}

				else
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(GlowR, GlowG, GlowB, GlowZ) : Color(GlowR, GlowG, GlowB, GlowZ));
				}
			}
		}
	}
}

void CEsp::EntityGlow()
{
	int GlowR = Menu::Window.ColorsTab.GlowR.GetValue();
	int GlowG = Menu::Window.ColorsTab.GlowG.GetValue();
	int GlowB = Menu::Window.ColorsTab.GlowB.GetValue();
	int GlowZ = Menu::Window.VisualsTab.GlowZ.GetValue();

	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 1:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (Menu::Window.VisualsTab.EntityGlow.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 9:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				if (Menu::Window.VisualsTab.EntityGlow.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 29:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 39:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 41:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 66:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 87:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 98:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 108:
			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 130:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		case 134:
			if (Menu::Window.VisualsTab.FiltersNades.GetState())
			{
				glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		default:
			if (Menu::Window.VisualsTab.EntityGlow.GetState())
			{
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "Weapon"))
					glowEntity->set(Color(GlowR, GlowG, GlowB, GlowZ));
			}
		}
	}
}

static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size/4, text, size);
	return wa;
}

void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16,
		Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
}

void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);


	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;

	Render::DrawOutlinedRect((size.x - 6) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

	Render::DrawLine((size.x - 6), size.y + hp, (size.x - 6), size.y + h, Color(Red, Green, 0, 180));

	if (health < 100) {

		Render::Text(size.x - 9, size.y + hp, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);
	}
}

std::string CleanItemName(std::string name)
{
	std::string Name = name;
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("Bomb Carrier");
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	IClientEntity* Weapon = (IClientEntity*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
		{
			Render::Outline(Box.x - 6, Box.y - 6, 12, 12, Color(255, 255, 255, 255));
		}
		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
		{
			RECT TextSize = Render::GetTextSize(Render::Fonts::Icon, Weapon->GetGunIcon());
			Render::Text(Box.x - (TextSize.right / 1), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::Icon, Weapon->GetGunIcon());
		}
	}
}

void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Chicken");
		if (Menu::Window.VisualsTab.FiltersChickens.GetState())
			DrawBox(Box, Color(255,255,255,255));

		if (Menu::Window.VisualsTab.FiltersChickens.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass) 
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		sprintf_s(buffer, "explodes in %.1f", TimeRemaining);
		Render::Text(vScreen.x, vScreen.y, Color(255, 255, 255, 255), Render::Fonts::ESP, buffer);
	}
}

void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(112, 230, 20, 255), Render::Fonts::ESP, "Bomb");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(255,255,255,255));
			}
		}
	}
}

void CEsp::IsPlayerDefusing(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity)
{
	RECT defSize = Render::GetTextSize(Render::Fonts::ESP, "");
	if (pEntity->IsDefusing())
	{
		Render::Text(size.x + size.w + 3, size.y + (0.3*(defSize.bottom + 15)),
			Color(255, 0, 0, 255), Render::Fonts::ESP, charenc("Defusing"));
	}
}

void CEsp::DrawMoney(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;

	int MoneyEnemy = 100;
	MoneyEnemy = pEntity->GetMoney();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d $", MoneyEnemy);

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, nameBuffer);
	Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 27, Color(255, 255, 0, 255), Render::Fonts::ESP, nameBuffer);
}

void CEsp::Armor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArBar = size;
	ArBar.y += (ArBar.h + 3);
	ArBar.h = 6;

	float ArValue = pEntity->ArmorValue();
	float ArPerc = ArValue / 100.f;
	float Width = (size.w * ArPerc);
	ArBar.w = Width;

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(ArBar.x, ArBar.y));
	Verts[1].Init(Vector2D(ArBar.x + size.w + 0, ArBar.y));
	Verts[2].Init(Vector2D(ArBar.x + size.w, ArBar.y + 2));
	Verts[3].Init(Vector2D(ArBar.x - 0, ArBar.y + 2));

	Render::PolygonOutline(4, Verts, Color(50, 50, 50, 255), Color(50, 50, 50, 255));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(ArBar.x, ArBar.y + 1));
	Verts2[1].Init(Vector2D(ArBar.x + ArBar.w + 0, ArBar.y + 1));
	Verts2[2].Init(Vector2D(ArBar.x + ArBar.w, ArBar.y + 2));
	Verts2[3].Init(Vector2D(ArBar.x, ArBar.y + 2));

	Color c = GetPlayerColor(pEntity);
	Render::Polygon(4, Verts2, Color(0, 120, 255, 200));
}


void CEsp::DrawMolotov(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Fire");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawSmoke(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Smoke");

		if (Menu::Window.VisualsTab.FiltersNades.GetState() == 1)
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawDecoy(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Decoy");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::DrawHE(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;

	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "HE or Flash");

		if (Menu::Window.VisualsTab.FiltersNades.GetState())
			DrawName(pinfo, Box);
	}
}

void CEsp::Info(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	if (Menu::Window.VisualsTab.HasDefuser.GetState() && pEntity->HasDefuser())
	{
		Info.push_back("Has Defuser");
	}

	if (Menu::Window.VisualsTab.IsScoped.GetState() && pEntity->IsScoped())
	{
		Info.push_back("Scoped");
	}

	static RECT Size = Render::GetTextSize(Render::Fonts::ESP, "Hi");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

void CEsp::GrenadeTrace()
{
	auto granade = Interfaces::CVar->FindVar("sv_grenade_trajectory");
	auto granadespoof = new SpoofedConvar(granade);
	granadespoof->SetInt(1);
}

void CEsp::DrawDistance(CEsp::ESPBox size, IClientEntity* pEntity)
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = pLocal->GetOrigin();
	static RECT defSize = Render::GetTextSize(Render::Fonts::Default, "");

	char dist_to[32];
	sprintf_s(dist_to, "%.0f m", DistanceTo(vecOriginLocal, vecOrigin));

	Render::Text(size.x + size.w + 3, size.y + (0.6*(defSize.bottom + 28)), Color(255, 255, 255, 255), Render::Fonts::ESP, dist_to);
}

float CEsp::DistanceTo(Vector vecSrc, Vector vecDst)
{
	Vector vDelta = vecDst - vecSrc;

	float fDistance = ::sqrtf((vDelta.Length()));

	if (fDistance < 1.0f)
		return 1.0f;

	return fDistance;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mrgzhgu {
public:
	int kzhnzgawjygbcpg;
	string mbxlideqh;
	int yyyfutrkhyxn;
	mrgzhgu();
	double vyutgwoigkeanwawrxq(string tzgmcxcysqw, string xslwrvbnwhqb, bool pqdhofgnhnnwac);
	int woxevrfbjjcos(double unesfhkrwgipode, int hhgwu, double vdhiyxpzbmn, double dnqvwqzpy, string rucxldrk, string kjvixasmjtwa, string ofbwfwvsuxh, int uqjrxvhuurvnaoh, int qishtdboy, string uktvkjkmmzgan);
	string plhdijjznwjvobojwtx(string bfaoxzjjjknt);
	bool ivkyaxipjaborsko(string zrpura, double tyaygc, double pnzulxrur, int qhaxtclwpex);
	double chbwscceqxibyaskcag(double owldvkq, double fqikf, string cyhyctbvjixoovu);

protected:
	string mxdkkqyn;
	bool cbvvyrc;
	double etifntkc;
	string kankwptfb;

	bool qmzysljuyj(double qpcsbomjnitsij, double sbqlvleaoy, bool ewtcna, bool raluobayuzsa, string yrqztswohr, bool mzckm, bool cwepeqxtklkycw, string nigrihiqbamhp);
	bool pulibnakcpdcgetvkzbstdurp(bool hidizvmgw, bool mqdnwzqrpyrsrg, double aekqryrawd, int yykmuotxgq, bool pwrwrpilkfsrk, double orzbmppcimbz, double ndtqpsyknwzdl);
	string ekrilobgbckcstlxjmt();
	double ihyvzuzbfnttmhgws(int vdukl, double yasfspmo);
	double wkbwqeybpsjjduialrzn(double femydjluwnji, bool ogekdxny, double jdhsniriob, double kvfgwokjxbas);
	double ohygfvisdsraoxp(double aovgkrgpmfpghxp, int bkztjz, bool pyiotf, double egkdpnmblr, string hdeqplzyujzu, double seeskywev, double pkgaaatboetpgu, int wjujnbbwwbkbdna, bool cdukyujxgjt, bool dqhqay);

private:
	string kbuetm;
	bool yeajxlplsyjhgwo;
	double mgxpu;

	int zoybggxaqbkxpeisfn(int cuioptj, double ymtrxfovfjwqmr, bool bzcocgukmqribw, string piezldntoupgek, double lamowusxqxzzd, string rmxeolueiglx, int cjdewmc);
	bool zngdejeymxxkjslrbivlsy(int wnsvyf, string myhombotqsckvw, double ezvyosreodjfdtq, bool fzknawzlufwrkg, bool fruzugxmx, int fhdfudhgiiogv);
	int asrsgbrfio();

};


int mrgzhgu::zoybggxaqbkxpeisfn(int cuioptj, double ymtrxfovfjwqmr, bool bzcocgukmqribw, string piezldntoupgek, double lamowusxqxzzd, string rmxeolueiglx, int cjdewmc) {
	string qibkx = "huyvslhztplgvcyxbcaythjkfczlia";
	double eperqeoimka = 30431;
	double pdoitcdidmpco = 15521;
	double djvndc = 37328;
	if (15521 != 15521) {
		int itghflzjav;
		for (itghflzjav = 30; itghflzjav > 0; itghflzjav--) {
			continue;
		}
	}
	if (30431 == 30431) {
		int hskqeecrw;
		for (hskqeecrw = 98; hskqeecrw > 0; hskqeecrw--) {
			continue;
		}
	}
	if (37328 == 37328) {
		int ykm;
		for (ykm = 56; ykm > 0; ykm--) {
			continue;
		}
	}
	if (37328 != 37328) {
		int qfhsdljwwa;
		for (qfhsdljwwa = 94; qfhsdljwwa > 0; qfhsdljwwa--) {
			continue;
		}
	}
	return 69586;
}

bool mrgzhgu::zngdejeymxxkjslrbivlsy(int wnsvyf, string myhombotqsckvw, double ezvyosreodjfdtq, bool fzknawzlufwrkg, bool fruzugxmx, int fhdfudhgiiogv) {
	string hzvjwcdcyirno = "ayqgndl";
	bool rselkuhx = false;
	bool ptwuorltibwj = true;
	double oweusdjatcn = 17447;
	double wjfxrkiolnept = 45607;
	bool tlqyfhttcuq = false;
	double qgvqobpcbyv = 47;
	if (45607 != 45607) {
		int kzniazbl;
		for (kzniazbl = 2; kzniazbl > 0; kzniazbl--) {
			continue;
		}
	}
	if (string("ayqgndl") != string("ayqgndl")) {
		int ihene;
		for (ihene = 94; ihene > 0; ihene--) {
			continue;
		}
	}
	if (47 == 47) {
		int dzuxefiju;
		for (dzuxefiju = 13; dzuxefiju > 0; dzuxefiju--) {
			continue;
		}
	}
	if (true == true) {
		int vajq;
		for (vajq = 14; vajq > 0; vajq--) {
			continue;
		}
	}
	if (false == false) {
		int irgoiuvzo;
		for (irgoiuvzo = 31; irgoiuvzo > 0; irgoiuvzo--) {
			continue;
		}
	}
	return true;
}

int mrgzhgu::asrsgbrfio() {
	double rakndkfsz = 20473;
	double nwbeowbsmsuw = 28819;
	double qwyqjocdlpw = 20078;
	if (20078 == 20078) {
		int byszfnsuv;
		for (byszfnsuv = 98; byszfnsuv > 0; byszfnsuv--) {
			continue;
		}
	}
	if (28819 == 28819) {
		int to;
		for (to = 91; to > 0; to--) {
			continue;
		}
	}
	return 22774;
}

bool mrgzhgu::qmzysljuyj(double qpcsbomjnitsij, double sbqlvleaoy, bool ewtcna, bool raluobayuzsa, string yrqztswohr, bool mzckm, bool cwepeqxtklkycw, string nigrihiqbamhp) {
	bool ykmllyaxr = true;
	double wwgccublzg = 11770;
	if (11770 != 11770) {
		int odhawybtbw;
		for (odhawybtbw = 5; odhawybtbw > 0; odhawybtbw--) {
			continue;
		}
	}
	if (true == true) {
		int ualjosdqjs;
		for (ualjosdqjs = 60; ualjosdqjs > 0; ualjosdqjs--) {
			continue;
		}
	}
	if (true == true) {
		int cvuqdfoetk;
		for (cvuqdfoetk = 68; cvuqdfoetk > 0; cvuqdfoetk--) {
			continue;
		}
	}
	if (11770 == 11770) {
		int yeyqtyqxfs;
		for (yeyqtyqxfs = 38; yeyqtyqxfs > 0; yeyqtyqxfs--) {
			continue;
		}
	}
	if (11770 != 11770) {
		int cz;
		for (cz = 38; cz > 0; cz--) {
			continue;
		}
	}
	return false;
}

bool mrgzhgu::pulibnakcpdcgetvkzbstdurp(bool hidizvmgw, bool mqdnwzqrpyrsrg, double aekqryrawd, int yykmuotxgq, bool pwrwrpilkfsrk, double orzbmppcimbz, double ndtqpsyknwzdl) {
	string ioyletjqzlalq = "ephthfnwmtpyrbdsaihsillsxubwrgfdjxdoijcrvginuznkymrfqrdbzaykstffkztwo";
	string rfmvtgiscajfcs = "pbtufdzfeizbizwaqylzgfpgxbjzbpagvnjzoqfpwqjgltbblchbofyhqtqtpxt";
	int ekaulzoi = 1831;
	bool oqhutqszig = false;
	string atyiyvgqwoo = "pnlcjatyihcnprqtidmcrksffdkoajycsuzppcudhsrltvnxvu";
	double scjfaemgb = 627;
	double gueqsgcfc = 40053;
	double ucgqv = 10688;
	bool pinaotzc = false;
	if (string("ephthfnwmtpyrbdsaihsillsxubwrgfdjxdoijcrvginuznkymrfqrdbzaykstffkztwo") == string("ephthfnwmtpyrbdsaihsillsxubwrgfdjxdoijcrvginuznkymrfqrdbzaykstffkztwo")) {
		int iwaqb;
		for (iwaqb = 68; iwaqb > 0; iwaqb--) {
			continue;
		}
	}
	return false;
}

string mrgzhgu::ekrilobgbckcstlxjmt() {
	string ssqemlmbcsah = "cyeaqkhqnwwftzardgffpnfydylkwaoefjsr";
	double sbwjb = 5567;
	if (5567 == 5567) {
		int wip;
		for (wip = 69; wip > 0; wip--) {
			continue;
		}
	}
	return string("czil");
}

double mrgzhgu::ihyvzuzbfnttmhgws(int vdukl, double yasfspmo) {
	double hhjreewtxmwvvs = 1606;
	string owyklo = "rhaxafw";
	bool waptkvh = true;
	double oyyqtcvgifpitum = 509;
	int ltihmhxughjrc = 1236;
	double xcptxuobhhe = 11104;
	double xokjcturpwmkyx = 43718;
	int unudsn = 7250;
	bool immemrrhgbr = false;
	if (7250 != 7250) {
		int vze;
		for (vze = 57; vze > 0; vze--) {
			continue;
		}
	}
	if (string("rhaxafw") == string("rhaxafw")) {
		int obpyqtbn;
		for (obpyqtbn = 86; obpyqtbn > 0; obpyqtbn--) {
			continue;
		}
	}
	if (false == false) {
		int dbfhrhly;
		for (dbfhrhly = 84; dbfhrhly > 0; dbfhrhly--) {
			continue;
		}
	}
	if (11104 == 11104) {
		int duxvua;
		for (duxvua = 70; duxvua > 0; duxvua--) {
			continue;
		}
	}
	return 64654;
}

double mrgzhgu::wkbwqeybpsjjduialrzn(double femydjluwnji, bool ogekdxny, double jdhsniriob, double kvfgwokjxbas) {
	int ewbybczcjqfyh = 681;
	double wiyeqlvxrlj = 20163;
	bool eskatfbm = false;
	if (20163 != 20163) {
		int xjlciazh;
		for (xjlciazh = 56; xjlciazh > 0; xjlciazh--) {
			continue;
		}
	}
	if (681 == 681) {
		int drkkim;
		for (drkkim = 33; drkkim > 0; drkkim--) {
			continue;
		}
	}
	return 53043;
}

double mrgzhgu::ohygfvisdsraoxp(double aovgkrgpmfpghxp, int bkztjz, bool pyiotf, double egkdpnmblr, string hdeqplzyujzu, double seeskywev, double pkgaaatboetpgu, int wjujnbbwwbkbdna, bool cdukyujxgjt, bool dqhqay) {
	int iahnjdgsrlhzof = 900;
	double zvjvakzyvdcq = 16468;
	bool gerszsvbisarl = true;
	if (900 == 900) {
		int cku;
		for (cku = 20; cku > 0; cku--) {
			continue;
		}
	}
	if (900 == 900) {
		int bbdcuhltf;
		for (bbdcuhltf = 18; bbdcuhltf > 0; bbdcuhltf--) {
			continue;
		}
	}
	if (900 != 900) {
		int hsf;
		for (hsf = 59; hsf > 0; hsf--) {
			continue;
		}
	}
	if (16468 != 16468) {
		int duzrl;
		for (duzrl = 89; duzrl > 0; duzrl--) {
			continue;
		}
	}
	return 95915;
}

double mrgzhgu::vyutgwoigkeanwawrxq(string tzgmcxcysqw, string xslwrvbnwhqb, bool pqdhofgnhnnwac) {
	double vfbwieinvxrs = 23183;
	int mtoqpe = 2780;
	int lourmut = 2846;
	bool pmnyckstpdpp = true;
	string znoogmqes = "twyvihbnschtssafkfvlqjlcgufzhbbnqkkpzcquegsrsfdxxhxapr";
	double zsombyqxgsodiq = 37527;
	string vrvprrdfobipo = "koxbspa";
	if (2780 != 2780) {
		int jwhqkiuw;
		for (jwhqkiuw = 92; jwhqkiuw > 0; jwhqkiuw--) {
			continue;
		}
	}
	if (true == true) {
		int jcwilir;
		for (jcwilir = 7; jcwilir > 0; jcwilir--) {
			continue;
		}
	}
	if (true != true) {
		int awpzmol;
		for (awpzmol = 98; awpzmol > 0; awpzmol--) {
			continue;
		}
	}
	if (string("twyvihbnschtssafkfvlqjlcgufzhbbnqkkpzcquegsrsfdxxhxapr") == string("twyvihbnschtssafkfvlqjlcgufzhbbnqkkpzcquegsrsfdxxhxapr")) {
		int vws;
		for (vws = 15; vws > 0; vws--) {
			continue;
		}
	}
	if (2846 == 2846) {
		int maykfigutn;
		for (maykfigutn = 57; maykfigutn > 0; maykfigutn--) {
			continue;
		}
	}
	return 60662;
}

int mrgzhgu::woxevrfbjjcos(double unesfhkrwgipode, int hhgwu, double vdhiyxpzbmn, double dnqvwqzpy, string rucxldrk, string kjvixasmjtwa, string ofbwfwvsuxh, int uqjrxvhuurvnaoh, int qishtdboy, string uktvkjkmmzgan) {
	double oiyqulosakkzmdt = 23611;
	string mbzjjzxhy = "induqdwtqanolnrzrrngucwhipyxxmxdvvrblikpzzpwnxppvrfzoiuq";
	if (string("induqdwtqanolnrzrrngucwhipyxxmxdvvrblikpzzpwnxppvrfzoiuq") == string("induqdwtqanolnrzrrngucwhipyxxmxdvvrblikpzzpwnxppvrfzoiuq")) {
		int fodsnugbwq;
		for (fodsnugbwq = 13; fodsnugbwq > 0; fodsnugbwq--) {
			continue;
		}
	}
	if (string("induqdwtqanolnrzrrngucwhipyxxmxdvvrblikpzzpwnxppvrfzoiuq") != string("induqdwtqanolnrzrrngucwhipyxxmxdvvrblikpzzpwnxppvrfzoiuq")) {
		int geb;
		for (geb = 51; geb > 0; geb--) {
			continue;
		}
	}
	return 46767;
}

string mrgzhgu::plhdijjznwjvobojwtx(string bfaoxzjjjknt) {
	bool ltlolad = true;
	if (true != true) {
		int quttawgoj;
		for (quttawgoj = 98; quttawgoj > 0; quttawgoj--) {
			continue;
		}
	}
	return string("axyhndxevti");
}

bool mrgzhgu::ivkyaxipjaborsko(string zrpura, double tyaygc, double pnzulxrur, int qhaxtclwpex) {
	string pppgvniefautcb = "jfdbaybsvokrkneqsbr";
	if (string("jfdbaybsvokrkneqsbr") != string("jfdbaybsvokrkneqsbr")) {
		int ejqlehxn;
		for (ejqlehxn = 18; ejqlehxn > 0; ejqlehxn--) {
			continue;
		}
	}
	if (string("jfdbaybsvokrkneqsbr") == string("jfdbaybsvokrkneqsbr")) {
		int hvph;
		for (hvph = 51; hvph > 0; hvph--) {
			continue;
		}
	}
	if (string("jfdbaybsvokrkneqsbr") == string("jfdbaybsvokrkneqsbr")) {
		int pqokwfg;
		for (pqokwfg = 96; pqokwfg > 0; pqokwfg--) {
			continue;
		}
	}
	return false;
}

double mrgzhgu::chbwscceqxibyaskcag(double owldvkq, double fqikf, string cyhyctbvjixoovu) {
	string aofswctsysqh = "vrotkixhevfwmrnqumpbijlitnzwqskgbdtopmoukvphzilrtypun";
	string qsnbczk = "jjqznnfhcfxkflmqipxrbxlqtcnqgqdfqgnebulcciijuhudrxzdcndhdzhvpvuog";
	string sthivqff = "aumpyzkgkjlnalyqmvetnifeuzbgwh";
	string nktpapprsqs = "sudslhzfnj";
	string thjmndlfzafh = "tpqnhkphwbp";
	string zkcqaq = "fyggcfiqtkmkicvenbrqrforpplvimkumpqagvelkdfjifgyc";
	return 15969;
}

mrgzhgu::mrgzhgu() {
	this->vyutgwoigkeanwawrxq(string("vexavtlgywhbvfgatqmgboskwyfkdcsgbxawicimtcodvkfhahcymgtkavbhzoowolvzlpouglststtpyug"), string("xamjvekhnmabjpphkxlbldjaggzblxxiguyyhrbklqquskoerouacdancfmogwyggjbooqbtfj"), true);
	this->woxevrfbjjcos(60346, 1765, 938, 38221, string("cgadnbhgvfebvnwubqowmrjxhyalzlbfnlcuaqnqqkuuwcupn"), string("eqlcdkzlnrtyutjbxoruzikjjfzbinlpwbmuwhbmc"), string("djnnqwtrzzoadeqakpcxxvxyvrswrcerqvslzlnuvkfekyurmqflaukemrbocryfnhxqqdeickuwdsqpoqtkbvin"), 583, 5820, string("ezsxnyidrhkdiruejsfylllrflzawmhegwbydlteuxwlyqzcobuoecbhomuu"));
	this->plhdijjznwjvobojwtx(string("zooxoqhvnzw"));
	this->ivkyaxipjaborsko(string("s"), 12486, 78, 8772);
	this->chbwscceqxibyaskcag(61671, 4047, string("ruepwmcetqphgriombdnvwdxrmbnlnxlwujtyezszqgdctaxfczfnyynscrkrtzlxaaagbjhwcprlupxrwihmceljo"));
	this->qmzysljuyj(15750, 7534, false, false, string("cwrfwnntybnhxhrbbfzx"), false, true, string("ijlyifez"));
	this->pulibnakcpdcgetvkzbstdurp(false, true, 42786, 260, false, 20998, 146909);
	this->ekrilobgbckcstlxjmt();
	this->ihyvzuzbfnttmhgws(1050, 18373);
	this->wkbwqeybpsjjduialrzn(33428, false, 72384, 14278);
	this->ohygfvisdsraoxp(16905, 95, true, 4470, string("udifvblllszyhoubarzrwqggveokhzscirakmpqaoemmjhjvtqnvhxsnsjvjearymfhiirmpgqfzncxjsrlybkngsrpabl"), 21912, 9653, 783, false, false);
	this->zoybggxaqbkxpeisfn(2118, 21315, true, string("ypxnkdqvgmphymrpfqrbbyuyuqqgrqzyzxysodbnaqbqyymmrjwtkyteovbyaja"), 39756, string("fvpjlglqppiuzsjjcdwxhglegoepnvnwr"), 1108);
	this->zngdejeymxxkjslrbivlsy(1026, string("rnxqxlceeogyheunwzdamjzvta"), 6026, true, false, 5682);
	this->asrsgbrfio();
}
