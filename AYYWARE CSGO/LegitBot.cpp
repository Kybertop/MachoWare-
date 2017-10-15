#include "LegitBot.h"
#include "RenderManager.h"
#include "MathFunctions.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{

}

static int CustomDelay = 0;
static int CustomBreak = 0;

void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (!Menu::Window.LegitBotTab.Active.GetState())
		return;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	static int CustomAimTime = 0;
	static int CustomAimStart = 0;
	if (Menu::Window.LegitBotTab.AimbotEnable.GetState())
	{
		if (StartAim > 0)
		{
			if (CustomAimStart < (StartAim * 333))
			{
				CustomAimStart++;
			}
			else
			{
				if (Aimtime > 0)
				{
					if (CustomAimTime < (Aimtime * 333))
					{
						DoAimbot(pCmd);
						CustomAimTime++;
					}
					if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
					{
						CustomAimTime = 0;
						CustomAimStart = 0;
					}
				}
				else
				{
					DoAimbot(pCmd);
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}

			if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
			{
				CustomAimStart = 0;
				CustomAimTime = 0;
			}
		}
		else
		{
			if (Aimtime > 0)
			{
				if (CustomAimTime < (Aimtime * 333))
				{
					DoAimbot(pCmd);
					CustomAimTime++;
				}
				if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
				{
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}
			else
			{
				DoAimbot(pCmd);
				CustomAimTime = 0;
				CustomAimStart = 0;
			}
		}
	}
	if (Menu::Window.LegitBotTab.TriggerEnable.GetState() && ((Menu::Window.LegitBotTab.TriggerKeyPress.GetState() && GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey()) || !Menu::Window.LegitBotTab.TriggerKeyPress.GetState())))
	{
		DoTrigger(pCmd);
	}

	SyncWeaponSettings();
}
void CLegitBot::NOT ()
{
	float pJunkcode = 3949978570582;
	pJunkcode = 4958495136096;
	if (pJunkcode = 83947710205979)
		pJunkcode = 9183241984225;
	pJunkcode = 681734896515;
	pJunkcode = 46911156489911;
	if (pJunkcode = 2345490534);
	pJunkcode = 62304534538943;
	pJunkcode = 439453453453;
	if (pJunkcode = 17178672425)
		pJunkcode = 12894836525;
	pJunkcode = 17327835501;
	pJunkcode = 43578345329844;
	if (pJunkcode = 347385034);
	pJunkcode = 230478307830943;
	pJunkcode = 439057303770453;
	if (pJunkcode = 1157530753342425)
		pJunkcode = 11567830574033425;
	pJunkcode = 48567830752465;
	pJunkcode = 76787803781345868;
	if (pJunkcode = 78378624156323474);
	pJunkcode = 2766783516848;
	pJunkcode = 365418623235953;
	if (pJunkcode = 19046479644397);
	pJunkcode = 776589543516848;
	pJunkcode = 807845453;
	if (pJunkcode = 739684415474);
	pJunkcode = 55029416955413288;
	pJunkcode = 502416955413288;
}

void CLegitBot::SyncWeaponSettings()
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponPistRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponPistAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponPistStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponSnipSpeed.GetValue();
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponSnipRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponSnipAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponSnipStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMachinegun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMGSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMGFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMGRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMGHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMGAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMGStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsShotgun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponShotgunSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponShotgunFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponShotgunRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponShotgunHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponShotgunAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponShotgunStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMpSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMpFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMpRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMpHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMpAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMpStartAimtime.GetValue() / 10;
	}
	else
	{
		Speed = Menu::Window.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMainRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
		Aimtime = Menu::Window.LegitBotTab.WeaponMainAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMainStartAimtime.GetValue() / 10;
	}
}

void CLegitBot::DoAimbot(CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
		SyncWeaponSettings();

	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				if (pLocal->GetVelocity().Length() > 45.f);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	if (TargetID >= 0 && pTarget)
	{
		SyncWeaponSettings();

		if (Menu::Window.LegitBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			if (Menu::Window.LegitBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}
	}
}

bool CLegitBot::TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex() && pEntity->GetIndex() < 65)
	{
		if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
		{
			if (!pEntity->HasGunGameImmunity())
			{
				return true;
			}
		}
	}

	return false;
}

void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	auto LocalPlayerWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(LocalPlayer->GetWeaponHandle());
	auto WeaponEntity = (IClientEntity*)LocalPlayerWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetAmmoInClip() == 0)
			return;

		auto ClientClass = WeaponEntity->GetClientClass2();
		if (ClientClass->m_ClassID == (int)CSGOClassID::CKnife ||
			ClientClass->m_ClassID == (int)CSGOClassID::CHEGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CDecoyGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CIncendiaryGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CSmokeGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CC4) {
			return;
		}
	}
	else
		return;

	Vector ViewAngles = pCmd->viewangles;
	if (Menu::Window.LegitBotTab.TriggerRecoil.GetState())
		ViewAngles += LocalPlayer->GetAimPunch() * 2.0f;

	Vector CrosshairForward;
	AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pSkip = LocalPlayer;

	Ray.Init(TraceSource, TraceDestination);
	Interfaces::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
		return;
	if (Trace.m_pEnt->IsImmune())
		return;

	if (!Menu::Window.LegitBotTab.TriggerTeammates.GetState()) {
		if (LocalPlayer->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
			return;
	}

	if (Menu::Window.LegitBotTab.TriggerSmokeCheck.GetState())
	{
		typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

		static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

		if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), Trace.m_pEnt->GetBonePos(6)))
			return;
	}

	if ((Menu::Window.LegitBotTab.TriggerHead.GetState() && Trace.hitgroup == HITGROUP_HEAD) ||
		(Menu::Window.LegitBotTab.TriggerChest.GetState() && Trace.hitgroup == HITGROUP_CHEST) ||
		(Menu::Window.LegitBotTab.TriggerStomach.GetState() && Trace.hitgroup == HITGROUP_STOMACH) ||
		(Menu::Window.LegitBotTab.TriggerArms.GetState() && (Trace.hitgroup == HITGROUP_LEFTARM || Trace.hitgroup == HITGROUP_RIGHTARM)) ||
		(Menu::Window.LegitBotTab.TriggerLegs.GetState() && (Trace.hitgroup == HITGROUP_LEFTLEG || Trace.hitgroup == HITGROUP_RIGHTLEG))) {
		pCmd->buttons |= IN_ATTACK;
	}
}

bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				if (Menu::Window.LegitBotTab.AimbotSmokeCheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
						return false;
				}

				if (Multihitbox)
				{
					if (!pEntity->HasGunGameImmunity())
					{
						if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 8) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 1 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 2 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 3 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 5 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 7 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 9 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 10 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6))))))))))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}

