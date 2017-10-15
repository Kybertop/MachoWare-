#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"

#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "hitmarker.h"

#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

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

Vector LastAngleAA;
bool Resolver::didhitHS;
CUserCmd* Globals::UserCmd;
IClientEntity* Globals::Target;
int Globals::Shots;
bool Globals::change;
int Globals::TargetID;
std::map<int, QAngle>Globals::storedshit;
int Globals::missedshots;

typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef void(__thiscall* PlaySoundFn)(const char*);

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();

PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;
PlaySoundFn oPlaySound;


void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
void __stdcall hookedPlaySound(const char* name);

namespace Hooks
{
	Utilities::Memory::VMTManager VMTPanel; 
	Utilities::Memory::VMTManager VMTClient;
	Utilities::Memory::VMTManager VMTClientMode;
	Utilities::Memory::VMTManager VMTModelRender; 
	Utilities::Memory::VMTManager VMTPrediction; 
	Utilities::Memory::VMTManager VMTPlaySound; 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
};

void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}

void Hooks::Initialise()
{
	Interfaces::Engine->ExecuteClientCmd("clear");
	Interfaces::CVar->ConsoleColorPrintf(Color(0, 255, 0, 255), ("\n Injected successfully! ^>>SteamXYZ<<^\n"));
	Interfaces::CVar->ConsoleColorPrintf(Color(255, 0, 0, 255), ("\n Build Date: %s \n"), __DATE__);
	Interfaces::CVar->ConsoleColorPrintf(Color(0, 0, 255, 100), ("\n Build TimeS: %s \n"), __TIME__);

	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);
	
	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);

	VMTPlaySound.Initialise((DWORD*)Interfaces::Surface);
	oPlaySound = (PlaySoundFn)VMTPlaySound.HookMethod((DWORD)&hookedPlaySound, 82);
}

void MovementCorrection(CUserCmd* pCmd)
{
}

float clip(float n, float lower, float upper)
{
	return (std::max)(lower, (std::min)(n, upper));
}

const char* clantaganimation[18] =
{
	" ",
	"S",
	"ST ",
	"STE",
	"STEA",
	"STEAM",
	"STEAMX",
	"STEAMXY",
	"STEAMXYZ",
	"STEAMXY",
	"STEAMX",
	"STEAM",
	"STEA",
	"STE",
	"ST",
	"S",
	" ",
};

int kek = 0;
int autism = 0;

int LagCompBreak() {
	IClientEntity *pLocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Vector velocity = pLocalPlayer->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f) {
		auto distance_per_tick = speed *
			Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
	return 1;
}

bool __stdcall CreateMoveClient_Hooked( float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		IClientEntity* pEntity;
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
		{
			Hacks::MoveHacks(pCmd, bSendPacket);
			ResolverSetup::GetInst().CM(pEntity);
		}

		if (Menu::Window.MiscTab.FakeLagEnable.GetState())
		{
			static int ticks = 0;
			static int ticks1 = 0;
			static int iTick = 0;
			static int iTick1 = 0;
			static int iTick2 = 0;
			int ticksMax = 16;
			int value = Menu::Window.MiscTab.FakeLagChoke.GetValue();
			if (Menu::Window.MiscTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.MiscTab.FakeLagTyp.GetIndex() == 2)
			{
				if (ticks >= ticksMax)
				{
					bSendPacket = true;
					ticks = 0;
				}
				else
				{
					int packetsToChoke;
					if (pLocal->GetVelocity().Length() > 0.f)
					{
						packetsToChoke = (int)((128.f / Interfaces::Globals->interval_per_tick) / pLocal->GetVelocity().Length()) + 1;
						if (packetsToChoke >= 15)
							packetsToChoke = 14;
						if (packetsToChoke < value)
							packetsToChoke = value;
					}
					else
						packetsToChoke = 0;

					bSendPacket = ticks < 18 - packetsToChoke;;
				}
				ticks++;
			}

			if (Menu::Window.MiscTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.MiscTab.FakeLagTyp.GetIndex() == 4)
			{
				if (!(pLocal->GetFlags() & FL_ONGROUND))
				{
					if (ticks1 >= ticksMax)
					{
						bSendPacket = true;
						ticks1 = 0;
					}
					else
					{
						int packetsToChoke;
						if (pLocal->GetVelocity().Length() > 0.f)
						{
							packetsToChoke = (int)((128.f / Interfaces::Globals->interval_per_tick) / pLocal->GetVelocity().Length()) + 1;
							if (packetsToChoke >= 15)
								packetsToChoke = 14;
							if (packetsToChoke < value)
								packetsToChoke = value;
						}
						else
							packetsToChoke = 0;

						bSendPacket = ticks1 < 18 - packetsToChoke;;
					}
					ticks1++;
				}
			}
			if (Menu::Window.MiscTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.MiscTab.FakeLagTyp.GetIndex() == 1)
			{

				if (iTick < value) {
					bSendPacket = false;
					iTick++;
				}
				else {
					bSendPacket = true;
					iTick = 0;
				}
			}
			if (Menu::Window.MiscTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.MiscTab.FakeLagTyp.GetIndex() == 3)
			{
				if (!(pLocal->GetFlags() & FL_ONGROUND))
				{
					if (iTick1 < value) {
						bSendPacket = false;
						iTick1++;
					}
					else {
						bSendPacket = true;
						iTick1 = 0;
					}
				}
			}
			if (Menu::Window.MiscTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.MiscTab.FakeLagTyp.GetIndex() == 5)
			{
				value = LagCompBreak();
				if (iTick2 < value) {
					bSendPacket = false;
					iTick2++;
				}
				else {
					bSendPacket = true;
					iTick2 = 0;
				}
			}
		}

		if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		{
			static auto SetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)(Utilities::Memory::FindPatternV2("engine.dll", "53 56 57 8B DA 8B F9 FF 15"))));
			if (Menu::Window.MiscTab.ClanTag.GetState())
			{
				static size_t lastTime = 0;

				if (GetTickCount() > lastTime)
				{
					kek++;
					if (kek > 10)
					{
						autism = autism + 1;

						if (autism >= 18) // number of clantaganimation
							autism = 0;

						char random[255];
						SetClanTag(clantaganimation[autism], clantaganimation[autism]);
						lastTime = GetTickCount() + 500;
					}

					if (kek > 10)
						kek = 0;
				}
			}
			else
				SetClanTag("", "");
		}

		static bool abc = false;
		if (Menu::Window.VisualsTab.NightMode.GetState())
		{
			if (!abc)
			{
				ConVar* staticdrop = Interfaces::CVar->FindVar("r_DrawSpecificStaticProp");
				SpoofedConvar* staticdrop_spoofed = new SpoofedConvar(staticdrop);
				staticdrop_spoofed->SetInt(0);

				{
					for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
					{
						IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

						if (!pMaterial)
							continue;

						if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))
						{
							pMaterial->ColorModulation(0.1f, 0.1f, 0.1f);
						}
						if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
						{
							pMaterial->ColorModulation(0.3f, 0.3f, 0.3f);
						}
					}
				}
			}
			abc = true;
		}
		else
		{
			abc = false;
		}

		static bool nosmoke;
		if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
		{

			if (!nosmoke) {
				ConVar* particles = Interfaces::CVar->FindVar("r_drawparticles");
				SpoofedConvar* particles_spoofed = new SpoofedConvar(particles);
				particles_spoofed->SetInt(0);
				
				ConVar* fog = Interfaces::CVar->FindVar("fog_enable");
				SpoofedConvar* fog_spoofed = new SpoofedConvar(fog);
				fog_spoofed->SetInt(0);
				nosmoke = true;
			}

		}
		else if (!Menu::Window.VisualsTab.OtherNoSmoke.GetState())
		{
			if (nosmoke)
			{
				ConVar* particles = Interfaces::CVar->FindVar("r_drawparticles");
				SpoofedConvar* particles_spoofed = new SpoofedConvar(particles);
				particles_spoofed->SetInt(1);

				ConVar* fog = Interfaces::CVar->FindVar("fog_enable");
				SpoofedConvar* fog_spoofed = new SpoofedConvar(fog);
				fog_spoofed->SetInt(1);
				nosmoke = false;
			}

		}

		if (Menu::Window.VisualsTab.NightSky.GetState())
		{
			ConVar* sky = Interfaces::CVar->FindVar("sv_skyname");
			SpoofedConvar* sky_spoofed = new SpoofedConvar(sky);
			sky_spoofed->SetString("sky_csgo_night02");
		}

		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f); 
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800); 
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (bSendPacket)
			LastAngleAA = pCmd->viewangles;
	}

	return false;

	int circlekey = Menu::Window.MiscTab.OtherCircle.GetKey();
	int circle = Menu::Window.MiscTab.CircleAmount.GetValue();

	static float CurYaw = 0.f;
	if (circlekey > 0 && GUI.GetKeyState(circlekey))
	{
		pCmd->forwardmove = -450.f;
		pCmd->sidemove = 0.f;
		static float CurYaw = 0.f;
		float Vel2D = hackManager.pLocal()->GetVelocity().Length2D();
		if (Vel2D >= 0)
		{

			float TurnRate = clip((Vel2D / 90.f), circle, 1.25f);
			CurYaw += TurnRate;


			pCmd->viewangles.y = CurYaw;
		}
	}
}

