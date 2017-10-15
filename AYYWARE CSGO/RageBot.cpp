#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;

	}
	return hitchance;
}

bool CanOpenFire() 
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsBallisticWeapon(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = -1;
		}
	}

	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}

void CRageBot::JUNK ()
{
	float pJunkcode = 31257973;
	pJunkcode = 2146542;
	if (pJunkcode = 81718741005)
		pJunkcode = 9183241984225;
	pJunkcode = 681734896515;
	pJunkcode = 46911156489911;
	if (pJunkcode = 23492345490534);
	pJunkcode = 62304534538943;
	pJunkcode = 439453453453;
	if (pJunkcode = 1717863272425)
		pJunkcode = 1837836525;
	pJunkcode = 17327835501;
	pJunkcode = 43578345329844;
	if (pJunkcode = 347385075388490534);
	pJunkcode = 230478307830943;
	pJunkcode = 439057303770453;
	if (pJunkcode = 1157530753342425)
		pJunkcode = 11567830574033425;
	pJunkcode = 48567830752465;
	pJunkcode = 76787803781345868;
	if (pJunkcode = 78378624156323474);
	pJunkcode = 2766783516848;
	pJunkcode = 96458680453;
	if (pJunkcode = 9647934623474);
	pJunkcode = 776589543516848;
	pJunkcode = 807845453;
	if (pJunkcode = 739684415474);
	pJunkcode = 5142230483516848;
	pJunkcode = 87359307918;
}

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

void CRageBot::DoAimbot(CUserCmd *pCmd,bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View;
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	if (GameUtils::IsRevolver(pWeapon) && Menu::Window.RageBotTab.AutoRevolver.GetState())
	{
		static int delay = 0;
		delay++;

		if (delay <= 15)
			pCmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		case 3:
			TargetID = GetTargetThreat(pCmd);
			break;
		case 4:
			TargetID = GetTargetNextShot();
			break;
		}

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

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		int StopKey = Menu::Window.RageBotTab.AimbotStopKey.GetKey();
		if (StopKey >= 0 && GUI.GetKeyState(StopKey))
		{
			TargetID = -1;
			pTarget = nullptr;
			HitBox = -1;
			return;
		}

		float pointscale = Menu::Window.RageBotTab.TargetPointscale.GetValue() - 5.f; 

		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);

		if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) // Autoscope
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
					}
					else
					{
						return;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					pCmd->buttons |= IN_ATTACK;
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK)
			Globals::Shots += 1;
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
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

	FLOAT DotProduct = Forward.Dot(Delta);

	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetNextShot()
{
	int target = -1;
	int minfov = 361;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minfov && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minfov = fov;
					target = i;
				}
				else
					minfov = 361;
			}
		}
	}

	return target;
}

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	AngleVectors(viewAngle, &aim);
	AngleVectors(aimAngle, &ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

double inline __declspec (naked) __fastcall FASTSQRT(double n)
{
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

float VectorDistance(Vector v1, Vector v2)
{
	return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

int CRageBot::GetTargetThreat(CUserCmd* pCmd)
{
	auto iBestTarget = -1;
	float flDistance = 8192.f;

	IClientEntity* pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			auto vecHitbox = pEntity->GetBonePos(NewHitBox);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				QAngle TempTargetAbs;
				CalcAngle(pLocal->GetEyePosition(), vecHitbox, TempTargetAbs);
				float flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);
				float flTempDistance = VectorDistance(pLocal->GetOrigin(), pEntity->GetOrigin());
				if (flTempDistance < flDistance && flTempFOVs < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}
	}
	return iBestTarget;
}

int CRageBot::GetTargetHealth()
{
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

#pragma region GetHitboxesToScan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int huso = (pEntity->GetHealth());
	int health = Menu::Window.RageBotTab.BaimIfUnderXHealth.GetValue();
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = Menu::Window.RageBotTab.TargetMultipoint.GetState();
	int TargetHitbox = Menu::Window.RageBotTab.TargetHitbox.GetIndex();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (huso < health)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
	}
	else
	{
		if (HitScanMode == 0)
		{
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				break;
			}
		}

	else if (Menu::Window.RageBotTab.AWPAtBody.GetState() && GameUtils::AWP(pWeapon))
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
	}

	else if (Menu::Window.RageBotTab.PreferBodyAim.GetState())
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
	}
	else
	{
		switch (HitScanMode)
		{
		case 1:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		case 2:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		case 3:
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
			}
		}
	}