int CLegitBot::GetTargetCrosshair()
{
	SyncWeaponSettings();
	int target = -1;


	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{
				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}

bool CLegitBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd)
{
	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl > 0)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * RecoilControl;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	GameUtils::NormaliseViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Normalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;
	}

	pCmd->viewangles += shit;
	Interfaces::Engine->SetViewAngles(pCmd->viewangles);

	return v;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ijftbnx {
public:
	string zkstzfqoagxdntv;
	string apvhcoqr;
	ijftbnx();
	bool vtjtxnuyeogxkl();
	int mgincszmxawxrbdstpqvgcy(bool cmfrrfjhqmkij);
	bool oskahzfsygh();
	string mbbydduswynagya(bool wnneeaqtjspaxk, double eacokwjiatmbr, double xdchmgv, bool qkkeicg, int gkzreckjq, int ijpux, string duiprw, string rxknemfwc);
	string mwcqyioxhwbsasw(double tavpklnacs, double dzeirfxo);
	string gmldmtnsvfsglzmlve(string cytfuy, string nkwcumc, int zoaicybvfftsm, double nfaeuoamlyqioq, int vtnrudutlh, double yvpfimoh, int ulijbeqy);
	bool faavccgoqht(string ovrxctynejl, string gemwiuva, bool jonpnzcyzcrwzz, int xwucmxljlmwzque);
	double gpwsxukmxmusz(bool gbovyhaqsdkpcey, bool nxymcmmpm, bool kxpqaxojvxxdnvc, bool oovhf, double uprpz, int pjlmbja);
	void bdcdaqjfmzcrut();
	bool qaqplbvdrbkmzj();

protected:
	int jczjntyd;
	double xiywxugxbbur;
	double xlllnwdrrtivow;
	bool rzbsmclotwzdstm;
	double rzwzy;

	string eyvhtqrsxn(bool xjgvoanwr, bool qkdgkbwhdvjokx, double pjewfptikpbk, int vmnhznkxwcmxrc);
	bool baexqlbhojjhvhw(double adfnlcol, bool thfcljciwqarn, bool chlagoxzizfuxjl, int pqyoh, int niqaudkjakcq);
	void ptialehrizpkykkqhb(bool tyzaei, string xslalamwndout, double qyoheopxzsjqwmz, bool izqri, int lqnuzdzcupkdj);
	double lbihdmnyguvchwtvjkyj(bool jjdesaimnxrfsik);
	string abxpolfwzlcskhcqaftkreb(string qrxdeoenkwmcn, bool jmpzg, double qxaponwwwqo, int daclfjc, bool rropgkadkdwjtr, string imwfqi, string dnmugo);
	string xqehewnqypgcvwgzjtd(int xsvdzxdpdbk);

private:
	string iqiid;

	bool rceeqnikinpxrz(string itfqn, bool qgntook, int hkipvzndlmlk, int uweqym, bool xlrdb, bool nureoclxkqou, int exqvms, string arzqndhssvgkr);
	void jyravcxzfhpstmkdptnhbesb(double gmzisvylsycs, int vqsagzxtari, int mrnwdaqo, int jifxhqvjqnbkthr, string alaunxwfxpch, string bqemshambx);
	bool ffsvzdieoewmlkdcrtxqhdff(int ulkdfsquzers, double qkdxuaisgdfl, double xzjntqkgiq, double hevknevp, int uajijhj, bool pdgwwtct, int jcbkmtiux, string qmboikcmffcfkac);
	void ppecxadmldxgarxodujifh(double knhiklllwuzhkbg, int kxsebwqeya);
	int pnsngqzqgblfkwhyjar(int nhgxxwkmxwq, bool elkkhnmx, double qxvsmzyl, double qfywxx, double setxzg, string ghuqqxyekcpnh);
	bool sgrzvxnbvpmqgcoekvldfyy();
	int chtdgtkfek(bool ifdwgeadsgnpni, bool yervntbm, bool fuanltprlfl, double iwdzhdwptq);
	int gmrfyyrnhsglow(bool gixsmaupivr, bool cwumd, bool qvtqaiguvxpcf, bool aubvlnsczk, double inhrkzq, double xpaga, double qrqyoqfzap, bool xlpnh, double abpbu, string vhofamakltumcj);
	void tdqxkrdpgavpu(bool ayajwoilvj, string xthifukgrmqwxcp, bool kvendmybrygc, double vksvrbjgk, bool ufprwqi, bool xcszeksf);
	bool gcemtxthqxmnifgkqiverew(double xvoubwjyauao, string eccluerhernrvo, int snloy, int hjpbzkecje);

};


bool ijftbnx::rceeqnikinpxrz(string itfqn, bool qgntook, int hkipvzndlmlk, int uweqym, bool xlrdb, bool nureoclxkqou, int exqvms, string arzqndhssvgkr) {
	bool wfdvftrki = false;
	string nvxiixsozoq = "rijpldjttxtenykjnmdnnztdbxdwjwuu";
	int fhkdsskk = 5285;
	if (5285 != 5285) {
		int ccuqgg;
		for (ccuqgg = 71; ccuqgg > 0; ccuqgg--) {
			continue;
		}
	}
	if (5285 != 5285) {
		int rqvaoinw;
		for (rqvaoinw = 28; rqvaoinw > 0; rqvaoinw--) {
			continue;
		}
	}
	return true;
}

void ijftbnx::jyravcxzfhpstmkdptnhbesb(double gmzisvylsycs, int vqsagzxtari, int mrnwdaqo, int jifxhqvjqnbkthr, string alaunxwfxpch, string bqemshambx) {
	string eervpcbwbghow = "hrzfiickfxectyznbgegiscrq";
	string fzsmbfaxfiqvhpm = "aygbugjjyymgvqcigqlnjbmvzluqvxpxdxqjawfxmjxgqfnhnq";
	int djrpelydyaii = 661;
	string cyyuicfsw = "wforfnodfamvzmrfbuqrotsnt";
	int zqzrdzpfyza = 5364;
	bool prqqvptyogga = false;
	bool jtfqfnjybhd = false;
	if (string("wforfnodfamvzmrfbuqrotsnt") != string("wforfnodfamvzmrfbuqrotsnt")) {
		int etbramyo;
		for (etbramyo = 19; etbramyo > 0; etbramyo--) {
			continue;
		}
	}
	if (false != false) {
		int qufk;
		for (qufk = 70; qufk > 0; qufk--) {
			continue;
		}
	}

}

bool ijftbnx::ffsvzdieoewmlkdcrtxqhdff(int ulkdfsquzers, double qkdxuaisgdfl, double xzjntqkgiq, double hevknevp, int uajijhj, bool pdgwwtct, int jcbkmtiux, string qmboikcmffcfkac) {
	string aedagchesudxwwn = "xmtdazdkyllkbzwskihrahlzauvzowhferepaufkxyxujkfewcbhokfwkbktprrsddqr";
	bool xftfsavqywkj = true;
	bool qkggnxmzb = false;
	double xaehuazcdtos = 58357;
	double ynfyecaxbp = 94470;
	string vftjyldavsxgmyi = "ocvncjgyjrwdorgdigukhrpgypjyhsybyxpxihchvwhhsperigqrgrqpgardcbczo";
	int usnjfjtva = 5357;
	string zluajuleowhns = "muyrbnxankdtrwrtfneyqoxgmphasufutsdvjlsnyt";
	bool jofwldkfxo = true;
	int bdqdebffljn = 114;
	if (string("ocvncjgyjrwdorgdigukhrpgypjyhsybyxpxihchvwhhsperigqrgrqpgardcbczo") != string("ocvncjgyjrwdorgdigukhrpgypjyhsybyxpxihchvwhhsperigqrgrqpgardcbczo")) {
		int tnryinb;
		for (tnryinb = 2; tnryinb > 0; tnryinb--) {
			continue;
		}
	}
	if (true == true) {
		int wbd;
		for (wbd = 62; wbd > 0; wbd--) {
			continue;
		}
	}
	return true;
}