bool sendpacket = true;

void FakeLag(CUserCmd* pCmd)
{

	IClientEntity* pLocal = hackManager.pLocal();

	int choke = Menu::Window.MiscTab.FakeLagChoke.GetValue() + 1;
	if (choke > 15)
		choke = 14;

	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
	{

		static int FakelagTick;
		FakelagTick++;
		int Mode = Menu::Window.MiscTab.FakeLagTyp.GetIndex();
		if (Mode == 0) sendpacket = Menu::Window.MiscTab.FakeLagEnable.GetState() ? !(FakelagTick % (choke)) : 1; 
		if (Mode == 1)
		{
			static int FakelagFactor = 2;
			sendpacket = Menu::Window.MiscTab.FakeLagTyp.GetIndex() ? !(FakelagTick % (FakelagFactor + 1)) : 1;
			if (sendpacket)
				FakelagFactor = max(1, min((int)(fabs(pLocal->GetVelocity().Length2D() / 80.f)), 12));
		}
		if (Mode == 2)
		{
			static int FakelagFactor = 8;
			sendpacket = Menu::Window.MiscTab.FakeLagTyp.GetIndex() ? !(FakelagTick % (FakelagFactor + 1)) : 1;
			if (sendpacket) {
				FakelagFactor = (rand() % 7) + 6;
			}
		}
		if (Mode == 3)
		{
			static int FakelagFactor = Menu::Window.MiscTab.FakeLagChoke.GetValue();
			sendpacket = Menu::Window.MiscTab.FakeLagTyp.GetIndex() ? !(FakelagTick % (FakelagFactor + 1)) : 1;
			if (sendpacket) {
				FakelagFactor = (rand() % 10) + 1;
			}
		}
		if (Mode == 4)
		{
			static int FakelagFactor = 5;
			int Based = FakelagFactor / 3;
			sendpacket = Menu::Window.MiscTab.FakeLagTyp.GetIndex() ? !(FakelagTick % (Based)) : 1;
			if (sendpacket) {
				FakelagFactor += 1;
				if (Based >= Menu::Window.MiscTab.FakeLagChoke.GetValue()) FakelagFactor = 3;
			}
		}
		if (Mode == 5)
		{
			static int FakelagFactor = 10;
			static int m_iOldHealth = 0;
			static int iChoked;
			if (m_iOldHealth != pLocal->GetHealth()) {
				m_iOldHealth = pLocal->GetHealth();
				FakelagFactor = rand() % 7 + 6 + 1;
			}
			sendpacket = Menu::Window.MiscTab.FakeLagTyp.GetIndex() ? !(FakelagTick % (FakelagFactor + 1)) : 1;
			if (!sendpacket)
				iChoked++;
			else iChoked = 0;

			if (iChoked > 14) {
				sendpacket = 1;
				FakelagTick = -1;
			}
		}
	}
}

std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Menu::Window.VisualsTab.Active.GetState() && Menu::Window.VisualsTab.OtherNoScope.GetState() && strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)) == 0)
		return;

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (Menu::Window.VisualsTab.Clock.GetState())
		{
			Render::Textf(14, 24, Color(255, 255, 255, 255), Render::Fonts::Slider, ("%s"), GetTimeString().c_str());
		}
		if (Menu::Window.VisualsTab.Watermark.GetState())
		{
			Render::Text(7, 7, Color(255, 0, 0, 255), Render::Fonts::Slider, ("STEAMXYZ"));
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && Menu::Window.VisualsTab.OtherHitmarker.GetState())
			hitmarker::singleton()->on_paint();

		if (Menu::Window.VisualsTab.Active.GetState())
		{
			float AmbientRedAmount = Menu::Window.VisualsTab.AmbientRed.GetValue();
			float AmbientGreenAmount = Menu::Window.VisualsTab.AmbientGreen.GetValue();
			float AmbientBlueAmount = Menu::Window.VisualsTab.AmbientBlue.GetValue();

			ConVar* AmbientRedCvar = Interfaces::CVar->FindVar("mat_ambient_light_r");
			*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
			AmbientRedCvar->SetValue(AmbientRedAmount);

			ConVar* AmbientGreenCvar = Interfaces::CVar->FindVar("mat_ambient_light_g");
			*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
			AmbientGreenCvar->SetValue(AmbientGreenAmount);

			ConVar* AmbientBlueCvar = Interfaces::CVar->FindVar("mat_ambient_light_b");
			*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
			AmbientBlueCvar->SetValue(AmbientBlueAmount);
		}

		Menu::DoUIFrame();
	}
}

VOID WINAPI_INLINE hookedPlaySound(const char* name)
{
	__asm
	{

		PUSH name
		CALL oPlaySound
	}

	if (Interfaces::Engine->IsConnected() || !(Menu::Window.MiscTab.AutoAccept.GetState()))
		return;

	if (!strcmp(name, "\x55\x49\x2F\x63\x6F\x6D\x70\x65\x74\x69\x74\x69\x76\x65\x5F\x61\x63\x63\x65\x70\x74\x5f\x62\x65\x65\x70\x2E\x77\x61\x76"))
	{
		Offsets::IsReady = (IsReadyFn)(Offsets::Functions::IsReadySig);
		Offsets::IsReady;
	}
}

bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;
bool flipAA;
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBot.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy ak47; buy m4a1;");
		}
		else if (Menu::Window.MiscTab.BuyBot.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy awp;");
		}
		else if (Menu::Window.MiscTab.BuyBot.GetIndex() == 3)
		{
			if (strcmp(Event->GetName(), "round_start") == 0) 
				Interfaces::Engine->ClientCmd_Unrestricted("buy scar20; buy g3sg1;buy elite;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotGrenades.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy flashbang; buy flashbang; buy smokegrenade; buy hegrenade;");
		}
		else if (Menu::Window.MiscTab.BuyBotGrenades.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy flashbang; buy smokegrenade; buy hegrenade; buy molotov; buy incgrenade;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotKevlar.GetState())
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy vest; buy vesthelm;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotDefuser.GetState())
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy defuser;");
		}
	}

	if (Menu::Window.RageBotTab.AimbotResolver.GetIndex() == 3)
	{
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			IClientEntity* pLocal = hackManager.pLocal();
			if (Interfaces::Engine->GetPlayerForUserID(deadfag) != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) == Interfaces::Engine->GetLocalPlayer())
			{
				IClientEntity* hittedplayer = (IClientEntity*)(Interfaces::Engine->GetPlayerForUserID(deadfag));
				int hit = Event->GetInt("hitgroup");
				if (hit == 1 && hittedplayer && deadfag && attackingfag)
				{
					Resolver::didhitHS = true;
					Globals::missedshots = 0;
				}
				else
				{
					Resolver::didhitHS = false;
					Globals::missedshots++;
				}
			}
		}
	}

	if (Menu::Window.RageBotTab.FlipAA.GetState())
	{
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			IClientEntity* pLocal = hackManager.pLocal();
			if (Interfaces::Engine->GetPlayerForUserID(deadfag) == Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) != Interfaces::Engine->GetLocalPlayer())
			{
				flipAA = true;

			}
			else
			{
				flipAA = false;
			}
		}
	}

	if (Menu::Window.MiscTab.TrashTalk.GetState()) {
		if (strcmp(Event->GetName(), "round_end") == 0) {
			RoundInfo = true;
			Delay = GetTickCount();
		}
		else if ((strcmp(Event->GetName(), "game_newmap") == 0) || (strcmp(Event->GetName(), "game_start") == 0) || (strcmp(Event->GetName(), "game_end") == 0)) {
			Kills = 0;
			Kills2 = 0;
		}
		else if (strcmp(Event->GetName(), "player_connect") == 0) {
			int PlayerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("userid"));

			player_info_t PlayerInfo;
			if (Interfaces::Engine->GetPlayerInfo(PlayerID, &PlayerInfo)) {
				char Buffer[128];
				sprintf(Buffer, "say Hello, %s.", PlayerInfo.name);
				Interfaces::Engine->ClientCmd(Buffer);
			}
		}
		else if ((strcmp(Event->GetName(), "player_death") == 0) || (strcmp(Event->GetName(), "entity_killed") == 0)) {
			int PlayerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("userid"));
			int AttackerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("attacker"));

			player_info_t PlayerInfo;
			player_info_t AttackerInfo;

			if (Interfaces::Engine->GetPlayerInfo(PlayerID, &PlayerInfo) &&
				Interfaces::Engine->GetPlayerInfo(AttackerID, &AttackerInfo)) {

				if (PlayerID == Interfaces::Engine->GetLocalPlayer())
					Kills = 0;

				if (PlayerID != Interfaces::Engine->GetLocalPlayer() && AttackerID == Interfaces::Engine->GetLocalPlayer()) {
					Kills++;
					Kills2++;

					char Buffer[128];

					switch (Kills) {
					case 1: sprintf(Buffer, "say  %s when you will rq? ", PlayerInfo.name); break;
					case 2:	sprintf(Buffer, "say  %s refund your shit cheat ", PlayerInfo.name); break;
					case 3:	sprintf(Buffer, "say  1 ", PlayerInfo.name); break;
					case 4:	sprintf(Buffer, "say  1 nerd ", PlayerInfo.name); break;
					case 5:	sprintf(Buffer, "say  why is this so ez ", PlayerInfo.name); break;
					case 6:	sprintf(Buffer, "say   %s did you pay for that?  ", PlayerInfo.name); break;
					case 7:	sprintf(Buffer, "say  f12'd ", PlayerInfo.name); break;
					case 8: sprintf(Buffer, "say  refund ", PlayerInfo.name); break;
					case 9: sprintf(Buffer, "say ez ", PlayerInfo.name); break;
					case 10: sprintf(Buffer, "say who are you? 1'd btw ", PlayerInfo.name); break;
					case 11: sprintf(Buffer, "say sit you nonamer", PlayerInfo.name); break;
					case 12: sprintf(Buffer, "say media incoming ", PlayerInfo.name); break;
					case 13: sprintf(Buffer, "say are you even cheating? ", PlayerInfo.name); break;
					case 14: sprintf(Buffer, "say  %s when you will rq? ", PlayerInfo.name); break;
					case 15: sprintf(Buffer, "say  %s refund your shit cheat ", PlayerInfo.name); break;
					case 16: sprintf(Buffer, "say  1 ", PlayerInfo.name); break;
					case 17:	sprintf(Buffer, "say  1 nerd ", PlayerInfo.name); break;
					case 18:	sprintf(Buffer, "say  why is this so ez ", PlayerInfo.name); break;
					case 19:	sprintf(Buffer, "say  %s did you pay for that? ", PlayerInfo.name); break;
					case 20:	sprintf(Buffer, "say  f12'd ", PlayerInfo.name); break;
					case 21: sprintf(Buffer, "say  refund ", PlayerInfo.name); break;
					case 22: sprintf(Buffer, "say ez ", PlayerInfo.name); break;
					case 23: sprintf(Buffer, "say who are you? 1'd btw ", PlayerInfo.name); break;
					case 24: sprintf(Buffer, "say sit you nonamer", PlayerInfo.name); break;
					case 25: sprintf(Buffer, "say media incoming ", PlayerInfo.name); break;
					case 26: sprintf(Buffer, "say are you even cheating? ", PlayerInfo.name); break;
					case 27: sprintf(Buffer, "say  %s when you will rq? ", PlayerInfo.name); break;
					case 28:	sprintf(Buffer, "say  %s refund your shit cheat ", PlayerInfo.name); break;
					case 29:	sprintf(Buffer, "say  1 ", PlayerInfo.name); break;
					case 30:	sprintf(Buffer, "say  1 nerd ", PlayerInfo.name); break;
					case 31:	sprintf(Buffer, "say  why is this so ez ", PlayerInfo.name); break;
					case 32:	sprintf(Buffer, "say  %s did you pay for that?  ", PlayerInfo.name); break;
					case 33:	sprintf(Buffer, "say  f12'd ", PlayerInfo.name); break;
					case 34: sprintf(Buffer, "say  refund ", PlayerInfo.name); break;
					case 35: sprintf(Buffer, "say ez ", PlayerInfo.name); break;
					case 36: sprintf(Buffer, "say who are you? 1'd btw ", PlayerInfo.name); break;
					case 37: sprintf(Buffer, "say sit you nonamer", PlayerInfo.name); break;
					case 38: sprintf(Buffer, "say media incoming ", PlayerInfo.name); break;
					case 39: sprintf(Buffer, "say are you even cheating? ", PlayerInfo.name); break;
					case 40: sprintf(Buffer, "say  %s when you will rq? ", PlayerInfo.name); break;
					case 41:	sprintf(Buffer, "say  %s refund your shit cheat ", PlayerInfo.name); break;
					case 42:	sprintf(Buffer, "say  1 ", PlayerInfo.name); break;
					case 43:	sprintf(Buffer, "say  1 nerd ", PlayerInfo.name); break;
					case 44:	sprintf(Buffer, "say  why is this so ez ", PlayerInfo.name); break;
					case 45:	sprintf(Buffer, "say  sit down  ", PlayerInfo.name); break;
					case 46:	sprintf(Buffer, "say  f12'd ", PlayerInfo.name); break;
					case 47: sprintf(Buffer, "say  refund ", PlayerInfo.name); break;
					case 48: sprintf(Buffer, "say ez ", PlayerInfo.name); break;
					case 49: sprintf(Buffer, "say who are you? 1'd btw ", PlayerInfo.name); break;
					case 50: sprintf(Buffer, "say sit you nonamer", PlayerInfo.name); break;
					case 51: sprintf(Buffer, "say media incoming ", PlayerInfo.name); break;
					case 52: sprintf(Buffer, "say are you even cheating? ", PlayerInfo.name); break;
					default: sprintf(Buffer, "1 ", PlayerInfo.name); break;
					}

					Interfaces::Engine->ClientCmd(Buffer);
				}
			}
		}
	}
	return oFireEventClientSide(ECX, Event);
}

