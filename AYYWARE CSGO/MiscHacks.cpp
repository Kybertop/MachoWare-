#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;

void CMiscHacks::Init()
{
}

void CMiscHacks::Draw()
{
	switch (Menu::Window.MiscTab.NameChanger.GetIndex())
	{
	case 0:
		break;
	case 1:
		Namespam();
		break;
	case 2:
		NoName();
		break;
	case 3:
		NameSteal();
		break;
	}

	if (Menu::Window.MiscTab.ChatSpam.GetState())
		ChatSpam();
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.MiscTab.OtherAutoJump.GetState())
		AutoJump(pCmd);

	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	if (Menu::Window.MiscTab.OtherCircleStrafe.GetState())
		CircularStrafe(pCmd, pCmd->viewangles);

	if (Menu::Window.MiscTab.FakeWalk.GetKey())
		FakeWalk(pCmd, bSendPacket);

	if (Menu::Window.MiscTab.OtherSlowMotion.GetKey())
		SlowMo(pCmd);

	if (Menu::Window.MiscTab.AutoPistol.GetState())
		AutoPistol(pCmd);

	if (Menu::Window.VisualsTab.DisablePostProcess.GetState())
		PostProcces();

	if (Menu::Window.MiscTab.CheatsByPass.GetState())
		SvCheats();

}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}

void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::AutoPistol(CUserCmd* pCmd)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (pWeapon)
	{
		if (GameUtils::IsBomb(pWeapon))
		{
			return;
		}

		if (!GameUtils::IsNotPistol(pWeapon))
		{
			return;
		}
	}

	static bool WasFiring = false;

	if (GameUtils::IsPistol)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}
		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
}

void CMiscHacks::PostProcces()
{
	ConVar* Meme = Interfaces::CVar->FindVar("mat_postprocess_enable");
	SpoofedConvar* meme_spoofed = new SpoofedConvar(Meme);
	meme_spoofed->SetString("mat_postprocess_enable 0");
}

void CMiscHacks::SvCheats()
{
	ConVar* sv = Interfaces::CVar->FindVar("sv_cheats");
	SpoofedConvar* sv_spoofed = new SpoofedConvar(sv);
	sv_spoofed->SetInt(2);
}

void CMiscHacks::FakeWalk(CUserCmd* pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	int FakeWalkKey = Menu::Window.MiscTab.FakeWalk.GetKey();
	if (FakeWalkKey > 0 && GUI.GetKeyState(FakeWalkKey))
	{
		static int iChoked = -1;
		iChoked++;

		if (iChoked < 1)
		{
			bSendPacket = false;

			pCmd->tick_count += 10;
			pCmd->command_number += 7 + pCmd->tick_count % 2 ? 0 : 1;

			pCmd->buttons |= pLocal->GetMoveType() == IN_BACK;
			pCmd->forwardmove = pCmd->sidemove = 0.f;
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;

			Interfaces::Globals->frametime *= (pLocal->GetVelocity().Length2D()) / 1.f;
			pCmd->buttons |= pLocal->GetMoveType() == IN_FORWARD;
		}
	}
}

void CMiscHacks::SlowMo(CUserCmd *pCmd)
{
	int SlowMotionKey = Menu::Window.MiscTab.OtherSlowMotion.GetKey();
	if (SlowMotionKey > 0 && GUI.GetKeyState(SlowMotionKey))
	{
		static bool slowmo;
		slowmo = !slowmo;
		if (slowmo)
		{
			pCmd->tick_count = INT_MAX;
		}
	}
}

void CMiscHacks::Namespam()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name("SteamXYZ");
			useSpace = !useSpace;
		}
		else
		{
			change_name("MinecrafteRr");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::NoName()
{
	change_name("\nญญญ");
}

void CMiscHacks::NameSteal()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{

		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;

		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name("p$i 1337");
	}

	start_t = clock();
}

void CMiscHacks::RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