void ijftbnx::ppecxadmldxgarxodujifh(double knhiklllwuzhkbg, int kxsebwqeya) {
	bool fgpyncoszgdpz = true;
	bool cjwtbphmwz = false;
	double zoagzkcpkf = 14770;
	string gshvcfhdd = "xszidvaowwhfyhctcpaotxdpxvbbskzmgwknzrpvm";
	bool gojlwqaqtsf = true;
	bool tqyeyilxjasf = true;
	bool vvhkqevaioo = false;
	bool fkvidgms = true;
	bool vocypnxdansxpj = false;
	if (14770 == 14770) {
		int htymttjy;
		for (htymttjy = 97; htymttjy > 0; htymttjy--) {
			continue;
		}
	}
	if (14770 == 14770) {
		int dtgfydy;
		for (dtgfydy = 84; dtgfydy > 0; dtgfydy--) {
			continue;
		}
	}

}

int ijftbnx::pnsngqzqgblfkwhyjar(int nhgxxwkmxwq, bool elkkhnmx, double qxvsmzyl, double qfywxx, double setxzg, string ghuqqxyekcpnh) {
	double xjkevpr = 9145;
	int ipfatd = 724;
	bool inteocmhzw = false;
	bool auhmxigphavwvpo = true;
	double xhawnsbprdem = 18333;
	double gnjuhbdssfdawi = 55455;
	bool kogmw = false;
	double gnikqdlpqtlysay = 5205;
	if (9145 != 9145) {
		int hjkoetrier;
		for (hjkoetrier = 14; hjkoetrier > 0; hjkoetrier--) {
			continue;
		}
	}
	if (18333 != 18333) {
		int oitsj;
		for (oitsj = 95; oitsj > 0; oitsj--) {
			continue;
		}
	}
	if (true != true) {
		int lezwevycvz;
		for (lezwevycvz = 4; lezwevycvz > 0; lezwevycvz--) {
			continue;
		}
	}
	if (false != false) {
		int ieuou;
		for (ieuou = 4; ieuou > 0; ieuou--) {
			continue;
		}
	}
	if (false == false) {
		int lheichovke;
		for (lheichovke = 28; lheichovke > 0; lheichovke--) {
			continue;
		}
	}
	return 59547;
}

bool ijftbnx::sgrzvxnbvpmqgcoekvldfyy() {
	double jzlbf = 33715;
	double sorjijqspqq = 68040;
	if (68040 != 68040) {
		int zloo;
		for (zloo = 78; zloo > 0; zloo--) {
			continue;
		}
	}
	return false;
}

int ijftbnx::chtdgtkfek(bool ifdwgeadsgnpni, bool yervntbm, bool fuanltprlfl, double iwdzhdwptq) {
	double wddzbaeqogegsjv = 16147;
	bool hiupxgdkewk = false;
	string zbpkhskwapvttum = "vgkozagodhhngjriywkhkalztkpblnzphwefwigbrhbnijoovsdlshqkyvahkwtxzulkrtxadfinbvuyszhrttupmlbegfdmro";
	int quddvqgs = 849;
	string khqycykrdo = "sfjseyxxluijiwgjijilpniwrjtmdoyzfuqgisscpemva";
	double blsupgcgdjnp = 45175;
	if (string("vgkozagodhhngjriywkhkalztkpblnzphwefwigbrhbnijoovsdlshqkyvahkwtxzulkrtxadfinbvuyszhrttupmlbegfdmro") == string("vgkozagodhhngjriywkhkalztkpblnzphwefwigbrhbnijoovsdlshqkyvahkwtxzulkrtxadfinbvuyszhrttupmlbegfdmro")) {
		int uvnpdzlvbx;
		for (uvnpdzlvbx = 20; uvnpdzlvbx > 0; uvnpdzlvbx--) {
			continue;
		}
	}
	return 10537;
}

int ijftbnx::gmrfyyrnhsglow(bool gixsmaupivr, bool cwumd, bool qvtqaiguvxpcf, bool aubvlnsczk, double inhrkzq, double xpaga, double qrqyoqfzap, bool xlpnh, double abpbu, string vhofamakltumcj) {
	string apdlevysav = "ytxkqtjqvbcumenrzahcjzfhtmnrhubqjyuuxmavqlwynykgfdxkmigg";
	return 41647;
}

void ijftbnx::tdqxkrdpgavpu(bool ayajwoilvj, string xthifukgrmqwxcp, bool kvendmybrygc, double vksvrbjgk, bool ufprwqi, bool xcszeksf) {
	double hpwgbps = 4178;
	double sggcofnkedmi = 12010;
	double haeafnxx = 6687;
	double yuaecuqk = 2255;
	if (4178 != 4178) {
		int ohakpwaujn;
		for (ohakpwaujn = 91; ohakpwaujn > 0; ohakpwaujn--) {
			continue;
		}
	}
	if (4178 == 4178) {
		int hvowago;
		for (hvowago = 60; hvowago > 0; hvowago--) {
			continue;
		}
	}

}

bool ijftbnx::gcemtxthqxmnifgkqiverew(double xvoubwjyauao, string eccluerhernrvo, int snloy, int hjpbzkecje) {
	int xlymhrx = 2210;
	int lyexlhquxw = 229;
	return true;
}

string ijftbnx::eyvhtqrsxn(bool xjgvoanwr, bool qkdgkbwhdvjokx, double pjewfptikpbk, int vmnhznkxwcmxrc) {
	bool ynputdhflafr = false;
	int uhysxtwzfrqlumr = 500;
	double dgfrlnveohpdwv = 81418;
	string zvtuxhxch = "qbqzciofkalbkklhzoieixggvzecjho";
	int gqsdoidqiwfpqss = 1551;
	bool mzqmhrxqoeomex = false;
	string ohxctaurheavzud = "dovwaygnyeuyvdcepwubowkvpklxojvdnqpidxtgginarsbnhszosbwpuvkepxqijxwcsvjufnasvgobgusesblqvpnav";
	if (false != false) {
		int yleoujo;
		for (yleoujo = 26; yleoujo > 0; yleoujo--) {
			continue;
		}
	}
	if (false != false) {
		int pqbdzhhljb;
		for (pqbdzhhljb = 13; pqbdzhhljb > 0; pqbdzhhljb--) {
			continue;
		}
	}
	if (string("qbqzciofkalbkklhzoieixggvzecjho") != string("qbqzciofkalbkklhzoieixggvzecjho")) {
		int lhrdzem;
		for (lhrdzem = 14; lhrdzem > 0; lhrdzem--) {
			continue;
		}
	}
	if (string("dovwaygnyeuyvdcepwubowkvpklxojvdnqpidxtgginarsbnhszosbwpuvkepxqijxwcsvjufnasvgobgusesblqvpnav") == string("dovwaygnyeuyvdcepwubowkvpklxojvdnqpidxtgginarsbnhszosbwpuvkepxqijxwcsvjufnasvgobgusesblqvpnav")) {
		int blxa;
		for (blxa = 95; blxa > 0; blxa--) {
			continue;
		}
	}
	if (false != false) {
		int ocva;
		for (ocva = 57; ocva > 0; ocva--) {
			continue;
		}
	}
	return string("mzpz");
}

bool ijftbnx::baexqlbhojjhvhw(double adfnlcol, bool thfcljciwqarn, bool chlagoxzizfuxjl, int pqyoh, int niqaudkjakcq) {
	bool ygzmwskfec = false;
	double ogtacxsr = 12371;
	double wwityu = 50947;
	bool hmamt = false;
	bool hbxyfh = false;
	bool dkhclrblkqtudsy = true;
	bool epxelombzjlzgk = false;
	if (50947 != 50947) {
		int botjwibqci;
		for (botjwibqci = 19; botjwibqci > 0; botjwibqci--) {
			continue;
		}
	}
	if (false != false) {
		int badnra;
		for (badnra = 41; badnra > 0; badnra--) {
			continue;
		}
	}
	if (false == false) {
		int mebij;
		for (mebij = 4; mebij > 0; mebij--) {
			continue;
		}
	}
	if (false != false) {
		int hukhb;
		for (hukhb = 73; hukhb > 0; hukhb--) {
			continue;
		}
	}
	return false;
}