void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	bool DontDraw = false;

	const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
	IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();
		int WeaponsChams = Menu::Window.VisualsTab.WeaponChams.GetState();
		if (ChamsStyle != 0 && Menu::Window.VisualsTab.FiltersPlayers.GetState() && strstr(ModelName, "models/player"))
		{
			if (pModelEntity == nullptr)
				return;

			if (pLocal && (!Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

				IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
				if (pModelEntity)
				{
					IClientEntity *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
					if (local)
					{
						if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 && pModelEntity->GetTeamNum() != local->GetTeamNum())
						{
							float alpha = 1.f;

							if (pModelEntity->HasGunGameImmunity())
								alpha = 0.5f;

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.ColorsTab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.ColorsTab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.ColorsTab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.ColorsTab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.ColorsTab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.ColorsTab.CTChamsB.GetValue() / 255.f;
							}

							if (Menu::Window.VisualsTab.ChamsVisibleOnly.GetState())
							{
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(0);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}
							else
							{
								Interfaces::RenderView->SetColorModulation(flColor);
								Interfaces::RenderView->SetBlend(1);
								Interfaces::ModelRender->ForcedMaterialOverride(covered);
								oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
							}

							if (pModelEntity->GetTeamNum() == 2)
							{
								flColor[0] = Menu::Window.ColorsTab.TChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.ColorsTab.TChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.ColorsTab.TChamsB.GetValue() / 255.f;
							}
							else
							{
								flColor[0] = Menu::Window.ColorsTab.CTChamsR.GetValue() / 255.f;
								flColor[1] = Menu::Window.ColorsTab.CTChamsG.GetValue() / 255.f;
								flColor[2] = Menu::Window.ColorsTab.CTChamsB.GetValue() / 255.f;
							}

							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
						else
						{
							color.SetColor(255, 255, 255, 255);
							ForceMaterial(color, open);
						}
					}
				}
			}
		}
		else if (HandsStyle != 0 && strstr(ModelName, "arms"))
		{
			if (HandsStyle == 1)
			{
				DontDraw = true;
			}
			else if (HandsStyle == 2)
			{
				Interfaces::RenderView->SetBlend(0.3);
			}

			else if (HandsStyle == 3)
			{
				flColor[0] = 10.f / 255.f;
				flColor[1] = 10.f / 255.f;
				flColor[2] = 10.f / 255.f;

				Interfaces::RenderView->SetColorModulation(flColor);
				static IMaterial* wire = CreateMaterial(true, false, true);
				IMaterial *handy = wire;
				handy->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				Interfaces::ModelRender->ForcedMaterialOverride(handy);
			}

			else if (HandsStyle == 4)
			{
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				if (pLocal)
				{
					if (pLocal->IsAlive())
					{
						int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(240, 30, 35, alpha);
						else
							color.SetColor(63, 72, 205, alpha);

						ForceMaterial(color, covered);
						oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

						if (pLocal->GetTeamNum() == 2)
							color.SetColor(247, 180, 20, alpha);
						else
							color.SetColor(32, 180, 57, alpha);
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
					}

					ForceMaterial(color, open);
				}
			}
			else
			{
				static int counter = 0;
				static float colors[3] = { 1.f, 0.f, 0.f };

				if (colors[counter] >= 1.0f)
				{
					colors[counter] = 1.0f;
					counter += 1;
					if (counter > 2)
						counter = 0;
				}
				else
				{
					int prev = counter - 1;
					if (prev < 0) prev = 2;
					colors[prev] -= 0.05f;
					colors[counter] += 0.05f;
				}

				Interfaces::RenderView->SetColorModulation(colors);
				Interfaces::RenderView->SetBlend(0.5);
				Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
			}
		}
		else if (WeaponsChams != 0 && strstr(ModelName, "models/weapons/v_") && !strstr(ModelName, "arms"))
		{
			IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
			IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
			if (pLocal)
			{
				if (pLocal->IsAlive())
				{
					int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(220, 220, 220, alpha);
					else
						color.SetColor(220, 220, 220, alpha);

					ForceMaterial(color, covered);
					oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

					if (pLocal->GetTeamNum() == 2)
						color.SetColor(220, 220, 220, alpha);
					else
						color.SetColor(220, 220, 220, alpha);
				}
				else
				{
					color.SetColor(255, 255, 255, 255);
				}

				ForceMaterial(color, open);
			}
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}

BYTE bMoveData[0x200];
void Prediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{
	if (Interfaces::MoveHelper && Menu::Window.RageBotTab.EnginePrediction.GetState() && LocalPlayer->IsAlive())
	{
		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		Interfaces::Prediction1->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::Prediction1->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;
	}
}

int RandomInt(int min, int max)
{
	return rand() % max + min;
}

bool bGlovesNeedUpdate;
void ApplyCustomGloves(IClientEntity* pLocal)
{
	if (Menu::Window.ColorsTab.EnableGloves.GetState())
	{
		if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
			return;

		if (bGlovesNeedUpdate || !pLocal->IsAlive())
		{
			DWORD* hMyWearables = (DWORD*)((size_t)pLocal + 0x2EF4);

			if (!Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF))
			{
				for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
				{
					if (pClass->m_ClassID != (int)CSGOClassID::CEconWearable)
						continue;

					int iEntry = (Interfaces::EntList->GetHighestEntityIndex() + 1);
					int	iSerial = RandomInt(0x0, 0xFFF);

					pClass->m_pCreateFn(iEntry, iSerial);
					hMyWearables[0] = iEntry | (iSerial << 16);

					break;
				}
			}

			player_info_t LocalPlayerInfo;
			Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalPlayerInfo);

			CBaseCombatWeapon* glovestochange = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF);

			if (!glovestochange)
				return;


			switch (Menu::Window.ColorsTab.GloveModel.GetIndex())
			{
			case 1:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5027;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
				break;
			}
			case 2:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5032;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
				break;
			}
			case 3:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5031;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
				break;
			}
			case 4:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5030;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
				break;
			}
			case 5:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5033;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
				break;
			}
			case 6:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5034;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
				break;
			}
			default:
				break;
			}

			switch (Menu::Window.ColorsTab.GloveSkin.GetIndex())
			{
			case 0:
				break;
			case 1:
				*glovestochange->FallbackPaintKit() = 10007;
				break;
			case 2:
				*glovestochange->FallbackPaintKit() = 10006;
				break;
			case 3:
				*glovestochange->FallbackPaintKit() = 10039;
				break;
			case 4:
				*glovestochange->FallbackPaintKit() = 10008;
				break;
			case 5:
				*glovestochange->FallbackPaintKit() = 10021;
				break;
			case 6:
				*glovestochange->FallbackPaintKit() = 10036;
				break;
			case 7:
				*glovestochange->FallbackPaintKit() = 10009;
				break;
			case 8:
				*glovestochange->FallbackPaintKit() = 10010;
				break;
			case 9:
				*glovestochange->FallbackPaintKit() = 10016;
				break;
			case 10:
				*glovestochange->FallbackPaintKit() = 10013;
				break;
			case 11:
				*glovestochange->FallbackPaintKit() = 10040;
				break;
			case 12:
				*glovestochange->FallbackPaintKit() = 10015;
				break;
			case 13:
				*glovestochange->FallbackPaintKit() = 10037;
				break;
			case 14:
				*glovestochange->FallbackPaintKit() = 10038;
				break;
			case 15:
				*glovestochange->FallbackPaintKit() = 10018;
				break;
			case 16:
				*glovestochange->FallbackPaintKit() = 10019;
				break;
			case 17:
				*glovestochange->FallbackPaintKit() = 10026;
				break;
			case 18:
				*glovestochange->FallbackPaintKit() = 10028;
				break;
			case 19:
				*glovestochange->FallbackPaintKit() = 10027;
				break;
			case 20:
				*glovestochange->FallbackPaintKit() = 10024;
				break;
			case 21:
				*glovestochange->FallbackPaintKit() = 10033;
				break;
			case 22:
				*glovestochange->FallbackPaintKit() = 10034;
				break;
			case 23:
				*glovestochange->FallbackPaintKit() = 10035;
				break;
			case 24:
				*glovestochange->FallbackPaintKit() = 10030;
				break;
			}

			*glovestochange->m_AttributeManager()->m_Item()->ItemIDHigh() = -1;
			*glovestochange->FallbackWear() = 0.001f;
			*glovestochange->m_AttributeManager()->m_Item()->AccountID() = LocalPlayerInfo.xuidlow;


			glovestochange->PreDataUpdate(0);
			bGlovesNeedUpdate = false;
		}
	}
}