bool CMiscHacks::CircularStrafe(CUserCmd* pCmd, Vector& OriginalView)
{
	if (!(Menu::Window.MiscTab.OtherCircleStrafe.GetState()))
		return false;

	IClientEntity* pLocalEntity = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity)
		return false;

	if (!pLocalEntity->IsAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 360)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	int CIRCLEKey = Menu::Window.MiscTab.OtherCircleButton.GetKey();

	if (CIRCLEKey > 0 && GUI.GetKeyState(CIRCLEKey))
	{
		pCmd->viewangles = OriginalView;
		RotateMovement(pCmd, GetItDoubled);
	}
	return true;
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity().Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{

	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static bool bDirection = true;

	static float move = 450.f;
	float s_move = move * 0.5065f;
	static float strafe = pCmd->viewangles.y;
	float rt = pCmd->viewangles.y, rotation;

	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{

		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		rotation = strafe - rt;

		strafe = rt;

		IClientEntity* pLocal = hackManager.pLocal();
		static bool bDirection = true;

		bool bKeysPressed = true;

		if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44))
			bKeysPressed = false;
		if (pCmd->buttons & IN_ATTACK)
			bKeysPressed = false;

		float flYawBhop = 0.f;

		float sdmw = pCmd->sidemove;
		float fdmw = pCmd->forwardmove;

		static float move = 450.f;
		float s_move = move * 0.5276f;
		static float strafe = pCmd->viewangles.y;

		if (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex() == 2 && !GetAsyncKeyState(VK_RBUTTON))
		{
			if (pLocal->GetVelocity().Length() > 45.f)
			{
				float x = 30.f, y = pLocal->GetVelocity().Length(), z = 0.f, a = 0.f;

				z = x / y;
				z = fabsf(z);

				a = x * z;

				flYawBhop = a;
			}

			if ((GetAsyncKeyState(VK_SPACE) && !(pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
			{

				if (bDirection)
				{
					AutoStrafeView -= flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = -450;
					bDirection = false;
				}
				else
				{
					AutoStrafeView += flYawBhop;
					GameUtils::NormaliseViewAngle(AutoStrafeView);
					pCmd->sidemove = 430;
					bDirection = true;
				}

				if (pCmd->mousedx < 0)
				{
					pCmd->forwardmove = 22;
					pCmd->sidemove = -450;
				}

				if (pCmd->mousedx > 0)
				{
					pCmd->forwardmove = +22;
					pCmd->sidemove = 450;
				}
			}
		}
	}
}

void CMiscHacks::undetected()
{
	float pJunkcode = 12579452111873;
	pJunkcode = 13487989242;
	if (pJunkcode = 171879562432425)
		pJunkcode = 183241025879425;
	pJunkcode = 1732198201425;
	pJunkcode = 111178978911;
	if (pJunkcode = 3492511173490534);
	pJunkcode = 2304586893748943;
	pJunkcode = 43931390453;
	if (pJunkcode = 17115232425)
		pJunkcode = 1834562425;
	pJunkcode = 173245692501;
	pJunkcode = 435781949844;
	if (pJunkcode = 349246588490534);
	pJunkcode = 23044748943;
	pJunkcode = 4390456870453;
	if (pJunkcode = 1157989342425)
		pJunkcode = 115602543425;
	pJunkcode = 48562198465;
	pJunkcode = 4357897851345868;
	if (pJunkcode = 34934624156323474);
	pJunkcode = 23048713516848;
	pJunkcode = 4456391980453;
	if (pJunkcode = 34934623474);
	pJunkcode = 292489543516848;
	pJunkcode = 3910453;
	if (pJunkcode = 3493462415474);
	pJunkcode = 230483516848;
	pJunkcode = 39145320453;
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

void CMiscHacks::ChatSpam()
{
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("Boosting service. Easy and Fast SteamXYZ");
	}
	else
	{
		SayInChat("SteamXYZ Your derank service.");
	}

	start_t = clock();
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class anocwuh {
public:
	int tkyqdxjfaqthlv;
	anocwuh();
	double wxexwggkdcxwpexrdnwduiwl(double iecomqioqs, bool aesnyg, bool vxmtybopyu, string tlwcdg, string rrhtumnioclxyjb, string npxwnfsvzup, bool ymusskdcbvpdx);
	double canahrnqrsdoyh(double fwosjrzqymbmcmb, double lsyvuggz);
	void yuxlkygovjznerc(string xklzcado, int qezntpzwucma, double fiyloihykbtftm, string ffimp, bool ecpqwiss, bool mdxrevgj, bool lubtdb);
	void ugxihzmzcg(string tozuxjmvuhf, int xpyznchqatv, int ulgzjvettc, int mbhmlqethgbfw, bool uuhwaexclecjxj, int dmgdofa);
	string vuzufpdulfqeejexdv(bool kjmukbytl, int nvyquqhvhgyzp, string xlceyygfspgkhow, bool ekjcvbwssganilo, double syoswran);
	string oiitcyquzxdbfrqxlwlsf(double newiayuyr, string scgzkabmrl, int yqkvnoebvj, string pzzgdo, bool cerjlkc, bool wjnkuik, int vwvqybbvxztodbc, bool fuvvksmf);

protected:
	bool owawkourrhnwm;
	bool vtwgutapnwhsu;
	double cxzovutjecxskv;

	int owjnfnptwytutu(double vhbffflbkeavqq);
	int wcajnlqkfcw(string sklitjfusqleafk, int ryoaw, string gjmkmndwpzgk, double adieebpui, int tjlrmgsozkxy, bool awhepwt, string dhdpjjcrbipfdpz, string xacqga, int lzpvwm);
	string fjvfcjovlzltdpcqesnnkjt(double mwndrergt, bool avsjlqdekygtxn, double pmrxfgthuf, double fhjmifet, int ulfoufjndidvmy);
	bool thlpavphykmdczrgzzdrqls(bool cambtjyt, bool bjzcdrdr, string okvyhlhgepnvq, double zidfs);

private:
	bool bxkcekqhsfcft;

	string vsnlsfhozcvdpngbxlvsvoh(bool iorjjhiwxv, double oyloffylspuh, string guzyb, bool ahhvajomteagq, string fwytdbsjdil, double ygylztupzru, bool oflsehlfuq);
	bool rcescistugjsknfkqaafd();
	string dcgtwmjtpjfool(bool gdjhqnv, bool vgqsbnkfy, string ojwbfkmhpf, string djkhsbuftovpavd, bool vjmkumodagi, double ueawzsydhjh, int ivrskveyd);
	void wzydpihilmtzdtqdwmwxthrqe(double lznzde, string dgdaiclogq, string mmdcrydul, double qdamwetopcxex, string kcfckv, int mkbrzrx, string ihhsipuzb, bool evybe, string ajjpcbpunu);
	int vncngaahllykebjifuyspjzma(string seqhodqcjftr, double pbxpa, double xevnfb, bool qppnqlcqb, int bmithmtjeaybf, bool vpoao);
	double wkmsezymso(int eablyquzz, int wwdxmbzky, double oabcpyiwthzvgu, int rcqqhzp, string urdzyomrino, int yxwpyoekrokv, int onsjv);

};


string anocwuh::vsnlsfhozcvdpngbxlvsvoh(bool iorjjhiwxv, double oyloffylspuh, string guzyb, bool ahhvajomteagq, string fwytdbsjdil, double ygylztupzru, bool oflsehlfuq) {
	bool cxmqyxousltjisz = false;
	bool xmhitkmdyxr = true;
	int zxsrg = 2552;
	double hpzvazovmyj = 42818;
	double idolewg = 6361;
	string gqdjlfniqemyg = "kyebombznfiifjrgjzeurvemhetrleakpggdbyceye";
	int geucfvaeyvyku = 3664;
	string pqnkdzwclw = "xevvfsadzqzcgpfqcusrmrropdedrpwezpupkkzvdixykpokugzipkrqvmbmvs";
	int yfavvneqgciwtc = 5401;
	if (2552 == 2552) {
		int gdt;
		for (gdt = 99; gdt > 0; gdt--) {
			continue;
		}
	}
	if (string("kyebombznfiifjrgjzeurvemhetrleakpggdbyceye") == string("kyebombznfiifjrgjzeurvemhetrleakpggdbyceye")) {
		int axlqs;
		for (axlqs = 0; axlqs > 0; axlqs--) {
			continue;
		}
	}
	if (true != true) {
		int kzvvljxi;
		for (kzvvljxi = 28; kzvvljxi > 0; kzvvljxi--) {
			continue;
		}
	}
	if (3664 == 3664) {
		int ywxsh;
		for (ywxsh = 45; ywxsh > 0; ywxsh--) {
			continue;
		}
	}
	return string("");
}

bool anocwuh::rcescistugjsknfkqaafd() {
	int zponhtn = 1139;
	double eulibl = 25589;
	int ujetfqfss = 7;
	double bjcxn = 22462;
	int xbddfro = 2430;
	double bppdowbt = 69841;
	bool pbhaajznbwnavz = true;
	string ybislmzbd = "mzvphwgrrrwxmzyfunsotrfsbnvsqgdbcpvgckbl";
	bool gicywqaejbxs = false;
	double ritjipegfrx = 16522;
	if (25589 == 25589) {
		int vqbznr;
		for (vqbznr = 15; vqbznr > 0; vqbznr--) {
			continue;
		}
	}
	return false;
}

string anocwuh::dcgtwmjtpjfool(bool gdjhqnv, bool vgqsbnkfy, string ojwbfkmhpf, string djkhsbuftovpavd, bool vjmkumodagi, double ueawzsydhjh, int ivrskveyd) {
	return string("hwhjlddahrcxt");
}

void anocwuh::wzydpihilmtzdtqdwmwxthrqe(double lznzde, string dgdaiclogq, string mmdcrydul, double qdamwetopcxex, string kcfckv, int mkbrzrx, string ihhsipuzb, bool evybe, string ajjpcbpunu) {
	bool ozjjk = true;
	bool naogbtaldt = true;
	double dxjabsnwzflt = 37385;
	bool oclmz = true;
	bool nqfveuqz = false;
	string jhojiukcwqh = "upxsrgfhyhlhgyencdpgrkihmivvzeeovfwpwlglzkwhhjtrbrfo";
	double knaccwsle = 65094;
	int vsghg = 1709;
	string qrdfemeode = "bpfkjhxoryrfhtmmlrlw";
	string qkydguskvyjp = "lnfnvolcalefztelibgtnsxbiswqhobouzbtmgvooxfyoxjcowsjhxiikyjxnjopalwrepmeypejiegzua";
	if (string("upxsrgfhyhlhgyencdpgrkihmivvzeeovfwpwlglzkwhhjtrbrfo") != string("upxsrgfhyhlhgyencdpgrkihmivvzeeovfwpwlglzkwhhjtrbrfo")) {
		int aifxjfdac;
		for (aifxjfdac = 16; aifxjfdac > 0; aifxjfdac--) {
			continue;
		}
	}
	if (65094 == 65094) {
		int xicd;
		for (xicd = 48; xicd > 0; xicd--) {
			continue;
		}
	}

}

int anocwuh::vncngaahllykebjifuyspjzma(string seqhodqcjftr, double pbxpa, double xevnfb, bool qppnqlcqb, int bmithmtjeaybf, bool vpoao) {
	int hokqxrbjtpsses = 6599;
	bool ltnlkurenqif = false;
	string fbbvbobuoyb = "swnvrmniwpeuamodlbnztfzzhbsmxtk";
	double jwhbddvgzkuryto = 50008;
	double ykczxcjl = 1151;
	if (6599 != 6599) {
		int hsmzadq;
		for (hsmzadq = 36; hsmzadq > 0; hsmzadq--) {
			continue;
		}
	}
	if (1151 != 1151) {
		int gb;
		for (gb = 42; gb > 0; gb--) {
			continue;
		}
	}
	return 81183;
}

double anocwuh::wkmsezymso(int eablyquzz, int wwdxmbzky, double oabcpyiwthzvgu, int rcqqhzp, string urdzyomrino, int yxwpyoekrokv, int onsjv) {
	string qurzotbsxmog = "buramyhxwtmpcumwspyhkbmxrzkkolqkayusmfgubrzoacfrgjgdskvdwjohrpaocsqilooffojypimtuodjk";
	int emibwgqlbodm = 157;
	double xnpkmtovewyuvi = 15654;
	bool zycrelk = false;
	bool tuecryscpahazhd = false;
	bool nryhlzfi = false;
	int rdxgmazi = 1758;
	return 10477;
}

int anocwuh::owjnfnptwytutu(double vhbffflbkeavqq) {
	double aclbgpapy = 5018;
	bool hghjtcm = true;
	if (true != true) {
		int rgblvhpj;
		for (rgblvhpj = 98; rgblvhpj > 0; rgblvhpj--) {
			continue;
		}
	}
	if (true == true) {
		int yzoagn;
		for (yzoagn = 34; yzoagn > 0; yzoagn--) {
			continue;
		}
	}
	if (true == true) {
		int rnydgmqh;
		for (rnydgmqh = 31; rnydgmqh > 0; rnydgmqh--) {
			continue;
		}
	}
	if (5018 != 5018) {
		int sqowddf;
		for (sqowddf = 12; sqowddf > 0; sqowddf--) {
			continue;
		}
	}
	return 73334;
}

int anocwuh::wcajnlqkfcw(string sklitjfusqleafk, int ryoaw, string gjmkmndwpzgk, double adieebpui, int tjlrmgsozkxy, bool awhepwt, string dhdpjjcrbipfdpz, string xacqga, int lzpvwm) {
	string nmafhwhoubcsewm = "jypmrdzhpkifwwhkncyyuzjkcbvicmttbcwmbie";
	int wabeg = 2920;
	string qmwqvb = "zazbmvqxofjliocmurkqmkegmizuvlqjnrjolppqjzuktcrlspoboigjvglddyonudykhwygoqhvuiaznavwbczfnvqytncy";
	int eamfchkrqtjkier = 3516;
	if (2920 != 2920) {
		int qujaubgwi;
		for (qujaubgwi = 80; qujaubgwi > 0; qujaubgwi--) {
			continue;
		}
	}
	if (3516 != 3516) {
		int kbcytragx;
		for (kbcytragx = 50; kbcytragx > 0; kbcytragx--) {
			continue;
		}
	}
	return 27900;
}

string anocwuh::fjvfcjovlzltdpcqesnnkjt(double mwndrergt, bool avsjlqdekygtxn, double pmrxfgthuf, double fhjmifet, int ulfoufjndidvmy) {
	bool xolvbdqkiqrd = false;
	bool smaet = true;
	double zepihe = 43605;
	bool hsliithmdsh = false;
	string qpqsvfx = "scvhfaiebfzvlpsiowssdnczgedaxmywtlfhkanycfurt";
	bool ttcjkcghjintbt = false;
	string znhmyxateeql = "rkrxozwcihomhivbnqlmlowmkbemrpixmra";
	if (string("rkrxozwcihomhivbnqlmlowmkbemrpixmra") == string("rkrxozwcihomhivbnqlmlowmkbemrpixmra")) {
		int ahg;
		for (ahg = 99; ahg > 0; ahg--) {
			continue;
		}
	}
	if (43605 != 43605) {
		int bodojopzi;
		for (bodojopzi = 76; bodojopzi > 0; bodojopzi--) {
			continue;
		}
	}
	return string("jrxvuhymohqgwpmni");
}

bool anocwuh::thlpavphykmdczrgzzdrqls(bool cambtjyt, bool bjzcdrdr, string okvyhlhgepnvq, double zidfs) {
	bool txiuxfppaaaejqa = false;
	bool dplmxqlw = false;
	double fquxlje = 19560;
	string uixgemnt = "zzvowkztvollrocxdxkxdwsnqxnvqdesukkdibepkwjstrsje";
	int mefdpjib = 3369;
	string qyrvy = "rwlttzgnvewibouowmnzfgcfokyketmgrxdknugmwz";
	if (false != false) {
		int dhzbhtx;
		for (dhzbhtx = 77; dhzbhtx > 0; dhzbhtx--) {
			continue;
		}
	}
	if (string("rwlttzgnvewibouowmnzfgcfokyketmgrxdknugmwz") == string("rwlttzgnvewibouowmnzfgcfokyketmgrxdknugmwz")) {
		int kcoithe;
		for (kcoithe = 82; kcoithe > 0; kcoithe--) {
			continue;
		}
	}
	return false;
}

double anocwuh::wxexwggkdcxwpexrdnwduiwl(double iecomqioqs, bool aesnyg, bool vxmtybopyu, string tlwcdg, string rrhtumnioclxyjb, string npxwnfsvzup, bool ymusskdcbvpdx) {
	int fasgaogblwdmqtw = 3591;
	string znnhi = "ktejutttcnrkjohxffgrjedfazedakklxolkobwvknhwwvhvagtfrfvdk";
	double xjqiolylj = 44468;
	if (string("ktejutttcnrkjohxffgrjedfazedakklxolkobwvknhwwvhvagtfrfvdk") == string("ktejutttcnrkjohxffgrjedfazedakklxolkobwvknhwwvhvagtfrfvdk")) {
		int xaffuz;
		for (xaffuz = 73; xaffuz > 0; xaffuz--) {
			continue;
		}
	}
	return 87886;
}

double anocwuh::canahrnqrsdoyh(double fwosjrzqymbmcmb, double lsyvuggz) {
	return 43289;
}

void anocwuh::yuxlkygovjznerc(string xklzcado, int qezntpzwucma, double fiyloihykbtftm, string ffimp, bool ecpqwiss, bool mdxrevgj, bool lubtdb) {
	int ikvxtycdmlh = 1257;
	double ywybyfkmpexj = 72187;
	string ucvztpmk = "xqjppdpucgyjuhwoudddtawrscfjgcwydpqyfj";
	double okezfgl = 242;
	bool carzumyovelfbsc = true;
	if (72187 == 72187) {
		int qmvh;
		for (qmvh = 72; qmvh > 0; qmvh--) {
			continue;
		}
	}

}

void anocwuh::ugxihzmzcg(string tozuxjmvuhf, int xpyznchqatv, int ulgzjvettc, int mbhmlqethgbfw, bool uuhwaexclecjxj, int dmgdofa) {
	double gcaduoi = 69258;
	string qltdnlomlbcj = "ujcutlipgbcadjlhkodlmyuscklub";
	if (69258 == 69258) {
		int lip;
		for (lip = 63; lip > 0; lip--) {
			continue;
		}
	}

}

string anocwuh::vuzufpdulfqeejexdv(bool kjmukbytl, int nvyquqhvhgyzp, string xlceyygfspgkhow, bool ekjcvbwssganilo, double syoswran) {
	return string("erzkw");
}

string anocwuh::oiitcyquzxdbfrqxlwlsf(double newiayuyr, string scgzkabmrl, int yqkvnoebvj, string pzzgdo, bool cerjlkc, bool wjnkuik, int vwvqybbvxztodbc, bool fuvvksmf) {
	bool ssfcpodtea = false;
	string ddaaaayq = "qriamurnispljuqbjhsvvrckgghecdvuexpotnxltbxvfpuxiokbcxlikoduupojdrtxvaulczqqjipnmmawqphzang";
	if (false != false) {
		int pzqdw;
		for (pzqdw = 58; pzqdw > 0; pzqdw--) {
			continue;
		}
	}
	return string("kfsacmoyikogsbiauz");
}

anocwuh::anocwuh() {
	this->wxexwggkdcxwpexrdnwduiwl(2400, true, true, string("jadypgwrjvhjqtuobcntgyvjzuwnhbbyfjznspzmzhdbmmyhrkdorroln"), string("nbgbeqceywojeohdwdpzxevvqntgefaudbjqmjzekuwwscfiwnxwpshtdbiilwcsecqhjvodcqqnesneromszmxxghzgqa"), string("gdhdobfrglqkxrbaakxxpdimpzzjnnnhatymhisvsohloayovgwiogjpzdfvswyfenwyrcpqomlqrxhs"), true);
	this->canahrnqrsdoyh(15917, 33674);
	this->yuxlkygovjznerc(string("bciusupqxnmjfgfincrtkyrsncqexgnqexbqomdwihtrkjtupyvgcybvet"), 532, 27817, string("qdvo"), true, false, false);
	this->ugxihzmzcg(string("ltkgunvrdtiwpedaoovvywnnqjcwjjdpuqzxdifanlnffhyrjxfgitgaulsyjqrcxw"), 900, 516, 207, true, 1385);
	this->vuzufpdulfqeejexdv(false, 1095, string("goctggfwqvofzbdrkcmwfhnwfprcqdnyqvombhzyujcomgfzczjmy"), false, 55458);
	this->oiitcyquzxdbfrqxlwlsf(58615, string("hpzwbjorcsktefbvxwbtlaaxdoqkemrpujocfjrlnbfhmfkhrjkzfnglaedthxyhfxxsnqpohqefkapnptfbmbgoekp"), 2552, string("rpbssiqzmbchyyhuuvdmrkipfuwvlshcajgvlsczkkiirlvkadpvlhcuydvjbxukcyzyk"), false, true, 6449, false);
	this->owjnfnptwytutu(35236);
	this->wcajnlqkfcw(string("rydhuxft"), 861, string("nvvxjywprlihwjdduhjjdmaogtdjqjbdpzemxaaeqsbyqrohtrhstmdrdn"), 30392, 1639, false, string("wxxtilgougpsvgjqdlmmdansxmkgtlbc"), string("ebmulygayjnfbrmodrvzhwqyaxnicooyrzhkajtivlblhvuggahkpxkwugzyuuzxax"), 3734);
	this->fjvfcjovlzltdpcqesnnkjt(1561, false, 23177, 17998, 4889);
	this->thlpavphykmdczrgzzdrqls(false, true, string("rcskbgutzfmkfsncggvdjxtzmnbsccfxggqhenoxjvbyuwsraclyxgggvetcirbabceffrtfwomdqrdnoycwj"), 17907);
	this->vsnlsfhozcvdpngbxlvsvoh(false, 23583, string("junrwepangezdgzwncyxpyfhbwvdiozqkqyqqbwlsyabdmdggkltuyzplfjeyayiqihdle"), true, string("qjrjxrtpatdwrlnyspnbmvtpsfkmo"), 23188, false);
	this->rcescistugjsknfkqaafd();
	this->dcgtwmjtpjfool(false, true, string("yaytvodletqcqwhzhajpfcdsfgresbscymzyujzwtwynpckyrqvyojjyfxmipvmslvaiasbjtdvjzyvgkhczcewqtirflpvwoqow"), string("qxfkgwkaxpymjjgguwwfcjiiadvrtxfxqthswinsnvmvajoajghbatyzxsribhhifcdumxas"), true, 18834, 1784);
	this->wzydpihilmtzdtqdwmwxthrqe(27811, string("svjbqdoyzaylbmmgmpavgzhtjglsipcxuaxxjwrcrqtasznqmqdcwqgxtblchyqsnyoue"), string("tdnxmmbygutpcrswkyonsxnzmlnjuefcqdezvbyeuvervkgbngrnlvytsmqrblgylxviqbyjoynalqdhqsmeqxbaeoekbk"), 51426, string("duwglrluwloxerjsp"), 4476, string("azpups"), true, string("gqcbsgrcqlxrthvfkumqsaobvlqbiqkqyaxskqtycuhlbzbe"));
	this->vncngaahllykebjifuyspjzma(string("twzhjnkhwerygltqyqjxuccdajhzrximvuuchnlueypweemmzpanfgqqpczkcxklsastqlrdvmzxzbpglvskgpbktcbgd"), 88276, 17912, true, 2011, false);
	this->wkmsezymso(2887, 3149, 23859, 1175, string("iheicserzgjvwfwitifjlbxkhgfvjsoqzxruoq"), 1306, 2296);
}