void ijftbnx::ptialehrizpkykkqhb(bool tyzaei, string xslalamwndout, double qyoheopxzsjqwmz, bool izqri, int lqnuzdzcupkdj) {
	bool risqa = true;
	int qqhklncwlfas = 3746;
	int ehdseaxg = 4223;
	int jwlcbapzsvm = 718;
	string zionuyfyskdpvna = "ckbaadrphgwvzicawrpixhvfwqrkxeiubwjovmkmhqgejjjgtpagvrxwrbvguwptdvyylbcbicxfmjybrwomcwazjhpuk";
	int chrqkttb = 176;
	if (true != true) {
		int qfrvymoy;
		for (qfrvymoy = 8; qfrvymoy > 0; qfrvymoy--) {
			continue;
		}
	}

}

double ijftbnx::lbihdmnyguvchwtvjkyj(bool jjdesaimnxrfsik) {
	int ooybjt = 885;
	int tohtvefwzskh = 1978;
	bool gvtemirtjja = false;
	double otzeiwotvgv = 19724;
	bool gfegzywqtda = true;
	return 75713;
}

string ijftbnx::abxpolfwzlcskhcqaftkreb(string qrxdeoenkwmcn, bool jmpzg, double qxaponwwwqo, int daclfjc, bool rropgkadkdwjtr, string imwfqi, string dnmugo) {
	double oirtjfiasyopl = 52563;
	int okyvnhwsqejwb = 1247;
	if (1247 == 1247) {
		int szxxuca;
		for (szxxuca = 15; szxxuca > 0; szxxuca--) {
			continue;
		}
	}
	if (52563 == 52563) {
		int oyoahozcco;
		for (oyoahozcco = 99; oyoahozcco > 0; oyoahozcco--) {
			continue;
		}
	}
	return string("brkmvsbhpncz");
}

string ijftbnx::xqehewnqypgcvwgzjtd(int xsvdzxdpdbk) {
	return string("cmzqtz");
}

bool ijftbnx::vtjtxnuyeogxkl() {
	bool loxaftinpml = true;
	string oitffqmlrhk = "bjyryympsvsrtorbjdjktxbylscvphevuxkwnbflzaoomxngujblehxdwid";
	bool jzzheg = false;
	if (string("bjyryympsvsrtorbjdjktxbylscvphevuxkwnbflzaoomxngujblehxdwid") == string("bjyryympsvsrtorbjdjktxbylscvphevuxkwnbflzaoomxngujblehxdwid")) {
		int xmwtyqmbs;
		for (xmwtyqmbs = 72; xmwtyqmbs > 0; xmwtyqmbs--) {
			continue;
		}
	}
	if (false != false) {
		int vjvsvzhg;
		for (vjvsvzhg = 9; vjvsvzhg > 0; vjvsvzhg--) {
			continue;
		}
	}
	if (true != true) {
		int ikhqvlhoi;
		for (ikhqvlhoi = 64; ikhqvlhoi > 0; ikhqvlhoi--) {
			continue;
		}
	}
	if (true == true) {
		int efttglngi;
		for (efttglngi = 21; efttglngi > 0; efttglngi--) {
			continue;
		}
	}
	return false;
}

int ijftbnx::mgincszmxawxrbdstpqvgcy(bool cmfrrfjhqmkij) {
	string lkown = "zrlccydzxajezeucnroadbmubrbyhyoxawajioztzdzxl";
	int asrndlflsqauf = 4218;
	string nvhpnmfnj = "ozcgmwbdexmljtfdeburyaozkjrqvytvauwbddvsijbvmpcizpuznepvcdntvcgullnpskxxlumdtbxew";
	int vvjbm = 5029;
	int eevazyld = 3123;
	int nskzesmdvsvhkm = 4210;
	bool qbphef = false;
	double nzppk = 12939;
	return 82224;
}

bool ijftbnx::oskahzfsygh() {
	string tllebk = "owubjqfblftiywrvhidtvqcyafximlbjjhsxzqbbhppssjmktyghhpfmlltdmejrnbafrsdqjqwzvobdtkexgjfzwmb";
	int jgtynjhgbrbbcg = 427;
	bool upcwcadfsqfcqq = true;
	double drfchmq = 11118;
	double zmcnnkfjpmp = 3651;
	string aqnxbrgjjwqae = "mbkczmymbvmrvnommnpmcgdzhzbxymz";
	int uosfgjitfmw = 1999;
	string sdqdbrwpkgcefyr = "gtzhjiewnnotcnhqchylrstggsoale";
	double wcpjrl = 48928;
	int ordlg = 1979;
	if (true != true) {
		int mnmrpgmg;
		for (mnmrpgmg = 26; mnmrpgmg > 0; mnmrpgmg--) {
			continue;
		}
	}
	if (1979 == 1979) {
		int msldg;
		for (msldg = 47; msldg > 0; msldg--) {
			continue;
		}
	}
	return true;
}

string ijftbnx::mbbydduswynagya(bool wnneeaqtjspaxk, double eacokwjiatmbr, double xdchmgv, bool qkkeicg, int gkzreckjq, int ijpux, string duiprw, string rxknemfwc) {
	int jtlixmvcbemsra = 5592;
	if (5592 == 5592) {
		int vh;
		for (vh = 56; vh > 0; vh--) {
			continue;
		}
	}
	if (5592 != 5592) {
		int syraohdr;
		for (syraohdr = 27; syraohdr > 0; syraohdr--) {
			continue;
		}
	}
	return string("hgtzhtsrct");
}

string ijftbnx::mwcqyioxhwbsasw(double tavpklnacs, double dzeirfxo) {
	int pvqawes = 2307;
	int qopfzjol = 5448;
	string ytnjnivzvfnwx = "dhekisbqoznmknwndxyhscexmrruecvejqycfsdkxqc";
	int ufyveeakzlxlmnr = 5541;
	string cougcpocpz = "fqvyvagbaecytmyaqvspkmnqquxvymtfdleijiikemm";
	string jorhqnozypzb = "gejorfywcypcoagxzxounbzrgeuqvqnkvdzotd";
	bool qrvqjwarozjjmq = true;
	int yxrfavdyfq = 1136;
	string lmahz = "duzmktmbfhcbxotzwsbnkntvaduozotjofxoeejrjkznizewux";
	int hjgbpyjcsjfsvxe = 927;
	if (1136 == 1136) {
		int bcfjznso;
		for (bcfjznso = 38; bcfjznso > 0; bcfjznso--) {
			continue;
		}
	}
	if (5541 == 5541) {
		int ouwjqrlgk;
		for (ouwjqrlgk = 8; ouwjqrlgk > 0; ouwjqrlgk--) {
			continue;
		}
	}
	return string("gxeupod");
}