void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IClientEntity* pEntity;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{

		if (pLocal->IsAlive())
		{
			if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
				*(Vector*)((DWORD)pLocal + 0x31C8) = LastAngleAA;
		}

		if (Menu::Window.MiscTab.OtherThirdperson.GetState())
		{
			static bool rekt = false;
			if (!rekt)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				rekt = true;
			}
		}

		static bool rekt1 = false;
		if (Menu::Window.MiscTab.OtherThirdperson.GetState() && pLocal->IsAlive() && pLocal->IsScoped() == 0)
		{
			if (!rekt1)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				rekt1 = true;
			}
		}
		else if (!Menu::Window.MiscTab.OtherThirdperson.GetState())
		{
			rekt1 = false;
		}

		static bool rekt = false;
		if (!Menu::Window.MiscTab.OtherThirdperson.GetState() || pLocal->IsAlive() == 0)
		{
			if (!rekt)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
				rekt = true;
			}
		}
		else if (Menu::Window.MiscTab.OtherThirdperson.GetState() || pLocal->IsAlive() == 0)
		{
			rekt = false;
		}

		static bool meme = false;
		if (Menu::Window.MiscTab.OtherThirdperson.GetState() && pLocal->IsScoped() == 0)
		{
			if (!meme)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				meme = true;
			}
		}
		else if (pLocal->IsScoped())
		{
			meme = false;
		}

		static bool kek = false;
		if (Menu::Window.MiscTab.OtherThirdperson.GetState() && pLocal->IsAlive())
		{
			if (!kek)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				kek = true;
			}
		}
		else if (pLocal->IsAlive() == 0)
		{
			kek = false;
		}
	}

	if (Menu::Window.RageBotTab.PVSFix.GetState())
	{
		for (int i = 1; i < Interfaces::Globals->maxClients; i++)
		{
			if (i == Interfaces::Engine->GetLocalPlayer()) continue;
			IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
			if (!pCurEntity) continue;
			*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
			*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
		}
	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		ResolverSetup::GetInst().FSN(pEntity, curStage);
		int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
		int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ApplyCustomGloves(pLocal);

				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						if (sWeapon.find("models/weapons", 0) != std::string::npos)
							continue;

						if (sWeapon.find("c4_planted", 0) != std::string::npos)
							continue;

						if (sWeapon.find("thrown", 0) != std::string::npos)
							continue;

						if (sWeapon.find("smokegrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("flashbang", 0) != std::string::npos)
							continue;

						if (sWeapon.find("fraggrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("molotov", 0) != std::string::npos)
							continue;

						if (sWeapon.find("decoy", 0) != std::string::npos)
							continue;

						if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("ied", 0) != std::string::npos)
							continue;

						if (sWeapon.find("w_eq_", 0) != std::string::npos)
							continue;

						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();

						if (Menu::Window.SkinchangerTab.SkinEnable.GetState())
						{
							int Model = Menu::Window.SkinchangerTab.KnifeModel.GetIndex();
							int M41S = Menu::Window.SkinchangerTab.M41SSkin.GetIndex();
							int M4A4 = Menu::Window.SkinchangerTab.M4A4Skin.GetIndex();
							int AK47 = Menu::Window.SkinchangerTab.AK47Skin.GetIndex();
							int AWP = Menu::Window.SkinchangerTab.AWPSkin.GetIndex();
							int GLOCK = Menu::Window.SkinchangerTab.GLOCKSkin.GetIndex();
							int USPS = Menu::Window.SkinchangerTab.USPSSkin.GetIndex();
							int DEAGLE = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
							int FIVE7 = Menu::Window.SkinchangerTab.FIVESEVENSkin.GetIndex();
							int AUG = Menu::Window.SkinchangerTab.AUGSkin.GetIndex();
							int FAMAS = Menu::Window.SkinchangerTab.FAMASSkin.GetIndex();
							int G3SG1 = Menu::Window.SkinchangerTab.G3SG1Skin.GetIndex();
							int Galil = Menu::Window.SkinchangerTab.GALILSkin.GetIndex();
							int M249 = Menu::Window.SkinchangerTab.M249Skin.GetIndex();
							int MAC10 = Menu::Window.SkinchangerTab.MAC10Skin.GetIndex();
							int P90 = Menu::Window.SkinchangerTab.P90Skin.GetIndex();
							int UMP45 = Menu::Window.SkinchangerTab.UMP45Skin.GetIndex();
							int XM1014 = Menu::Window.SkinchangerTab.XM1014Skin.GetIndex();
							int BIZON = Menu::Window.SkinchangerTab.BIZONSkin.GetIndex();
							int MAG7 = Menu::Window.SkinchangerTab.MAG7Skin.GetIndex();
							int NEGEV = Menu::Window.SkinchangerTab.NEGEVSkin.GetIndex();
							int SAWEDOFF = Menu::Window.SkinchangerTab.SAWEDOFFSkin.GetIndex();
							int TEC9 = Menu::Window.SkinchangerTab.TECNINESkin.GetIndex();
							int P2000 = Menu::Window.SkinchangerTab.P2000Skin.GetIndex();
							int MP7 = Menu::Window.SkinchangerTab.MP7Skin.GetIndex();
							int MP9 = Menu::Window.SkinchangerTab.MP9Skin.GetIndex();
							int NOVA = Menu::Window.SkinchangerTab.NOVASkin.GetIndex();
							int P250 = Menu::Window.SkinchangerTab.P250Skin.GetIndex();
							int SCAR20 = Menu::Window.SkinchangerTab.SCAR20Skin.GetIndex();
							int SG553 = Menu::Window.SkinchangerTab.SG553Skin.GetIndex();
							int SSG08 = Menu::Window.SkinchangerTab.SSG08Skin.GetIndex();
							int Magnum = Menu::Window.SkinchangerTab.RevolverSkin.GetIndex();
							int Duals = Menu::Window.SkinchangerTab.DUALSSkin.GetIndex();

							if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CKnife)
							{
								if (Menu::Window.SkinchangerTab.SkinName.getText().length() > 1)
								{
									auto pCustomName = MakePtr(char*, pWeapon, 0x301C);
									strcpy_s(pCustomName, 32, Menu::Window.SkinchangerTab.SkinName.getText().c_str());
								}
							}

							if (Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != NULL && Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != "")
							{
								int st = atoi(Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str());

								if (Menu::Window.SkinchangerTab.StatTrakEnable.GetState())
									*pWeapon->FallbackStatTrak() = st;
							}

							int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();

							switch (weapon)
							{
							case 7: // AK47 
							{
								switch (AK47)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 341;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 14;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 172;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 180;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 394;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 300;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 226;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 282;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 302;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 316;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 340;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 380;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 656;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 456;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 474;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 490;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 506;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 524;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 600;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 639;
									break;
								default:
									break;
								}
							}
							break;
							case 16: // M4A4
							{
								switch (M4A4)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 155;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 187;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 255;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 309;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 215;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 336;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 384;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 400;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 449;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 471;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 480;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 512;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 533;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 588;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 632;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 664;
									break;
								default:
									break;
								}
							}
							break;
							case 60:
							{
								switch (M41S)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 430;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 644;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 254;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 189;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 301;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 257;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 321;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 326;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 360;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 383;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 440;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 445;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 497;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 548;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 587;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 631;
									break;
								default:
									break;
								}
							}
							break;
							case 9:
							{
								switch (AWP)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 174;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 344;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 84;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 640;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 51;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 181;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 259;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 395;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 212;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 227;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 251;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 279;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 662;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 446;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 451;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 475;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 525;
									break;
								default:
									break;
								}
							}
							break;
							case 61:
							{
								switch (USPS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 183;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 339;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 221;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 653;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 277;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 290;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 313;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 318;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 332;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 364;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 454;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 489;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 504;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 540;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 637;
									break;
								default:
									break;
								}
							}
							break;
							case 4:
							{
								switch (GLOCK)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 586;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 437;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 48;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 353;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 532;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 381;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 367;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 607;
									break;
								default:
									break;
								}
							}
							break;
							case 1: // Deagle
							{
								switch (DEAGLE)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 645;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 527;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 469;
									break;
								default:
									break;
								}
							}
							break;
							case 2: //duals
							{
								switch (Duals)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 491;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 447;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 220;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 261;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 396;
									break;
								default:
									break;
								}
							}
							break;
							case 3: // Five Seven
							{
								switch (FIVE7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 352;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 510;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 530;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 427;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 274;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 660;
									break;
								default:
									break;
								}
							}
							break;
							case 8: // AUG
							{
								switch (AUG)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 9;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 280;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 305;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 375;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 444;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 455;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 507;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 541;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 601;
									break;
								default:
									break;
								}
							}
							break;
							case 10: // Famas
							{
								switch (FAMAS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 429;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 154;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 178;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 194;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 244;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 218;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 260;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 288;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 371;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 477;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 492;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 529;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 604;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 626;
									break;
								default:
									break;
								}
							}
							break;
							case 11: // G3SG1
							{
								switch (G3SG1)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 195;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 229;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 465;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 382;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 438;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 493;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 511;
									break;
								default:
									break;
								}
							}
							break;
							case 13: // Galil
							{
								switch (Galil)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 428;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 494;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 379;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 460;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 398;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 546;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 478;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 661;
									break;
								default:
									break;
								}
							}
							break;
							case 14: // M249
							{
								switch (M249)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 401;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 452;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 472;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 496;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 547;
									break;
								default:
									break;
								}
							}
							break;
							case 17: // Mac 10
							{
								switch (MAC10)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 433;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 188;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 337;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 284;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 310;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 333;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 343;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 372;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 402;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 498;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 534;
									break;
								default:
									break;
								}
							}
							break;
							case 19: // P90
							{
								switch (P90)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 67;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 156;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 516;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 182;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 359;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 20;
									break;
								default:
									break;
								}
							}
							break;
							case 24: // UMP-45
							{
								switch (UMP45)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 556;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 441;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 436;
									break;
								default:
									break;
								}
							}
							break;
							case 25: // XM1014
							{
								switch (XM1014)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 393;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 521;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 505;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 314;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 407;
									break;
								default:
									break;
								}
							}
							break;
							case 26: // Bizon
							{
								switch (BIZON)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 13;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 267;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 542;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 349;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 508;
									break;
								default:
									break;
								}
							}
							break;
							case 27: // Mag 7
							{
								switch (MAG7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 462;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 431;
									break;
								default:
									break;
								}
							}
							break;
							case 28: // Negev
							{
								switch (NEGEV)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 432;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 317;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 355;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 369;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 483;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 514;
									break;
								default:
									break;
								}
							}
							break;
							case 29: // Sawed Off
							{
								switch (SAWEDOFF)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 405;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 256;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 638;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 390;
									break;
								default:
									break;
								}
							}
							break;
							case 30: // Tec 9
							{
								switch (TEC9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 463;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 303;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 520;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 374;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 614;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 555;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 179;
									break;
								default:
									break;
								}
							}
							break;
							case 32: // P2000
							{
								switch (P2000)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 485;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 184;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 211;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 389;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 591;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								default:
									break;
								}
							}
							break;
							case 33: // MP7
							{
								switch (MP7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 481;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 536;
									break;
								default:
									break;
								}
							}
							break;
							case 34: // MP9
							{
								switch (MP9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 482;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								default:
									break;
								}
							}
							break;
							case 35: // Nova
							{
								switch (NOVA)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 166;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 62;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 107;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 158;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 170;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 191;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 214;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 225;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 263;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 286;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 299;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 356;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 450;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 484;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 537;
									break;
								default:
									break;
								}
							}
							break;
							case 36: // P250
							{
								switch (P250)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 168;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 162;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 258;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 551;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 271;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 295;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 358;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 388;
									break;
								default:
									break;
								}
							}
							break;
							case 38: // Scar 20
							{
								switch (SCAR20)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 165;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 196;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 232;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 391;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 597;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 312;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 406;
									break;
									break;
								}
							}
							break;
							case 39: // SG553
							{
								switch (SG553)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 410;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 347;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 287;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 363;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 378;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 487;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 519;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 553;
									break;
								default:
									break;
								}
							}
							break;
							case 40: // SSG08
							{
								switch (SSG08)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 624;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 222;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 554;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 319;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 361;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 503;
									break;
								default:
									break;
								}
							}
							break;
							case 64: // Revolver
							{
								switch (Magnum)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 522;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 12;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 523;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 595;
									break;
								default:
									break;
								}
							}
							break;
							default:
								break;
							}

							if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
							{
								auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
								if (Menu::Window.SkinchangerTab.KnifeName.getText().length() > 1)
								{
									strcpy_s(pCustomName1, 32, Menu::Window.SkinchangerTab.KnifeName.getText().c_str());
								}

								if (Model == 0) // Bayonet
								{
									*pWeapon->ModelIndex() = iBayonet; 
									*pWeapon->ViewModelIndex() = iBayonet;
									*pWeapon->WorldModelIndex() = iBayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 1) // Bowie Knife
								{
									*pWeapon->ModelIndex() = iBowie; 
									*pWeapon->ViewModelIndex() = iBowie;
									*pWeapon->WorldModelIndex() = iBowie + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 2) // Butterfly Knife
								{
									*pWeapon->ModelIndex() = iButterfly;
									*pWeapon->ViewModelIndex() = iButterfly;
									*pWeapon->WorldModelIndex() = iButterfly + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 3) // Falchion Knife
								{
									*pWeapon->ModelIndex() = iFalchion;
									*pWeapon->ViewModelIndex() = iFalchion;
									*pWeapon->WorldModelIndex() = iFalchion + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 4) // Flip Knife
								{
									*pWeapon->ModelIndex() = iFlip;
									*pWeapon->ViewModelIndex() = iFlip;
									*pWeapon->WorldModelIndex() = iFlip + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 5) // Gut Knife
								{
									*pWeapon->ModelIndex() = iGut; 
									*pWeapon->ViewModelIndex() = iGut;
									*pWeapon->WorldModelIndex() = iGut + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 6) // Huntsman Knife
								{
									*pWeapon->ModelIndex() = iHuntsman; 
									*pWeapon->ViewModelIndex() = iHuntsman;
									*pWeapon->WorldModelIndex() = iHuntsman + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 7) // Karambit
								{
									*pWeapon->ModelIndex() = iKarambit; 
									*pWeapon->ViewModelIndex() = iKarambit;
									*pWeapon->WorldModelIndex() = iKarambit + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 8) // M9 Bayonet
								{
									*pWeapon->ModelIndex() = iM9Bayonet; 
									*pWeapon->ViewModelIndex() = iM9Bayonet;
									*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 10) // Shadow Daggers
								{
									*pWeapon->ModelIndex() = iDagger; 
									*pWeapon->ViewModelIndex() = iDagger;
									*pWeapon->WorldModelIndex() = iDagger + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.SkinchangerTab.KnifeSkin.getText().c_str());
								}
							}

							*pWeapon->OwnerXuidLow() = 0;
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						}
					}
				}

			}
		}
	}

	oFrameStageNotify(curStage);
}