#pragma endregion Get the list of shit to scan

	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);
			if (CanHit(Point, &Damage))
			{
				c = Color(0, 255, 0, 255);
				if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue())
				{
					return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}

	return -1;
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->m_flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;

	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}


	IsLocked = true;

	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; 

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	return ReturnValue;
}

namespace AntiAims 
{

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up)
		{
			pCmd->viewangles.x = 45;
			up = !up;
		}
		else
		{
			pCmd->viewangles.x = 89;
			up = !up;
		}
	}

	void FakePitch(CUserCmd *pCmd, bool &bSendPacket)
	{	
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.x = 89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 51;
			ChokedPackets = -1;
		}
	}

	void StaticJitter(CUserCmd *pCmd)
	{
		static bool down = true;
		if (down)
		{
			pCmd->viewangles.x = 179.0f;
			down = !down;
		}
		else
		{
			pCmd->viewangles.x = 89.0f;
			down = !down;
		}
	}

	// Yaws

	void FastSpin(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 100;

		y2 += spinBotSpeedFast;

		if (y2 >= 179)
			y2 = -179;

		pCmd->viewangles.y = y2;
	}

	void FakeEdge(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		
		Vector vEyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		CTraceFilter filter;
		filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int y = 0; y < 360; y++)
		{
			Vector qTmp(10.0f, pCmd->viewangles.y, 0.0f);
			qTmp.y += y;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			GameUtils::NormaliseViewAngle(qTmp);

			Vector vForward;

			VectorAngles(qTmp, vForward);

			float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
			vForward *= fLength;

			trace_t tr;

			Vector vTraceEnd = vEyePos + vForward;

			Ray_t ray;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector angles;

				Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

				VectorAngles(vNegative, angles);

				GameUtils::NormaliseViewAngle(angles);

				qTmp.y = angles.y;

				GameUtils::NormaliseViewAngle(qTmp);

				trace_t trLeft, trRight;

				Vector vLeft, vRight;
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

				vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
				vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

				vTraceEnd = vEyePos + vLeft;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

				vTraceEnd = vEyePos + vRight;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

				if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
					qTmp.y -= 90.f;
				else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
					qTmp.y += 90.f;

				if (qTmp.y > 180.0)
					qTmp.y -= 360.0;
				else if (qTmp.y < -180.0)
					qTmp.y += 360.0;

				pCmd->viewangles.y = qTmp.y;

				int offset = Menu::Window.RageBotTab.AntiAimOffset.GetValue();

				static int ChokedPackets = -1;
				ChokedPackets++;
				if (ChokedPackets < 1)
				{
					bSendPacket = false; // +=180?
				}
				else
				{
					bSendPacket = true;
					pCmd->viewangles.y -= offset;
					ChokedPackets = -1;
				}
				return;
			}
		}
		pCmd->viewangles.y += 360.0f;
	}
	
	void BackJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -70 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void FakeSideways(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void FastSpint(CUserCmd *pCmd)
	{
		int r1 = rand() % 100;
		int r2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (r1 == 1) dir = !dir;

		if (dir)
			current_y += 15 + rand() % 10;
		else
			current_y -= 15 + rand() % 10;

		pCmd->viewangles.y = current_y;

		if (r1 == r2)
			pCmd->viewangles.y += r1;
	}

	void BackwardJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -70 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void Jitter(CUserCmd *pCmd)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y -= 90;
		}

		int re = rand() % 4 + 1;


		if (jitterangle <= 1)
		{
			if (re == 4)
				pCmd->viewangles.y += 180;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			if (re == 4)
				pCmd->viewangles.y -= 180;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}

	void FakeStatic(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void TJitter(CUserCmd *pCmd)
	{
		static bool Turbo = true;
		if (Turbo)
		{
			pCmd->viewangles.y -= 90;
			Turbo = !Turbo;
		}
		else
		{
			pCmd->viewangles.y += 90;
			Turbo = !Turbo;
		}
	}

	void TFake(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y = -90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 90;
			ChokedPackets = -1;
		}
	}

	void FakeJitter(CUserCmd* pCmd, bool &bSendPacket)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}


	void Up(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = -89.0f;
	}

	void Zero(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = 0.f;
	}

	void Static(CUserCmd *pCmd)
	{
		static bool aa1 = false;
		aa1 = !aa1;
		if (aa1)
		{
			static bool turbo = false;
			turbo = !turbo;
			if (turbo)
			{
				pCmd->viewangles.y -= 90;
			}
			else
			{
				pCmd->viewangles.y += 90;
			}
		}
		else
		{
			pCmd->viewangles.y -= 180;
		}
	}

	void fakelowerbody(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f;
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = true;
		}
	}

	void FakeSideLBY(CUserCmd *pCmd, bool &bSendPacket)
	{
		int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i;
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static bool isMoving;
		float PlayerIsMoving = abs(pLocal->GetVelocity().Length());
		if (PlayerIsMoving > 0.1) isMoving = true;
		else if (PlayerIsMoving <= 0.1) isMoving = false;

		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		static bool bFlipYaw;
		float flInterval = Interfaces::Globals->interval_per_tick;
		float flTickcount = pCmd->tick_count;
		float flTime = flInterval * flTickcount;
		if (std::fmod(flTime, 1) == 0.f)
			bFlipYaw = !bFlipYaw;

		if (PlayerIsMoving <= 0.1)
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += 180.f;
			}
			else
			{
				if (flip)
				{
					pCmd->viewangles.y += bFlipYaw ? 90.f : -90.f;

				}
				else
				{
					pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + bFlipYaw ? 90.f : -90.f;
				}
			}
		}
		else if (PlayerIsMoving > 0.1)
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += 180.f;
			}
			else
			{
				pCmd->viewangles.y += 90.f;
			}
		}
	}

	void LBYJitter(CUserCmd* cmd, bool& packet)
	{
		static bool ySwitch;
		static bool jbool;
		static bool jboolt;
		ySwitch = !ySwitch;
		jbool = !jbool;
		jboolt = !jbool;
		if (ySwitch)
		{
			if (jbool)
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 90.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 90.f;
					packet = false;
				}
			}
			else
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 125.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 125.f;
					packet = false;
				}
			}
		}
		else
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			packet = true;
		}
	}

	void LBYSpin(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int skeet = 179;
		int SpinSpeed = 100;
		static int ChokedPackets = -1;
		ChokedPackets++;
		skeet += SpinSpeed;

		if
			(pCmd->command_number % 9)
		{
			bSendPacket = true;
			if (skeet >= pLocal->GetLowerBodyYaw() + 180);
			skeet = pLocal->GetLowerBodyYaw() - 0;
			ChokedPackets = -1;
		}
		else if
			(pCmd->command_number % 9)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 179;
			ChokedPackets = -1;
		}
		pCmd->viewangles.y = skeet;
	}

	void SlowSpin(CUserCmd *pCmd)
	{
		int r1 = rand() % 100;
		int r2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (r1 == 1) dir = !dir;

		if (dir)
			current_y += 4 + rand() % 10;
		else
			current_y -= 4 + rand() % 10;

		pCmd->viewangles.y = current_y;

		if (r1 == r2)
			pCmd->viewangles.y += r1;

	}

	void EdgeDetect(CUserCmd* pCmd, bool &bSendPacket)
	{

		IClientEntity* pLocal = hackManager.pLocal();

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocal;

		bool bEdge = false;

		Vector angle;
		Vector eyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		for (float i = 0; i < 360; i++)
		{
			Vector vecDummy(10.f, pCmd->viewangles.y, 0.f);
			vecDummy.y += i;

			Vector forward = vecDummy.Forward();

			float flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			forward *= flLength;

			Ray_t ray;
			CGameTrace tr;

			ray.Init(eyePos, (eyePos + forward));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, tr, true);

			if (tr.fraction != 1.0f)
			{
				Vector negate = tr.plane.normal;
				negate *= -1;

				Vector vecAng = negate.Angle();

				vecDummy.y = vecAng.y;

				trace_t leftTrace, rightTrace;

				Vector left = (vecDummy + Vector(0, 45, 0)).Forward(); // or 45
				Vector right = (vecDummy - Vector(0, 45, 0)).Forward();

				left *= (flLength * cosf(rad(30)) * 2); 
				right *= (flLength * cosf(rad(30)) * 2); 

				ray.Init(eyePos, (eyePos + left));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, leftTrace, true);

				ray.Init(eyePos, (eyePos + right));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, rightTrace, true);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				{
					vecDummy.y -= 45; // left
				}
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				{
					vecDummy.y += 45; // right     
				}

				angle.y = vecDummy.y;
				angle.y += 360;
				bEdge = true;
			}
		}

		if (bEdge)
		{
			static bool turbo = true;

			if (Menu::Window.RageBotTab.AntiAimEdge.GetState())
			{
				pCmd->viewangles.y = angle.y;
			}
		}
	}
}

void CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
{
	float delta_view, first_function, second_function;

	if (old_angles.y < 0.f) first_function = 360.0f + old_angles.y;
	else first_function = old_angles.y;

	if (cmd->viewangles.y < 0.0f) second_function = 360.0f + cmd->viewangles.y;
	else second_function = cmd->viewangles.y;

	if (second_function < first_function) delta_view = abs(second_function - first_function);
	else delta_view = 360.0f - abs(first_function - second_function);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

float GetLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{
		return 0.0f;
	}
}
float GetOutgoingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}
float GetIncomingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else
	{
		return 0.0f;
	}
}

float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;
float CurrentVelocity(IClientEntity* LocalPlayer)
{
	int vel = LocalPlayer->GetVelocity().Length2D();
	return vel;
}
bool NextLBYUpdate()
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	float flServerTime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);


	if (OldLBY != LocalPlayer->GetLowerBodyYaw())
	{
		LBYBreakerTimer++;
		OldLBY = LocalPlayer->GetLowerBodyYaw();
		bSwitch = !bSwitch;
		LastLBYUpdateTime = flServerTime;
	}

	if (CurrentVelocity(LocalPlayer) > 0.5)
	{
		LastLBYUpdateTime = flServerTime;
		return false;
	}

	if ((LastLBYUpdateTime + 1 - (GetLatency() * 2) < flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (LastLBYUpdateTime + 1.1 - (GetLatency() * 2) < flServerTime)
		{
			LastLBYUpdateTime += 1.1;
		}
		return true;
	}
	return false;
}