string ijftbnx::gmldmtnsvfsglzmlve(string cytfuy, string nkwcumc, int zoaicybvfftsm, double nfaeuoamlyqioq, int vtnrudutlh, double yvpfimoh, int ulijbeqy) {
	string keyvabjj = "uvjnnhpaynnvnypcnqexzqxpcqlnzpupdnsbhfbzspvlvnnwuuevhdhfgxztucwpkgpdjkrwslojgmfbhkayabdc";
	int cdqevkoen = 3131;
	string mfmdg = "lhncttihcvrjlcbmmgpwoakyf";
	int kcrdqjj = 2737;
	double viksl = 6220;
	if (3131 == 3131) {
		int si;
		for (si = 0; si > 0; si--) {
			continue;
		}
	}
	if (2737 == 2737) {
		int pwa;
		for (pwa = 82; pwa > 0; pwa--) {
			continue;
		}
	}
	if (2737 == 2737) {
		int wlcgaipdvj;
		for (wlcgaipdvj = 68; wlcgaipdvj > 0; wlcgaipdvj--) {
			continue;
		}
	}
	if (2737 == 2737) {
		int eceeigfx;
		for (eceeigfx = 12; eceeigfx > 0; eceeigfx--) {
			continue;
		}
	}
	return string("ae");
}

bool ijftbnx::faavccgoqht(string ovrxctynejl, string gemwiuva, bool jonpnzcyzcrwzz, int xwucmxljlmwzque) {
	int zikoszzbqlhenus = 5217;
	string ldkajcv = "kwxqaycuqealolqocfweykgrdivcgslwpayygifsfcmfernplzqgxgmekctm";
	double nshlqaqrmwiic = 20888;
	int tfsfjnbxcdotxuv = 6830;
	bool dadbmuo = true;
	double mvanaptxgtz = 3888;
	string secqrdfjoq = "tzgfohzrgoqbsnlkdshviiukxvwumuiozsuyhdkakhicqyndc";
	int zlmfrlh = 1254;
	bool xamlk = false;
	if (string("kwxqaycuqealolqocfweykgrdivcgslwpayygifsfcmfernplzqgxgmekctm") == string("kwxqaycuqealolqocfweykgrdivcgslwpayygifsfcmfernplzqgxgmekctm")) {
		int ynwnhiosxq;
		for (ynwnhiosxq = 79; ynwnhiosxq > 0; ynwnhiosxq--) {
			continue;
		}
	}
	if (5217 == 5217) {
		int lb;
		for (lb = 46; lb > 0; lb--) {
			continue;
		}
	}
	if (string("kwxqaycuqealolqocfweykgrdivcgslwpayygifsfcmfernplzqgxgmekctm") == string("kwxqaycuqealolqocfweykgrdivcgslwpayygifsfcmfernplzqgxgmekctm")) {
		int ja;
		for (ja = 92; ja > 0; ja--) {
			continue;
		}
	}
	if (string("tzgfohzrgoqbsnlkdshviiukxvwumuiozsuyhdkakhicqyndc") != string("tzgfohzrgoqbsnlkdshviiukxvwumuiozsuyhdkakhicqyndc")) {
		int cnb;
		for (cnb = 3; cnb > 0; cnb--) {
			continue;
		}
	}
	return false;
}

double ijftbnx::gpwsxukmxmusz(bool gbovyhaqsdkpcey, bool nxymcmmpm, bool kxpqaxojvxxdnvc, bool oovhf, double uprpz, int pjlmbja) {
	double ypytzjzgcqinucj = 37217;
	int vkggafkf = 594;
	double gvscdciuu = 95001;
	int lbcothxhsxfwwd = 578;
	int nfsnum = 199;
	bool buqcgjjq = true;
	bool neigoscpb = false;
	string xdkuittr = "jegxtnumkpfuvqqrheintqrkrytkaqdkvmbqhdaqrjcvajpldbjodyhvpwwqvzsgen";
	if (199 != 199) {
		int cdbhslyc;
		for (cdbhslyc = 19; cdbhslyc > 0; cdbhslyc--) {
			continue;
		}
	}
	if (false == false) {
		int vrewmhd;
		for (vrewmhd = 0; vrewmhd > 0; vrewmhd--) {
			continue;
		}
	}
	if (37217 == 37217) {
		int zckmeeuojv;
		for (zckmeeuojv = 94; zckmeeuojv > 0; zckmeeuojv--) {
			continue;
		}
	}
	return 16364;
}

void ijftbnx::bdcdaqjfmzcrut() {
	bool ekbcydfackzqpxn = true;
	if (true == true) {
		int ydh;
		for (ydh = 76; ydh > 0; ydh--) {
			continue;
		}
	}

}

bool ijftbnx::qaqplbvdrbkmzj() {
	int ncnkc = 3017;
	if (3017 != 3017) {
		int mfjfd;
		for (mfjfd = 33; mfjfd > 0; mfjfd--) {
			continue;
		}
	}
	if (3017 == 3017) {
		int zddakxu;
		for (zddakxu = 29; zddakxu > 0; zddakxu--) {
			continue;
		}
	}
	if (3017 != 3017) {
		int klchqxw;
		for (klchqxw = 87; klchqxw > 0; klchqxw--) {
			continue;
		}
	}
	return true;
}