void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (pSetup->fov = 90)
				pSetup->fov = Menu::Window.VisualsTab.OtherFOV.GetValue();
		}

		oOverrideView(ecx, edx, pSetup);
	}

}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
		fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class eqvuibv {
public:
	string yqyltduddupfdqt;
	int haytluvuow;
	bool tognoplrvlkecv;
	eqvuibv();
	string ntlcmmuolutlsbnfipfsy(double nyitincw);
	double okdefckzetqydjaulvutmi(int qlatvmktcyhyxzp, string djqapixzci, bool zsnopawmbqvj, double gqyylpeletffik, bool nfkiguoewpzgjn, bool efqbmciq, double qwyrmnczinak, double bdnngotxmuh, int fhbycyeabt);
	double qmuhsxmmabmerab(int mhxahif, double iedagdiwvkuzyp, bool oegtiudg);
	int trluncprgsuvsbyinnpyzkuuh(string ziradhvjiguvtf, bool sodfi, bool rmgdthvsppojw, double dxgtsutouugzx, bool hbrcgyjxmmba, double ccruyywiwoe, string xxdqphmbiannhqq);
	bool mbfynqwmfvj();

protected:
	int hjtuhjxrarxrj;
	double ltyznhc;
	int uxsnitqzwxuxor;
	int azxdxlvmroagtkv;

	bool kbxawtlewmqkruthuf(double zkcmn, bool rdowhjgaz, bool rdypcwcjitqbqb, string qwmamvgc, int zokcjp, string uaibbn, string ydugeiohsduoc, bool orcohhdofppmqvf, string rikwukojwwcrz);
	string jguutibwfnjnacmegjaucu(double ixilak, double gbkpq, bool zvmjimyg, string jusehfick, int puvlezaqfkeqrxg, string thawpulab, string uuqaipav, string ulsxji, bool zhkfuc, bool foyhctay);
	string kqdqdimekezrsqjyve(string mvkgqowrdlkldle, int sczddtmgll, double vcewcg, int ziodwpgba, string xeabilpbsfmwyaf, bool dcechhiqevqs, bool bweuboa, string ynurang, int mjdvq, string rmkxxklcsxqwv);
	int tbassabelvwwrmekuxnuoyd(double ymrzegvherqso, int jfysdan, string gcgvui, int avnqoc, string dmcfexnhhp, bool opqzvadobt);
	double cejcqavbjhopyukxgv(double ibldulngtrmoo, double nlqnpxo);
	double lgmxuzopdafleswhfax(bool agufeqbqlnekrdy, string vfgzgrm, double flgqx, int irlaxda, string kdvjuwxhaqjtkjj, string vrdohmwbwlyep, double ryqiyspdt, bool ygixe, bool emgcrpnccgs);

private:
	bool agrfenvgetddelw;

	void johtjfvygjpibcmexyk(double xheubuqvmq, bool bkzwbunaqvey, string ecgsfhhdrsehyvf, bool ktzifuffbd, double zqauigcyl);
	void pvuoyvysbmzlgpghswi(double jjzgh, string fcjyfthad);
	void qxtbujsusd();
	int balililxstjdwd(double rcaehqgrfrtkxb, double alzonrnpftnp, double klqwictq, int igfvwcuzvsowsud, bool ddxerkwfwoki, double wbhfi);

};