void DoLBYBreak(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
}

void DoLBYBreakReal(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
}

void DoRealAA(CUserCmd* pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!Menu::Window.RageBotTab.AntiAimEnable.GetState())
		return;
	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		// Fast Spin
		AntiAims::FastSpint(pCmd);
		break;
	case 2:
		// Slow Spin
		AntiAims::SlowSpin(pCmd);
		break;
	case 3:
		AntiAims::Jitter(pCmd);
		break;
	case 4:
		// 180 Jitter
		AntiAims::BackJitter(pCmd);
		break;
	case 5:
		//backwards
		pCmd->viewangles.y -= 180;
		break;
	case 6:
		AntiAims::BackwardJitter(pCmd);
		break;
	case 7:
		//Sideways-switch
		if (switch2)
			pCmd->viewangles.y = 90;
		else
			pCmd->viewangles.y = -90;

		switch2 = !switch2;
		break;
	case 8:
		//Sideways
		pCmd->viewangles.y -= 90;
		break;
	case 9:
		pCmd->viewangles.y += 90;
		break;
	case 10:
		pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
		break;
	case 11:
		AntiAims::LBYJitter(pCmd, bSendPacket);
		break;
	case 12:
		AntiAims::FakeSideLBY(pCmd, bSendPacket);
		break;
	case 13:
		AntiAims::LBYSpin(pCmd, bSendPacket);
		break;
	case 14:
		DoLBYBreakReal(pCmd, pLocal, bSendPacket);
		break;
	}

	static auto RandomReal = false;
	if (RandomReal)
		pCmd->viewangles.y += Menu::Window.RageBotTab.AntiAimOffset.GetValue();
	else
		pCmd->viewangles.y -= Menu::Window.RageBotTab.AntiAimOffset.GetValue();
	RandomReal = !RandomReal;
}