ijftbnx::ijftbnx() {
	this->vtjtxnuyeogxkl();
	this->mgincszmxawxrbdstpqvgcy(true);
	this->oskahzfsygh();
	this->mbbydduswynagya(false, 5296, 15584, true, 7685, 843, string("iojtkmmqyztxtyuchgyppxrdqlzbmrsvtlytodq"), string("herraughighhqbqfcrobhkuxicxesofflrrgbrisibpukfmpmhpqomzprbzivltaflnpdvrnqzuciur"));
	this->mwcqyioxhwbsasw(15069, 9298);
	this->gmldmtnsvfsglzmlve(string("wmpbewqihjqzryyudj"), string("dtc"), 989, 46198, 3784, 47979, 1713);
	this->faavccgoqht(string("verqbddupyxlpnpglpiienmyfgoktfrrritqpmutdjlclblkyznwcyezilxqppacsbimgnz"), string("vapirqrrenjuagumnscadmjfyrqobullaeewqjhxpsipwrgfvt"), false, 4994);
	this->gpwsxukmxmusz(true, true, true, true, 15393, 2631);
	this->bdcdaqjfmzcrut();
	this->qaqplbvdrbkmzj();
	this->eyvhtqrsxn(true, true, 34102, 909);
	this->baexqlbhojjhvhw(73707, true, true, 1322, 5832);
	this->ptialehrizpkykkqhb(false, string("snwhyjgdtcgvurarmuptcuyqoavy"), 1077, false, 165);
	this->lbihdmnyguvchwtvjkyj(true);
	this->abxpolfwzlcskhcqaftkreb(string("llxbjwfmpoqpaimexhvlsezmdknzjoutheaykjmzlmedywezldzziojnqehadsbcscbfp"), false, 17088, 324, true, string("zkivhrgecakrylgxmgwohiqflskiyeqzyrusicukwzrshslzo"), string("adnhsibwbedllibycpkxdstatl"));
	this->xqehewnqypgcvwgzjtd(660);
	this->rceeqnikinpxrz(string("daryokpofjcdfhjysenqdjzfnhmrsvvenfbzsmopheyr"), false, 1003, 5854, false, false, 6395, string("zjpzygribhdfhfkwniginmbychmsmdhuncdposamwybzstfwceutbfnwuwbrjqpite"));
	this->jyravcxzfhpstmkdptnhbesb(22491, 7922, 2302, 2334, string("y"), string("yhitqqfbekgyequpbkprsfqgqgfojhmkncfrmrfnsaeuxblqoptvtackobmjvlfscpiofzmrphjfnbigzykd"));
	this->ffsvzdieoewmlkdcrtxqhdff(1428, 13849, 54589, 10575, 777, true, 1755, string("ockzzaauovxacqmlqebwyhwtbyvyxweamkufvlfamqtyxvnkfrciasejirfomfpxkcppejubcuuhrwemtkjlplikoha"));
	this->ppecxadmldxgarxodujifh(5453, 3564);
	this->pnsngqzqgblfkwhyjar(2359, false, 9910, 75647, 27987, string("hvquwxnoxesyklyxcfrvjuampdgguvcclwcqtyoezvcdpdmwxmyndotfjaxvxbjqz"));
	this->sgrzvxnbvpmqgcoekvldfyy();
	this->chtdgtkfek(true, false, false, 40648);
	this->gmrfyyrnhsglow(true, true, false, true, 3078, 48261, 48054, false, 35272, string("jjwqgqjlsngcunntchxgkiosevutigiuesjvmtzmoaonlwrzmsbdsumnbjcmcvvkgochedgzue"));
	this->tdqxkrdpgavpu(true, string("okvex"), false, 550, true, false);
	this->gcemtxthqxmnifgkqiverew(34753, string("xjwmkvdfhmquljnnirnfbwjjpckqiosnftxkpycosfhnwjitvgubfmnqvanntfuwvwydwtnejwumcnlsjdusveelifgkvp"), 7973, 6365);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oghsyrt {
public:
	double czntd;
	string jehkb;
	string taayavsqienzygv;
	oghsyrt();
	void peixieelfjpufvadr(int ckydpzgmihsnoo);
	string eskgeukrijkerncjiienhzp(bool qvzbkvgycbjanq, string jfigwwwibyvkqh, string ntoklqcujkfch, bool abcwghbzvqdl, int siolwbjt, int burxcxxv, string rdstysxl, bool oefkml, double fcjofkmtltp, bool scybalogdhma);
	string dkkyuzhikrnlvydmabow(string nnnqdfrdch, string sreqkbpmbg, bool mwevzc, bool uhavbndtipaubsb, bool hxgqr, bool iuueympc, bool erwly, bool ktdatttlaqoysd, int ihzzjghgp, bool fccoqelnmclensw);
	string objwvhggknrcfvg(string mfdofihxkxzdl, int iezlrkd, double ylxsltfac, string ikool, bool hdkadhcub, string njisvea);
	string ybwsormohdsztjdd(bool rwdjileuyrmxpd, int oyrbszqwrrpcqwu, int wfaujyqfp, string uaobxiidiciryj, string qcpwk, string hvmgsf, int zyaqvshqyljvbo, int uygahf, double iyqav, double dinnuypbqew);

protected:
	bool lirzgzjsnryzz;
	double qeynlfve;
	bool dlwpouqil;

	string frxxfnrqafiryj(string uasino, int ygmbtjkftvpur, int urwsem, int yuuxjnvbt);
	int heoxasgxeawitywfoaswnegel(double onxfuleyifbpo, string tpoemldy, bool gdewrwxuoexfz, int cclocfkumwag, int xkywtkkbsumpuum, string cgjxuryzvxmm, double kgeuclge);
	string ceorslsoiocmz(string lerppgjpbdqooe, bool ntetg);

private:
	string fbrxmtkxlzd;
	string qogejuuy;
	bool fveqtbzde;
	string akjdctrpigs;
	double obwifedgmksd;

	string whossgmlkcyvntmjdqtyrxz(bool jofkcji, double dfqdkrzzr);
	int mnrqoqvnzrjwzsc(bool svgykqydfeqk, string cthjtaehu, double ecgekhrshwavclh, int dypyslanhdzfv, string oewapzqwvvef, bool morskc, bool shkqfneb, string bcxcbezjoifgblp, string izqinkoy, bool rgzpaomxgwhwjz);
	string denkawuufizcjmtghs(int oywvjbdwudiuuq, bool yaohih, string dninritlhyrxpmy, int lgruzxfklqfye, bool pbekhinrttvrgto, string wmspptdqnh, int tmdyfiormjlhets, bool xxsosbzpg, string vhutzptt);
	string txhrdobrsvwvrpv(string psqfkqcqmfahis, string qxdwjeewqsrgouf, string ahssriglfdb, bool vwabvkuhsawn, double mxkozcten, double cyglmxzunrc, string jpvvpt, bool smtrqth);
	double kgwdtxdcrddtyepjobkznax(bool uqgpc, int ngyyzutc, double vodhxqnbsyccku);
	int athjhibbyybpwfvqffwwus(bool nouyqoy, string hpffeivonrqcr, bool qppavzxqhur, int eptggi, string nwecjesiea, double jahuqnbh, string clmsrlsuzkjjw, bool uvwden, string topvhmxn);
	bool kikdvgathatwcbsem(string vlhjk, int dsxctieowkwe, string wbaarefccfyjj, double wsmcfegmk, bool sppyghijmg);
	bool ktpkwaniqpuryqdjso(double pitpnfhgwwbfzef, double wbttf, bool wsqltqctg, double bkbpgnlrgypseaq);

};


string oghsyrt::whossgmlkcyvntmjdqtyrxz(bool jofkcji, double dfqdkrzzr) {
	bool dfexpjndcvnyuoy = false;
	double ozjmbfdrfcc = 57252;
	double hhskgwlgpiizd = 31354;
	int zojwkjgjdoehp = 1041;
	int dpwnignlca = 860;
	double aqxsmpgsafvic = 77082;
	double vmpvollrhq = 37043;
	string eiauvvoogxg = "wezucvvawwksezjfivyuoyanctliiyuxggcitqnpeqwkqnvhlorutewsgxsdzuznwt";
	double kbdkjaoydv = 69323;
	if (37043 == 37043) {
		int misnc;
		for (misnc = 42; misnc > 0; misnc--) {
			continue;
		}
	}
	if (69323 == 69323) {
		int bfpbosrkl;
		for (bfpbosrkl = 37; bfpbosrkl > 0; bfpbosrkl--) {
			continue;
		}
	}
	if (69323 == 69323) {
		int ipfw;
		for (ipfw = 88; ipfw > 0; ipfw--) {
			continue;
		}
	}
	if (31354 != 31354) {
		int mvcz;
		for (mvcz = 56; mvcz > 0; mvcz--) {
			continue;
		}
	}
	return string("tlzb");
}

int oghsyrt::mnrqoqvnzrjwzsc(bool svgykqydfeqk, string cthjtaehu, double ecgekhrshwavclh, int dypyslanhdzfv, string oewapzqwvvef, bool morskc, bool shkqfneb, string bcxcbezjoifgblp, string izqinkoy, bool rgzpaomxgwhwjz) {
	return 46982;
}

string oghsyrt::denkawuufizcjmtghs(int oywvjbdwudiuuq, bool yaohih, string dninritlhyrxpmy, int lgruzxfklqfye, bool pbekhinrttvrgto, string wmspptdqnh, int tmdyfiormjlhets, bool xxsosbzpg, string vhutzptt) {
	bool cmlguj = false;
	bool wackldrfqit = true;
	bool pkqpjbth = true;
	double mwwnzpeisyvitro = 43712;
	bool qbrtnmwiwhygri = false;
	double tyfvarsrpuyy = 49968;
	if (false != false) {
		int aycs;
		for (aycs = 13; aycs > 0; aycs--) {
			continue;
		}
	}
	if (true == true) {
		int rpvkimnwwx;
		for (rpvkimnwwx = 87; rpvkimnwwx > 0; rpvkimnwwx--) {
			continue;
		}
	}
	if (true != true) {
		int czstna;
		for (czstna = 56; czstna > 0; czstna--) {
			continue;
		}
	}
	return string("benutzrqzfxpf");
}

string oghsyrt::txhrdobrsvwvrpv(string psqfkqcqmfahis, string qxdwjeewqsrgouf, string ahssriglfdb, bool vwabvkuhsawn, double mxkozcten, double cyglmxzunrc, string jpvvpt, bool smtrqth) {
	string iicxr = "bytvczlvrlsvurpuigzhjedykvesnpiront";
	bool hmrndwodoc = false;
	bool mnstebuiavnty = false;
	string qsvfme = "gvuszrpii";
	if (false == false) {
		int nucwio;
		for (nucwio = 12; nucwio > 0; nucwio--) {
			continue;
		}
	}
	if (string("gvuszrpii") == string("gvuszrpii")) {
		int qxuvf;
		for (qxuvf = 36; qxuvf > 0; qxuvf--) {
			continue;
		}
	}
	if (false != false) {
		int mzrwkzwp;
		for (mzrwkzwp = 52; mzrwkzwp > 0; mzrwkzwp--) {
			continue;
		}
	}
	if (false == false) {
		int ebapmbze;
		for (ebapmbze = 27; ebapmbze > 0; ebapmbze--) {
			continue;
		}
	}
	if (string("gvuszrpii") != string("gvuszrpii")) {
		int nfvn;
		for (nfvn = 56; nfvn > 0; nfvn--) {
			continue;
		}
	}
	return string("cyjiojtrqygeij");
}

double oghsyrt::kgwdtxdcrddtyepjobkznax(bool uqgpc, int ngyyzutc, double vodhxqnbsyccku) {
	double lxnjasuh = 31293;
	int ncfqfegico = 3171;
	string bbyjo = "usbrzxmtgkhxorzzthoakccssfuelwhhmjjksdjmfsobaffwtahbmnjcqlyceffjobdzmycmrs";
	string ptxugwozyglfwu = "hmgrbblwsielmolxatubjpsgianybkmolaznmncllsaaoshkcfb";
	int irexkmhji = 2659;
	int azvcsalbz = 6966;
	string fyaremqpq = "vzbwmzzdjpgesmjfcqdelkthjkpgnrop";
	bool ezdcaeruoyvyg = true;
	if (2659 != 2659) {
		int lelty;
		for (lelty = 62; lelty > 0; lelty--) {
			continue;
		}
	}
	if (string("usbrzxmtgkhxorzzthoakccssfuelwhhmjjksdjmfsobaffwtahbmnjcqlyceffjobdzmycmrs") == string("usbrzxmtgkhxorzzthoakccssfuelwhhmjjksdjmfsobaffwtahbmnjcqlyceffjobdzmycmrs")) {
		int vb;
		for (vb = 100; vb > 0; vb--) {
			continue;
		}
	}
	if (string("hmgrbblwsielmolxatubjpsgianybkmolaznmncllsaaoshkcfb") == string("hmgrbblwsielmolxatubjpsgianybkmolaznmncllsaaoshkcfb")) {
		int uqdpjyelwb;
		for (uqdpjyelwb = 60; uqdpjyelwb > 0; uqdpjyelwb--) {
			continue;
		}
	}
	if (6966 == 6966) {
		int ricc;
		for (ricc = 0; ricc > 0; ricc--) {
			continue;
		}
	}
	if (string("hmgrbblwsielmolxatubjpsgianybkmolaznmncllsaaoshkcfb") != string("hmgrbblwsielmolxatubjpsgianybkmolaznmncllsaaoshkcfb")) {
		int eseymxn;
		for (eseymxn = 36; eseymxn > 0; eseymxn--) {
			continue;
		}
	}
	return 80183;
}

int oghsyrt::athjhibbyybpwfvqffwwus(bool nouyqoy, string hpffeivonrqcr, bool qppavzxqhur, int eptggi, string nwecjesiea, double jahuqnbh, string clmsrlsuzkjjw, bool uvwden, string topvhmxn) {
	string trynfk = "bwawpxquynnigesou";
	string gslch = "alfqhvcimbuduchheyymtkcofjmrdcdnaixohtykhkuzqdaliuvxbgtveo";
	double ryucjgka = 42498;
	int nfhayle = 297;
	bool ixiifdponzz = false;
	if (297 != 297) {
		int vmcawbxyzc;
		for (vmcawbxyzc = 21; vmcawbxyzc > 0; vmcawbxyzc--) {
			continue;
		}
	}
	if (string("alfqhvcimbuduchheyymtkcofjmrdcdnaixohtykhkuzqdaliuvxbgtveo") == string("alfqhvcimbuduchheyymtkcofjmrdcdnaixohtykhkuzqdaliuvxbgtveo")) {
		int kdduiu;
		for (kdduiu = 51; kdduiu > 0; kdduiu--) {
			continue;
		}
	}
	return 68842;
}

bool oghsyrt::kikdvgathatwcbsem(string vlhjk, int dsxctieowkwe, string wbaarefccfyjj, double wsmcfegmk, bool sppyghijmg) {
	int urvkjplxfj = 2537;
	int jltip = 1129;
	if (1129 != 1129) {
		int roplk;
		for (roplk = 60; roplk > 0; roplk--) {
			continue;
		}
	}
	return true;
}

bool oghsyrt::ktpkwaniqpuryqdjso(double pitpnfhgwwbfzef, double wbttf, bool wsqltqctg, double bkbpgnlrgypseaq) {
	int cyryzeiadpvwh = 1544;
	int uigkeqpyt = 953;
	if (1544 == 1544) {
		int abcczbiw;
		for (abcczbiw = 90; abcczbiw > 0; abcczbiw--) {
			continue;
		}
	}
	return false;
}

string oghsyrt::frxxfnrqafiryj(string uasino, int ygmbtjkftvpur, int urwsem, int yuuxjnvbt) {
	int nfnkgd = 2926;
	bool omstsiidhbjiq = true;
	int lfcfaohd = 1949;
	return string("huzidhjhtdlgz");
}

int oghsyrt::heoxasgxeawitywfoaswnegel(double onxfuleyifbpo, string tpoemldy, bool gdewrwxuoexfz, int cclocfkumwag, int xkywtkkbsumpuum, string cgjxuryzvxmm, double kgeuclge) {
	return 90380;
}

string oghsyrt::ceorslsoiocmz(string lerppgjpbdqooe, bool ntetg) {
	int wdmbtjw = 6220;
	double hfxawtfaf = 33257;
	int xakqzyi = 704;
	bool gkfqbyjyua = false;
	int fhadezqx = 626;
	double tndtnqfvqmqzudv = 8080;
	string qwtsj = "bdtmkmlqqenkdxnolwzoflkjriauijzbvuusyojlbxocvjujvomvhwhdkymtxoucxrkrlhkoecgjzhuzz";
	bool ohscefznl = true;
	bool ebhrbbroigy = false;
	string widae = "omvfernhm";
	return string("mnzyxempjckmwzve");
}

void oghsyrt::peixieelfjpufvadr(int ckydpzgmihsnoo) {
	bool osvepgyynvbcua = true;
	int jnjlgw = 8629;
	double shnmqvpqgvpfc = 10644;
	bool ezvbcu = true;

}

string oghsyrt::eskgeukrijkerncjiienhzp(bool qvzbkvgycbjanq, string jfigwwwibyvkqh, string ntoklqcujkfch, bool abcwghbzvqdl, int siolwbjt, int burxcxxv, string rdstysxl, bool oefkml, double fcjofkmtltp, bool scybalogdhma) {
	double qvjyxltrl = 48473;
	string pmopzaut = "etxzvopektbffofmtyjsuyyyocljjujhavzejxdeinnbpeyleistsxkhfkhigr";
	int bmuyc = 7494;
	bool qdtkcwiyeomscd = false;
	string idcvzqwjbogzt = "idioelhevghmlieahct";
	double myrhnmpnxaubm = 27119;
	int pnezbsvix = 223;
	int upmdybgsblsbhtq = 1388;
	string juoyhw = "gxfdjokepchbmolsdnwuhpryhkmdfagjmzrjcauwfqcczqvozwagahseqspcokwkqzkizdpq";
	string werho = "wpwvedxjps";
	if (false != false) {
		int am;
		for (am = 78; am > 0; am--) {
			continue;
		}
	}
	if (1388 == 1388) {
		int ufmf;
		for (ufmf = 43; ufmf > 0; ufmf--) {
			continue;
		}
	}
	return string("odisggxrabzbizyc");
}

string oghsyrt::dkkyuzhikrnlvydmabow(string nnnqdfrdch, string sreqkbpmbg, bool mwevzc, bool uhavbndtipaubsb, bool hxgqr, bool iuueympc, bool erwly, bool ktdatttlaqoysd, int ihzzjghgp, bool fccoqelnmclensw) {
	bool iddpjo = false;
	string yqvpjxufs = "nkxhdqyesyezoxvvvokfhwbwmntqwwptoagklrbilnppfosrzirsxbmqiyeakzxwndblkgerfkyxzpowhwvlhvbgmzps";
	bool acnpnm = true;
	int nwlctvtfncab = 2988;
	if (false != false) {
		int kasadjtcl;
		for (kasadjtcl = 85; kasadjtcl > 0; kasadjtcl--) {
			continue;
		}
	}
	if (false != false) {
		int urzv;
		for (urzv = 60; urzv > 0; urzv--) {
			continue;
		}
	}
	if (2988 == 2988) {
		int uk;
		for (uk = 91; uk > 0; uk--) {
			continue;
		}
	}
	return string("arvzwrzjrznxfvjwkryv");
}

string oghsyrt::objwvhggknrcfvg(string mfdofihxkxzdl, int iezlrkd, double ylxsltfac, string ikool, bool hdkadhcub, string njisvea) {
	double ummutsnl = 37342;
	int kpbtjjsffxiw = 6056;
	bool ccweoumvkfayppi = false;
	bool vlgqxzjbukv = true;
	bool ncprp = false;
	int szgoyy = 2198;
	bool qgtmp = true;
	return string("pmunqbnjtpqd");
}

string oghsyrt::ybwsormohdsztjdd(bool rwdjileuyrmxpd, int oyrbszqwrrpcqwu, int wfaujyqfp, string uaobxiidiciryj, string qcpwk, string hvmgsf, int zyaqvshqyljvbo, int uygahf, double iyqav, double dinnuypbqew) {
	double aaeaaxdtc = 2122;
	string mdkzew = "amevrmjzgzyqzouqpnsjfedrpzueswyafcyzeksfncuvsbfwjbkfjrahkotwjqfhmivgrwlbsgtwl";
	bool uldzmotmsiwpchi = false;
	int szimfoevgg = 79;
	string tvdufp = "cankaxqfoizxlgpraodnxfbewvrnnggoqpwonfjbrmv";
	return string("cfpaujbklru");
}

oghsyrt::oghsyrt() {
	this->peixieelfjpufvadr(1018);
	this->eskgeukrijkerncjiienhzp(true, string("pnndsteahrfrvwyvkhepdmapcymwedaytatplpcy"), string("fzcapqz"), false, 1484, 942, string("bbifnwmfbbxhsznfzjlihblztlu"), true, 15125, false);
	this->dkkyuzhikrnlvydmabow(string("xpwymuiomglogdoijukhdfggcupwvqldixgazhqrsaomsywjngoxuuxzytzakplo"), string("gzqbdiyoztjaqfbvyapticawllbwzqwxyatnllppevluycyjoujubactiu"), false, true, false, true, false, true, 4125, false);
	this->objwvhggknrcfvg(string("gczegotvhj"), 2738, 45072, string("bgnyfuiyimhpsldsrluljvochysotphvmkrawypzwaumapnzbjawyylzeiblfhogjvoggxhjcdyxhhoqqjclxn"), false, string("h"));
	this->ybwsormohdsztjdd(true, 5567, 1331, string("vzey"), string("rjwwxvsjonnqfvectiunymt"), string("bfkughvogdasevjscfxkmxruoaywgkwpytfxvoljtcudthvlvykvantlncrebkayftplynxeukleqdmpqwtjsdmlcxfxtmur"), 3431, 603, 35354, 32296);
	this->frxxfnrqafiryj(string("hkokxrrxiwzsyfnjtayfazjkkdnneljk"), 3366, 2966, 4041);
	this->heoxasgxeawitywfoaswnegel(4433, string(""), true, 365, 5457, string("lltxqyaapsdfcmykgvbqpwomudjqcifcspqpsomfmihupbjjzrjway"), 29049);
	this->ceorslsoiocmz(string("czgiwqogpiugekqcdklurexfgvfkqsgylsmysdaudhiyfyhyfaeufucqzvvjiughpkeqwt"), true);
	this->whossgmlkcyvntmjdqtyrxz(false, 185);
	this->mnrqoqvnzrjwzsc(true, string("ypaqunadjoxyfrrocofmruxvtwuvabrvkkfebtysgoajsjzarsglkcnwkzesmpfmjprtl"), 37487, 1398, string("gdjqbdytekzlmmibflsauxxzwqfrtq"), false, false, string("srzrnsntskevshctapcvyrggrdusagzjqlcepstgyvapgugtpmoshcvpuos"), string("wxescpjxd"), true);
	this->denkawuufizcjmtghs(905, false, string("aqqhjkrzbrhlklndeqphmpzgckotovkugygvufhzbpuxuzkrxukntyqlmoexcahwcgznqosnogwiakqxk"), 3956, false, string("dhfeymg"), 311, true, string("peyampsmdsfdzdpxvoytzaixeudveftrveszhrwzwrbsun"));
	this->txhrdobrsvwvrpv(string("bwaizwxbeqxjxlcpqvnxepkvlyobsgufnbjlkkigushawqlkkhjzp"), string("wmqtpdrgiuaerafluzxlnwnnmdwkbsllwyavdwqcuzlizvfoevuewjpfiyskda"), string("dzsyhsdqnvkowwoexpmdlkwlbnzeslhdiptohbrstrroxpmkqovrywzzzpqblhrtxhjcfcg"), false, 40898, 10550, string("smehunjbpveuudfmvwrdujoqlrfgzjnpareyotibbtqigbhuwkqwa"), true);
	this->kgwdtxdcrddtyepjobkznax(true, 946, 30631);
	this->athjhibbyybpwfvqffwwus(false, string("irzlhedpcndzfwqpedlrxkritlfdxrjolrwihqvvtmt"), true, 2687, string("eqpemnjejoyihayge"), 48533, string("svshnqdghevrtuyqknpfatvxplzkhdvzibqwvudbtnbp"), true, string("ybwtbfehtlhjcjgmxxtl"));
	this->kikdvgathatwcbsem(string("eyvxsejhqracyyvrhuhbnnrhgjjkwwenlthigvjj"), 4569, string("vjdwdyxzwcypqgutsdybsczcgodlgpymppogcdiryqjoyqjfibuuiqdkocrovvcch"), 15138, false);
	this->ktpkwaniqpuryqdjso(54843, 90768, false, 18717);
}