void eqvuibv::johtjfvygjpibcmexyk(double xheubuqvmq, bool bkzwbunaqvey, string ecgsfhhdrsehyvf, bool ktzifuffbd, double zqauigcyl) {
	double gvhkhjuf = 31935;
	bool xezmjoi = false;
	string igzlumjz = "cgjqgnuwubkkewcnkgcwporscjtbosnsdjmhntsimznjdpkhkfgbkkdjpppsojgpqqdwzbgopzzcmj";
	double iaixvvf = 20374;
	double gfgubfpj = 5141;
	double ociva = 24614;
	if (5141 != 5141) {
		int jexiyn;
		for (jexiyn = 59; jexiyn > 0; jexiyn--) {
			continue;
		}
	}
	if (string("cgjqgnuwubkkewcnkgcwporscjtbosnsdjmhntsimznjdpkhkfgbkkdjpppsojgpqqdwzbgopzzcmj") != string("cgjqgnuwubkkewcnkgcwporscjtbosnsdjmhntsimznjdpkhkfgbkkdjpppsojgpqqdwzbgopzzcmj")) {
		int era;
		for (era = 67; era > 0; era--) {
			continue;
		}
	}

}

void eqvuibv::pvuoyvysbmzlgpghswi(double jjzgh, string fcjyfthad) {
	bool yzuftzgmcyveyp = false;
	int zcykowvzc = 1042;
	bool endxg = false;
	bool urvwkgfvtjz = false;
	double qderr = 29372;
	if (1042 == 1042) {
		int cddro;
		for (cddro = 13; cddro > 0; cddro--) {
			continue;
		}
	}
	if (1042 == 1042) {
		int rwxk;
		for (rwxk = 92; rwxk > 0; rwxk--) {
			continue;
		}
	}
	if (false != false) {
		int jwxfofdfqz;
		for (jwxfofdfqz = 100; jwxfofdfqz > 0; jwxfofdfqz--) {
			continue;
		}
	}
	if (false == false) {
		int tazundajm;
		for (tazundajm = 71; tazundajm > 0; tazundajm--) {
			continue;
		}
	}
	if (false == false) {
		int af;
		for (af = 83; af > 0; af--) {
			continue;
		}
	}

}

void eqvuibv::qxtbujsusd() {
	int yhpdwtjtfv = 2496;
	string etcktsovuppm = "yjflenydfedjuwwpcnamffzweoyhbwuhtbnx";
	string kwemkcw = "abdaoiraavokrxyazqlvaefprzrgvyggvyytmqtptnrvwkvyypsehxrl";
	int peigjgzezszl = 2796;
	int tulisbaio = 3093;
	bool mguvxjxd = true;
	double ayqso = 4662;
	int eslyj = 3809;
	if (true == true) {
		int upqiwvr;
		for (upqiwvr = 30; upqiwvr > 0; upqiwvr--) {
			continue;
		}
	}
	if (string("yjflenydfedjuwwpcnamffzweoyhbwuhtbnx") == string("yjflenydfedjuwwpcnamffzweoyhbwuhtbnx")) {
		int fg;
		for (fg = 26; fg > 0; fg--) {
			continue;
		}
	}
	if (2796 != 2796) {
		int zqboxdgek;
		for (zqboxdgek = 94; zqboxdgek > 0; zqboxdgek--) {
			continue;
		}
	}
	if (2496 != 2496) {
		int jyjrvzgzwr;
		for (jyjrvzgzwr = 21; jyjrvzgzwr > 0; jyjrvzgzwr--) {
			continue;
		}
	}
	if (3093 == 3093) {
		int ipofnlq;
		for (ipofnlq = 52; ipofnlq > 0; ipofnlq--) {
			continue;
		}
	}

}

int eqvuibv::balililxstjdwd(double rcaehqgrfrtkxb, double alzonrnpftnp, double klqwictq, int igfvwcuzvsowsud, bool ddxerkwfwoki, double wbhfi) {
	string euvgvhi = "kjjwsrllzgbrexgfldaauzq";
	return 31725;
}

bool eqvuibv::kbxawtlewmqkruthuf(double zkcmn, bool rdowhjgaz, bool rdypcwcjitqbqb, string qwmamvgc, int zokcjp, string uaibbn, string ydugeiohsduoc, bool orcohhdofppmqvf, string rikwukojwwcrz) {
	double xgdwiadecr = 4349;
	if (4349 == 4349) {
		int dgdwcbyhc;
		for (dgdwcbyhc = 18; dgdwcbyhc > 0; dgdwcbyhc--) {
			continue;
		}
	}
	if (4349 == 4349) {
		int eeuqtgkdgv;
		for (eeuqtgkdgv = 47; eeuqtgkdgv > 0; eeuqtgkdgv--) {
			continue;
		}
	}
	if (4349 != 4349) {
		int bpam;
		for (bpam = 89; bpam > 0; bpam--) {
			continue;
		}
	}
	return true;
}

string eqvuibv::jguutibwfnjnacmegjaucu(double ixilak, double gbkpq, bool zvmjimyg, string jusehfick, int puvlezaqfkeqrxg, string thawpulab, string uuqaipav, string ulsxji, bool zhkfuc, bool foyhctay) {
	bool edujabmvwl = true;
	bool sreqsu = true;
	double ovnpwjzfxl = 34452;
	int ynnfkodgaleo = 7455;
	int uckojnh = 4122;
	string xrsqkjxaryfh = "elcrourv";
	bool mpuxr = false;
	int fsskooyghxaldq = 2163;
	return string("cxdhnscnehohlfrp");
}

string eqvuibv::kqdqdimekezrsqjyve(string mvkgqowrdlkldle, int sczddtmgll, double vcewcg, int ziodwpgba, string xeabilpbsfmwyaf, bool dcechhiqevqs, bool bweuboa, string ynurang, int mjdvq, string rmkxxklcsxqwv) {
	int fqphdqi = 105;
	int xdagbo = 117;
	bool pmqlbmnkwrinaem = false;
	bool vumrdnkevvcsu = false;
	string wmhnftyn = "vzjqpcoyvpbksvcmxdfdlmbuihvfxwywmdvijevbivfwvpqoxaqtqjygx";
	bool vfjyptnckqnist = false;
	bool kcsszubmjynjow = false;
	double uigajsbmpzl = 512;
	string uzcvxsta = "saujandjwuigavbbsmaoxteqlzwvsidweylyulkwsngvx";
	string swuyfxjex = "bbpi";
	if (string("bbpi") != string("bbpi")) {
		int gdlerjq;
		for (gdlerjq = 3; gdlerjq > 0; gdlerjq--) {
			continue;
		}
	}
	if (false == false) {
		int azf;
		for (azf = 48; azf > 0; azf--) {
			continue;
		}
	}
	return string("cgrqlxspxltnbukybee");
}

int eqvuibv::tbassabelvwwrmekuxnuoyd(double ymrzegvherqso, int jfysdan, string gcgvui, int avnqoc, string dmcfexnhhp, bool opqzvadobt) {
	return 2096;
}

double eqvuibv::cejcqavbjhopyukxgv(double ibldulngtrmoo, double nlqnpxo) {
	string mqxdqmlimzyuebx = "pulmhaaqlrjweviyuopqykiibcaswdfwfziksfkcxwmkmegbppkctrmwsi";
	int tfgpmwom = 3726;
	bool lamny = true;
	if (true == true) {
		int qdzymw;
		for (qdzymw = 39; qdzymw > 0; qdzymw--) {
			continue;
		}
	}
	return 16233;
}

double eqvuibv::lgmxuzopdafleswhfax(bool agufeqbqlnekrdy, string vfgzgrm, double flgqx, int irlaxda, string kdvjuwxhaqjtkjj, string vrdohmwbwlyep, double ryqiyspdt, bool ygixe, bool emgcrpnccgs) {
	string rkgefmhcpbweob = "wzlxdoehpttyvxkbpkfmpkpzrsycicsabtvujibnpfzulsylkokqusuosj";
	int rvtniyv = 2669;
	string wbfrqd = "ylayzqeuoenuzobhnxgsfsodgaltqbmodjubumpqsppnptccdprnq";
	bool bvxmbvkuy = true;
	string zuernamhi = "tvjqsiwmahsv";
	return 78649;
}