void DoFakeAA(CUserCmd* pCmd, bool& bSendPacket, IClientEntity* pLocal)
{
	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!Menu::Window.RageBotTab.AntiAimEnable.GetState())
		return;
	switch (Menu::Window.RageBotTab.FakeYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		// Fast Spin 
		AntiAims::FastSpint(pCmd);
		break;
	case 2:
		// Slow Spin 
		AntiAims::SlowSpin(pCmd);
		break;
	case 3:
		AntiAims::Jitter(pCmd);
		break;
	case 4:
		// 180 Jitter 
		AntiAims::BackJitter(pCmd);
		break;
	case 5:
		//backwards
		pCmd->viewangles.y -= 180;
		break;
	case 6:
		AntiAims::BackwardJitter(pCmd);
		break;
	case 7:
		//Sideways-switch
		if (switch2)
			pCmd->viewangles.y = 90;
		else
			pCmd->viewangles.y = -90;

		switch2 = !switch2;
		break;
	case 8:
		pCmd->viewangles.y -= 90;
		break;
	case 9:
		pCmd->viewangles.y += 90;
		break;
	case 10:
		pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
		break;
	case 11:
		AntiAims::LBYJitter(pCmd, bSendPacket);
		break;
	case 12:
		AntiAims::FakeSideLBY(pCmd, bSendPacket);
		break;
	case 13:
		AntiAims::LBYSpin(pCmd, bSendPacket);
		break;
	case 14:
		DoLBYBreak(pCmd, pLocal, bSendPacket);
		break;
	}

	static auto RandomFake = false;
	if (RandomFake)
		pCmd->viewangles.y += Menu::Window.RageBotTab.AddFakeYaw.GetValue();
	else
		pCmd->viewangles.y -= Menu::Window.RageBotTab.AddFakeYaw.GetValue();
	RandomFake = !RandomFake;
}

void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (IsAimStepping || pCmd->buttons & IN_ATTACK)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(pWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}
	if (Menu::Window.RageBotTab.AntiAimTarget.GetState())
	{
		aimAtPlayer(pCmd);
	}
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		pCmd->viewangles.x = 45.f;
		break;
	case 2:
		AntiAims::JitterPitch(pCmd);
		break;
	case 3:
		pCmd->viewangles.x = 89.000000;
		break;
	case 4:
		AntiAims::Up(pCmd);
		break;
	case 5:
		AntiAims::Zero(pCmd);
		break;
	case 6:
		static auto Random = false;
		if (Random)
			pCmd->viewangles.x += Menu::Window.RageBotTab.CustomPitch.GetValue();
		else
			pCmd->viewangles.x -= Menu::Window.RageBotTab.CustomPitch.GetValue();
		Random = !Random;

	}

	if (Menu::Window.RageBotTab.LBY.GetState() && pLocal->GetVelocity().Length2D() == 0)
	{
#define RandomInt(min, max) (rand() % (max - min + 1) + min)
		static bool fakeantiaim;
		int rand2;
		{
			int var1;
			int var2;
			float var3;

			pCmd->viewangles.y += 179.9;
			var1 = rand() % 100;
			var2 = rand() % (10 - 6 + 1) + 10;
			var3 = var2 - (rand() % var2);
			if (var1 < 60 + (rand() % 14))
				pCmd->viewangles.y -= var3;
			else if (var1 < 100 + (rand() % 14))
				pCmd->viewangles.y += var3;
		}

		if (fakeantiaim)
		{
			rand2 = RandomInt(1, 100);

			if (rand2 < 2.0)
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 92.3 - 0 - 31.3;
			}

			else
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 91.7;
			}
			fakeantiaim = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y += 154.4;
			fakeantiaim = true;
		}
	}
	else
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = true;
			DoFakeAA(pCmd, bSendPacket, pLocal);
		}
		else
		{
			bSendPacket = false;
			DoRealAA(pCmd, pLocal, bSendPacket);
			ChokedPackets = -1;
		}

		if (flipAA)
		{
			pCmd->viewangles.y -= 25;
		}
	}
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class awvrlxr {
public:
	int thxjlxfsotignnv;
	bool eexfvhljec;
	int krmevvkfioyckt;
	string rzpoyemcluwnazs;
	awvrlxr();
	double agovdvotvpowoicme(string sakgdguhzdxhay, int ylfybg, double qmiqplewzvvg, string ybviiso, bool qkzdg, bool sagyongqwpzxpet, string iuzcvqwg, double gvqwvn, string dpywxnegrvjdj, string vmsxiazeegps);
	int fejwpnozydxwdz(double ujpdctl, double mjkfizgjwnggum, bool eiblysfkw, bool ejfybbzwphellcj, bool nxnwgfhcnig, bool sarvyyjejlwn);
	double oypdcczpgyldvjxi();
	double dvfdiebujnh(double qrywmwghlq, double pptuwihlnpgukmy, string fqedssf, double fmwfvx, bool gkduratabq);
	void gilxqlvfkj(double eyzjoamx, bool zsecbugomwc, string mchonxexouqk, int etchvkiptmfp, string zgoezvmknbzvqg, int rattgqp, double jdbkpxxjyi, bool lmjtl, double vctxt);
	void pjhfchxaymcrhjaahxssora(string akcyhthkbaf, double qlmiagsg, int xhychpnxjfbc, double sriyvefuehykkz, string plevluswii, bool drtpzmdukf, double kgiotwmqzbch, bool jhpeskcpvompul, bool amsgfjfetm);

protected:
	double etnavk;
	bool qgourkbhk;
	string yskyfg;

	int jjzkblymnzyptzwsj(string whcdpri, string qvnzana, string caokfsymyhagoo);
	void ftexjrpcepbskszilvvunod(string dfetnln, int blmfmvjtuqrraz, int pgvgpmq, bool gmfnmcimeis, string oudph, string tqqtsnhqzhikbio);
	void ijmflrqgnvrzxxxqqhjpq(int otubslvrekbvfkz);

private:
	string mixxpq;
	double lefmllkgdc;
	string tnxyyiyim;

	double rkanjehlqyqsfjrmhlrfptcwb(double ucytmf);

};