string eqvuibv::ntlcmmuolutlsbnfipfsy(double nyitincw) {
	int cterzpyoyagbc = 2517;
	string tdmmjpnzsq = "hkirmjfvavblsmpqwnknpo";
	bool xopmydkqwvn = true;
	string wzxabnlgmffxx = "vlzotkalrdxdixusspjyhmsoqzjcsvvqcvsbtjdlo";
	bool frrisfsoww = true;
	bool jvatoo = false;
	int ubuivuqnz = 1733;
	if (string("hkirmjfvavblsmpqwnknpo") == string("hkirmjfvavblsmpqwnknpo")) {
		int ktlznipkg;
		for (ktlznipkg = 42; ktlznipkg > 0; ktlznipkg--) {
			continue;
		}
	}
	if (true == true) {
		int dmokbmcfz;
		for (dmokbmcfz = 8; dmokbmcfz > 0; dmokbmcfz--) {
			continue;
		}
	}
	if (2517 == 2517) {
		int uupine;
		for (uupine = 21; uupine > 0; uupine--) {
			continue;
		}
	}
	return string("vixwoffdpidbrglw");
}

double eqvuibv::okdefckzetqydjaulvutmi(int qlatvmktcyhyxzp, string djqapixzci, bool zsnopawmbqvj, double gqyylpeletffik, bool nfkiguoewpzgjn, bool efqbmciq, double qwyrmnczinak, double bdnngotxmuh, int fhbycyeabt) {
	return 94677;
}

double eqvuibv::qmuhsxmmabmerab(int mhxahif, double iedagdiwvkuzyp, bool oegtiudg) {
	bool gguzzibfl = false;
	string emlqalpcyvkn = "ailmhxxvskwpvovqlmrwiyjpedfdvafpuhgpv";
	if (false == false) {
		int iuvinvj;
		for (iuvinvj = 90; iuvinvj > 0; iuvinvj--) {
			continue;
		}
	}
	return 21428;
}

int eqvuibv::trluncprgsuvsbyinnpyzkuuh(string ziradhvjiguvtf, bool sodfi, bool rmgdthvsppojw, double dxgtsutouugzx, bool hbrcgyjxmmba, double ccruyywiwoe, string xxdqphmbiannhqq) {
	double xegrbrzalru = 13009;
	string htfrwllxeca = "aystwuooayncxibtozafiefcclyuemxzqzinyhriplrmeiyqfmpxqomompwxetdrsusttbovvovinypc";
	double ivwcoal = 21082;
	string pjmlyfkswtyg = "";
	double tkubxmztvs = 421;
	if (string("") != string("")) {
		int aoiicnshu;
		for (aoiicnshu = 43; aoiicnshu > 0; aoiicnshu--) {
			continue;
		}
	}
	if (13009 == 13009) {
		int owfdnycdw;
		for (owfdnycdw = 9; owfdnycdw > 0; owfdnycdw--) {
			continue;
		}
	}
	if (string("aystwuooayncxibtozafiefcclyuemxzqzinyhriplrmeiyqfmpxqomompwxetdrsusttbovvovinypc") != string("aystwuooayncxibtozafiefcclyuemxzqzinyhriplrmeiyqfmpxqomompwxetdrsusttbovvovinypc")) {
		int clcma;
		for (clcma = 88; clcma > 0; clcma--) {
			continue;
		}
	}
	if (21082 != 21082) {
		int hndprqbd;
		for (hndprqbd = 59; hndprqbd > 0; hndprqbd--) {
			continue;
		}
	}
	if (21082 == 21082) {
		int ohqxokikx;
		for (ohqxokikx = 62; ohqxokikx > 0; ohqxokikx--) {
			continue;
		}
	}
	return 19909;
}

bool eqvuibv::mbfynqwmfvj() {
	double ktaqvfzdcz = 24291;
	double wpyoxudztg = 24229;
	bool pbxyyqdvbygqhzf = false;
	bool jajcyi = true;
	double rasucbycvflcym = 9049;
	bool pzfqbslpoolgl = true;
	if (24229 == 24229) {
		int qgzua;
		for (qgzua = 58; qgzua > 0; qgzua--) {
			continue;
		}
	}
	if (24291 != 24291) {
		int hsg;
		for (hsg = 77; hsg > 0; hsg--) {
			continue;
		}
	}
	if (24291 != 24291) {
		int ufpszsf;
		for (ufpszsf = 20; ufpszsf > 0; ufpszsf--) {
			continue;
		}
	}
	if (9049 != 9049) {
		int qilgvmuvk;
		for (qilgvmuvk = 97; qilgvmuvk > 0; qilgvmuvk--) {
			continue;
		}
	}
	return false;
}

eqvuibv::eqvuibv() {
	this->ntlcmmuolutlsbnfipfsy(73607);
	this->okdefckzetqydjaulvutmi(8277, string("alqwmpgchoounyycuverhzaluxuswjywfolyztwz"), false, 13017, true, false, 38087, 22618, 1558);
	this->qmuhsxmmabmerab(4433, 66416, false);
	this->trluncprgsuvsbyinnpyzkuuh(string("hujtpiakgziualaodhswbeynilczrxhyxkbckefiodjghlgkqhxiyfjxmsddwnqnsagqppogrchjqhroynkfr"), true, true, 22024, true, 20293, string("ktbfwqgyvzdrejekgqhydbwdtvcjcnwfjmaddfctuxunhdwrtpdqyelzunkdudvk"));
	this->mbfynqwmfvj();
	this->kbxawtlewmqkruthuf(485, false, true, string("dxflpresniwvjmbckucbbjdieradqieveznpnjvgerduilinpcclxqmdieqpportumoduneyc"), 455, string("tf"), string("zbxycmfxvsmqskilarbmcshwtlqksebldjxjnbiyrryrfworuflzteyidvpmcuyyhbwfnbkkybra"), true, string("dhriydsvoammeznmsxsgs"));
	this->jguutibwfnjnacmegjaucu(18641, 373, false, string("duddswawfsxbamdcnxgovnkckmuql"), 6049, string("jboozykjwsfdvukyvknlqhnektstvvmtkalwmjxbhaovokdsxuxp"), string("mczlenhrpetmggpenubewxyjfrigdjmcewwjcmrsbmblfynnebulkciwsbspsdvebrhaupuzpeiekfhqppeqsqztseeckvomclea"), string("ywwltghlscoazgxjkdhootrlnsjrivlhuazyyinacfs"), true, false);
	this->kqdqdimekezrsqjyve(string("rjrkydfntvfhvnkkdbztohemtpjwcethradfsimonkooo69oooo4ooawuyabquoxmptwzckuvbwkwjccaowiysedfr"), 1266, 48077, 2663, string("vwlivtcjkvfxznjjutpoatexnrgajhttztobbnrdhlkewzcimawsjvoodcnuvzgrluicrgpgsjsewyeigybvogws"), false, false, string("oqcpppsuhxotewlosixduwwpdnpujmzxfxfzgjexuwwvdqipkcmumdfedoagqoakmjjvymgbausxirsjh"), 2441, string("qafticuroywrnzcuauooesfkxrqzfrqthylnlbrfdnatmwfydzhdnheavibyibmqrpwbpkymculhvnmwjzqlayahhzbih"));
	this->tbassabelvwwrmekuxnuoyd(62360, 9709, string("aksozfqqccamilchbedghuhkirjyzkhzchahxnuulnuzlbkfewzkxgoffcyyitwephhchqcbxfrghuk"), 2707, string("ydwugfqdgsmrbidxgslenuiegfulzdlfcfalndjpyrmpheosfysapooqsfkbucmytoijnnmbfkygruadlldtvgdlmikqxqqeusv"), false);
	this->cejcqavbjhopyukxgv(20584, 39422);
	this->lgmxuzopdafleswhfax(false, string("gympghlrftwbhotbyewwqamcuwwpzdhoujyxfwjeqcwpyvkjaqffuojdrccfh"), 31014, 261, string("jjblgeymqeohgwupmbpq"), string("nrwllcrfyichgkbclcuuqyzhxbikphcyehdxosihgsyjwcguxtabsjvtzxzawmy"), 17619, false, true);
	this->johtjfvygjpibcmexyk(24617, true, string("ohmfgcjksnushhwwcmkobnyneadaqknerhtdgjpnkuolkaiqjollvrwxjeafxadlpwhmvyyzzbtjzzvkacqvonvxpkqbs"), false, 18488);
	this->pvuoyvysbmzlgpghswi(2185, string("sqmuiheccgfvdlgbpgssmejxpxhbfnujhytxr"));
	this->qxtbujsusd();
	this->balililxstjdwd(5483, 12033, 26780, 3352, false, 50939);
}