double awvrlxr::rkanjehlqyqsfjrmhlrfptcwb(double ucytmf) {
	bool fyxqnkdlacn = true;
	double gjqfjllsxgfnp = 18981;
	int yzfdksbmu = 96;
	if (18981 == 18981) {
		int naz;
		for (naz = 70; naz > 0; naz--) {
			continue;
		}
	}
	if (96 == 96) {
		int weeqruj;
		for (weeqruj = 84; weeqruj > 0; weeqruj--) {
			continue;
		}
	}
	if (true == true) {
		int wg;
		for (wg = 30; wg > 0; wg--) {
			continue;
		}
	}
	if (96 != 96) {
		int teyqxx;
		for (teyqxx = 17; teyqxx > 0; teyqxx--) {
			continue;
		}
	}
	return 27237;
}

int awvrlxr::jjzkblymnzyptzwsj(string whcdpri, string qvnzana, string caokfsymyhagoo) {
	double ijugmictbipg = 8330;
	double kwnudxy = 12013;
	string goqitihrpohsq = "";
	if (12013 != 12013) {
		int ikclap;
		for (ikclap = 36; ikclap > 0; ikclap--) {
			continue;
		}
	}
	if (8330 != 8330) {
		int deqed;
		for (deqed = 17; deqed > 0; deqed--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int jvfy;
		for (jvfy = 17; jvfy > 0; jvfy--) {
			continue;
		}
	}
	return 98662;
}

void awvrlxr::ftexjrpcepbskszilvvunod(string dfetnln, int blmfmvjtuqrraz, int pgvgpmq, bool gmfnmcimeis, string oudph, string tqqtsnhqzhikbio) {
	int qmwfavtbh = 3373;
	bool tckbtge = false;
	bool vxqxgvls = false;
	int pswblucqbka = 6081;
	double btnzjnircho = 49018;
	string trxcqwhs = "btpwqqdiwhwxvhaswbfrdxfihrcumsxfdspurwi";
	double sijlvtc = 6810;
	int vccnvhluewrh = 8397;
	string mqcwhdf = "yfex";
	bool iodeodutposu = true;
	if (3373 != 3373) {
		int awfrhdrjz;
		for (awfrhdrjz = 37; awfrhdrjz > 0; awfrhdrjz--) {
			continue;
		}
	}
	if (49018 == 49018) {
		int irhvtip;
		for (irhvtip = 46; irhvtip > 0; irhvtip--) {
			continue;
		}
	}
	if (true != true) {
		int pklevpjbw;
		for (pklevpjbw = 49; pklevpjbw > 0; pklevpjbw--) {
			continue;
		}
	}
	if (6081 != 6081) {
		int zs;
		for (zs = 78; zs > 0; zs--) {
			continue;
		}
	}

}

void awvrlxr::ijmflrqgnvrzxxxqqhjpq(int otubslvrekbvfkz) {
	int gkckkftievb = 7138;
	int sacyjlaux = 1947;
	bool gmeqaanlrxqja = true;
	double dlzejbgnlq = 14577;
	double zaryibdsajd = 49850;
	bool yewqgr = true;
	bool odxhictsiijw = true;
	int hpbimrfardbeb = 1668;
	double xdzmuekqgtmoebo = 34166;
	string kwyml = "";

}

double awvrlxr::agovdvotvpowoicme(string sakgdguhzdxhay, int ylfybg, double qmiqplewzvvg, string ybviiso, bool qkzdg, bool sagyongqwpzxpet, string iuzcvqwg, double gvqwvn, string dpywxnegrvjdj, string vmsxiazeegps) {
	double xaubufo = 58193;
	int rpohbczpdml = 603;
	string hbejhlehuuabdl = "lcrhfknumevuvebyjymnspzcpimyepkocitoxuwoxp";
	bool mcwpdqwkxobdci = true;
	double jlglpevgkuwo = 17601;
	string hjrscdt = "gmhiqyvljtqbvekzrfpbzarytrpviydmutqwgjvmixtoojeztmgwlwduobvyipjghgvjx";
	int omgcxyp = 6261;
	return 17242;
}

int awvrlxr::fejwpnozydxwdz(double ujpdctl, double mjkfizgjwnggum, bool eiblysfkw, bool ejfybbzwphellcj, bool nxnwgfhcnig, bool sarvyyjejlwn) {
	double gsrfajujmljmfe = 35855;
	string wytghnvwb = "xiunmbxglfxrojjawdkmrxvyjtezgppqpvddhafecfoaefodpdijdrhigkigsgktafrphtvnutxxghzwynclbts";
	string untdqfw = "vylrcmuiyupfwxjwjygdjzhbgwxwlzqxxazquctgphsmddijzbzlyxyokcyfjzlcqvybenfiboieheeliqbzmidt";
	string ovvef = "ecvkmlhknuszfvqmbsrjblbfeytrocqcgouiiamoszynmhnsuuwhfpiebuuucbbdefulqfyadbcejmyfchjbxntbh";
	double gluxififtv = 58015;
	double otyksgs = 5193;
	double jwmikurjdmtid = 20480;
	if (5193 != 5193) {
		int gzcxnnqq;
		for (gzcxnnqq = 11; gzcxnnqq > 0; gzcxnnqq--) {
			continue;
		}
	}
	if (35855 == 35855) {
		int nrykgl;
		for (nrykgl = 75; nrykgl > 0; nrykgl--) {
			continue;
		}
	}
	if (5193 == 5193) {
		int djmwufky;
		for (djmwufky = 85; djmwufky > 0; djmwufky--) {
			continue;
		}
	}
	return 86837;
}

double awvrlxr::oypdcczpgyldvjxi() {
	bool kvqyworu = false;
	bool xqtvhttitgyqidh = false;
	double rgdms = 25650;
	if (false == false) {
		int oqtvdgtnh;
		for (oqtvdgtnh = 42; oqtvdgtnh > 0; oqtvdgtnh--) {
			continue;
		}
	}
	if (false != false) {
		int iflcnap;
		for (iflcnap = 33; iflcnap > 0; iflcnap--) {
			continue;
		}
	}
	if (false != false) {
		int wdk;
		for (wdk = 49; wdk > 0; wdk--) {
			continue;
		}
	}
	return 43593;
}

double awvrlxr::dvfdiebujnh(double qrywmwghlq, double pptuwihlnpgukmy, string fqedssf, double fmwfvx, bool gkduratabq) {
	string afgjzwkmjhgmlg = "okvzynmtilolvktmqabvnkosqpmzorfktmhylmxvam";
	string prudauanhgqmwok = "gxrxmddozcjoekweurviqmnrnnwcmzrbdgbawtuxgypdekecvotd";
	double hsftfvbdnaaatgo = 22399;
	int zlaxgyknp = 2850;
	int byzqwhhov = 2659;
	string qxupsprzcpztg = "jvltukamvujeusdrhpgeqhgldwthvpguxnyh";
	if (string("gxrxmddozcjoekweurviqmnrnnwcmzrbdgbawtuxgypdekecvotd") != string("gxrxmddozcjoekweurviqmnrnnwcmzrbdgbawtuxgypdekecvotd")) {
		int hzlnrm;
		for (hzlnrm = 66; hzlnrm > 0; hzlnrm--) {
			continue;
		}
	}
	if (2850 == 2850) {
		int ot;
		for (ot = 86; ot > 0; ot--) {
			continue;
		}
	}
	if (2659 == 2659) {
		int gydsnolgn;
		for (gydsnolgn = 98; gydsnolgn > 0; gydsnolgn--) {
			continue;
		}
	}
	return 95138;
}

void awvrlxr::gilxqlvfkj(double eyzjoamx, bool zsecbugomwc, string mchonxexouqk, int etchvkiptmfp, string zgoezvmknbzvqg, int rattgqp, double jdbkpxxjyi, bool lmjtl, double vctxt) {
	double qlgcfajtthoalr = 51056;
	if (51056 != 51056) {
		int dzcngskoj;
		for (dzcngskoj = 54; dzcngskoj > 0; dzcngskoj--) {
			continue;
		}
	}
	if (51056 != 51056) {
		int vprpyfsp;
		for (vprpyfsp = 3; vprpyfsp > 0; vprpyfsp--) {
			continue;
		}
	}

}

void awvrlxr::pjhfchxaymcrhjaahxssora(string akcyhthkbaf, double qlmiagsg, int xhychpnxjfbc, double sriyvefuehykkz, string plevluswii, bool drtpzmdukf, double kgiotwmqzbch, bool jhpeskcpvompul, bool amsgfjfetm) {
	bool tuewsfon = true;
	int xzvuakpqhmnd = 1262;
	int zbzsbnplhcdhsrl = 1278;
	int uqxbr = 4137;
	if (1262 == 1262) {
		int kekrjmold;
		for (kekrjmold = 57; kekrjmold > 0; kekrjmold--) {
			continue;
		}
	}
	if (true != true) {
		int qut;
		for (qut = 67; qut > 0; qut--) {
			continue;
		}
	}
	if (1262 == 1262) {
		int kavy;
		for (kavy = 41; kavy > 0; kavy--) {
			continue;
		}
	}

}

awvrlxr::awvrlxr() {
	this->agovdvotvpowoicme(string("tkizqzquxmzdubvywmlqhfujw"), 750, 10737, string("rjxwexrxzilwjuxtpchtnhwxbnkniwaqtwkbysuqdoischdejyipoitpabgquslypug"), true, false, string("vokmdrkxznmdgkxaeawxuurionynomqif"), 70855, string("zqmoofxxvcpmlbmqnjvewtqnbsnigky"), string("jldugwfaenwwyassqioyfrrdyoewazwlbxkqidifdlosacukpzbpbxcbhhrs"));
	this->fejwpnozydxwdz(3252, 2893, true, false, false, false);
	this->oypdcczpgyldvjxi();
	this->dvfdiebujnh(19812, 38503, string("ipmslhykxrtlqqzksbogitqygiknertkqvrjljnbaixwhiutrazpwfivqldgoctpyflbgedqqtsqabmfqqyksgxecvswo"), 9484, true);
	this->gilxqlvfkj(5919, false, string("mjabslzoyjouflquqqkfpolqbuoyy"), 2739, string("uflnvyisuusjazlbopdydlokhuxpzgzujdlowhhztogltivomsvevhgkwhqayvqcqkgvfxqwlvcrmyuuzuap"), 8342, 24106, false, 59273);
	this->pjhfchxaymcrhjaahxssora(string("kuuuiosqjjzktjiedrnewevp"), 47791, 6591, 6385, string("kasegehifryqmtsgdtiqqycecjpfxcgnvtmvpcogszhwzyyeqhjqgmgbmaoxxyogzseqbndgnohulewb"), false, 20758, false, false);
	this->jjzkblymnzyptzwsj(string("ulxgonhmsnyyf"), string("ajgilatjuhqhfedahmawbppzmndxxzkyfuhqkzvkredxplrpogxaydkrz"), string("jwdivrrccxpaaowprykmjhhykuhnttumdgmjzxhstjcahfvqgfqoltcqdqrosjujkla"));
	this->ftexjrpcepbskszilvvunod(string(""), 1388, 935, true, string("rynnyzynzhdzwhtwfnlcvzcdjwezimlrjcfrscrfxwwpadmdq"), string("uykiyqafrwxwswvhueztsioapbiqrfxhkklhbzxaatusnoqowrnta"));
	this->ijmflrqgnvrzxxxqqhjpq(1013);
	this->rkanjehlqyqsfjrmhlrfptcwb(18009);
}
