#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" 
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 933
#define WINDOW_HEIGHT 700

AvozWindow Menu::Window;

void SaveLegitCallbk()
{
	switch (Menu::Window.ColorsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "legit.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "rage.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "mmhvh.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "casual.cfg");
		break;
	}
}

void LoadLegitCallbk()
{
	switch (Menu::Window.ColorsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "legit.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "rage.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "mmhvh.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "casual.cfg");
		break;
	}
}

void SaveRageCallbk()
{
	GUI.SaveWindowState(&Menu::Window, "rage.cfg");
}

void LoadRageCallbk()
{
	GUI.LoadWindowState(&Menu::Window, "rage.cfg");
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void GlovesApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void AvozWindow::Setup()
{
	SetPosition(350, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("                                                                       SteamXYZ");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);
	RegisterTab(&SkinchangerTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();
	SkinchangerTab.Setup();

#pragma region Bottom Buttons

	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveLegitCallbk);
	SaveButton.SetPosition(405, Client.bottom - 610);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadLegitCallbk);
	LoadButton.SetPosition(580, Client.bottom - 610);

	ColorsTab.RegisterControl(&SaveButton);
	ColorsTab.RegisterControl(&ConfigBox);
	ColorsTab.RegisterControl(&LoadButton);

#pragma endregion
}

void CRageBotTab::Setup()
{
	SetTitle("a");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, -15);
	RegisterControl(&Active);

#pragma region Aimbot

	AimbotGroup.SetPosition(16, 11);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(400, 280);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(5.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AWPAtBody.SetFileId("aim_awpatbody");
	AimbotGroup.PlaceLabledControl("AWP at Body", this, &AWPAtBody);

	AutoRevolver.SetFileId("aim_autorevolver");
	AimbotGroup.PlaceLabledControl("Auto Revolver", this, &AutoRevolver);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aim Step", this, &AimbotAimStep);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);

	AimbotStopKey.SetFileId("aim_stop");
	AimbotGroup.PlaceLabledControl("Stop Aim", this, &AimbotStopKey);
#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(16, 297);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(376, 310);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Closest To Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Lowest Health");
	TargetSelection.AddItem("Threat");
	TargetSelection.AddItem("Next Shot");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Friendly Fire", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Foots");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off"); 
	TargetHitscan.AddItem("Low"); 
	TargetHitscan.AddItem("Medium"); 
	TargetHitscan.AddItem("High"); 
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	PVSFix.SetFileId("tgt_pvsfix");
	TargetGroup.PlaceLabledControl("PVS Fix", this, &PVSFix);

	EnginePrediction.SetFileId("tgt_enginepred");
	TargetGroup.PlaceLabledControl("Engine Prediction", this, &EnginePrediction);

	TargetMultipoint.SetFileId("tgt_multipoint");
	TargetGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	TargetPointscale.SetFileId("tgt_pointscale");
	TargetPointscale.SetBoundaries(0.f, 10.f);
	TargetPointscale.SetValue(5.f);
	TargetGroup.PlaceLabledControl("Aim Height", this, &TargetPointscale);
#pragma endregion Targetting controls 

#pragma region Accuracy
	AccuracyGroup.SetPosition(408, 11);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(360, 270); 
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Remove Recoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 99.f);
	AccuracyMinimumDamage.SetValue(1.f);
	AccuracyGroup.PlaceLabledControl("Autowall Damage", this, &AccuracyMinimumDamage);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	LowerbodyFix.SetFileId("acc_lowerbodyfix");
	AccuracyGroup.PlaceLabledControl("LBY Correction", this, &LowerbodyFix);

	AdvancedResolver.SetFileId("acc_advancedresolver");
	AccuracyGroup.PlaceLabledControl("Pitch Correction", this, &AdvancedResolver);

	AimbotResolver.SetFileId("acc_aaa");
	AimbotResolver.AddItem("Off");
	AimbotResolver.AddItem("Level 1");
	AimbotResolver.AddItem("Level 2");
	AimbotResolver.AddItem("Level 3");
	AccuracyGroup.PlaceLabledControl("Resolver", this, &AimbotResolver);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Body-Aim if HP is lower than X", this, &BaimIfUnderXHealth);

	PreferBodyAim.SetFileId("acc_preferbaim");
	AccuracyGroup.PlaceLabledControl("Prefer Body-Aim", this, &PreferBodyAim);

#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(408, 297); 
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(360, 310);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Halfdown");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("Zero");
	AntiAimPitch.AddItem("Custom");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Fast Spin");
	AntiAimYaw.AddItem("Slow Spin");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("180 Jitter");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Backwards Jitter");
	AntiAimYaw.AddItem("Sideways Switch");
	AntiAimYaw.AddItem("Sideways Right");
	AntiAimYaw.AddItem("Sideways Left");
	AntiAimYaw.AddItem("LBY");
	AntiAimYaw.AddItem("LBY Jitter");
	AntiAimYaw.AddItem("LBY Sideways");
	AntiAimYaw.AddItem("LBY Spin");
	AntiAimYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Real Yaw", this, &AntiAimYaw);

	FakeYaw.SetFileId("fakeaa");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("Fast Spin");
	FakeYaw.AddItem("Slow Spin");
	FakeYaw.AddItem("Jitter");
	FakeYaw.AddItem("180 Jitter");
	FakeYaw.AddItem("Backwards");
	FakeYaw.AddItem("Backwards Jitter");
	FakeYaw.AddItem("Sideways Switch");
	FakeYaw.AddItem("Sideways Right");
	FakeYaw.AddItem("Sideways Left");
	FakeYaw.AddItem("LBY");
	FakeYaw.AddItem("LBY Jitter");
	FakeYaw.AddItem("LBY Sideways");
	FakeYaw.AddItem("LBY Spin");
	FakeYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Fake Yaw", this, &FakeYaw);

	CustomPitch.SetFileId("aa_offset");
	CustomPitch.SetBoundaries(0, 89);
	CustomPitch.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Custom Pitch", this, &CustomPitch);

	AntiAimOffset.SetFileId("aa_offset");
	AntiAimOffset.SetBoundaries(0, 360);
	AntiAimOffset.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Real Yaw", this, &AntiAimOffset);

	AddFakeYaw.SetFileId("aa_offset");
	AddFakeYaw.SetBoundaries(0, 360);
	AddFakeYaw.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Fake Yaw", this, &AddFakeYaw);

	FlipAA.SetFileId("aa_flip");
	AntiAimGroup.PlaceLabledControl("Flip AA on Hit", this, &FlipAA);

	LBY.SetFileId("aa_lby");
	AntiAimGroup.PlaceLabledControl("Anti Resolver", this, &LBY);

	AntiAimEdge.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Wall Detection", this, &AntiAimEdge);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Knife/Nade Anti Aim", this, &AntiAimKnife);

	AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AntiAimTarget);
#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("b");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, -15);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 11);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 170);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl("Smoke Check", this, &AimbotSmokeCheck);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key Bind", this, &AimbotKeyBind);

#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(272, 11);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 170);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl("On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("Key Bind", this, &TriggerKeyBind);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl("Recoil Control", this, &TriggerRecoil);

	TriggerSmokeCheck.SetFileId("trig_smokecheck");
	TriggerGroup.PlaceLabledControl("Smoke Check", this, &TriggerSmokeCheck);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	TriggerGroup.PlaceLabledControl("Delay", this, &TriggerDelay);

#pragma endregion Triggerbot stuff

#pragma region TriggerbotFilter
	TriggerFilterGroup.SetPosition(528, 11);
	TriggerFilterGroup.SetText("Triggerbot Filter");
	TriggerFilterGroup.SetSize(240, 170);
	RegisterControl(&TriggerFilterGroup);

	TriggerHead.SetFileId("trig_head");
	TriggerFilterGroup.PlaceLabledControl("Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	TriggerFilterGroup.PlaceLabledControl("Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	TriggerFilterGroup.PlaceLabledControl("Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	TriggerFilterGroup.PlaceLabledControl("Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	TriggerFilterGroup.PlaceLabledControl("Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	TriggerFilterGroup.PlaceLabledControl("Friendly Fire", this, &TriggerTeammates);

#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 208);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(240, 135);
	RegisterControl(&WeaponMainGroup);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 100.f);
	WeaponMainSpeed.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("Max Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(3.5f);
	WeaponMainGroup.PlaceLabledControl("Field of View", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
	WeaponMainRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("Recoil Control", this, &WeaponMainRecoil);

	WeaponMainAimtime.SetValue(0);
	WeaoponMainStartAimtime.SetValue(0);

#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(272, 208);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 135);
	RegisterControl(&WeaponPistGroup);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multihitbox");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(3.f);
	WeaponPistGroup.PlaceLabledControl("Field of View", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(1.f);
	WeaponPistGroup.PlaceLabledControl("Recoil Control", this, &WeaponPistRecoil);

	WeaponPistAimtime.SetValue(0);
	WeaoponPistStartAimtime.SetValue(0);

#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(528, 208);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 135);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multihitbox");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(1.5f);
	WeaponSnipGroup.PlaceLabledControl("Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(2.f);
	WeaponSnipGroup.PlaceLabledControl("Field of View", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(1.f);
	WeaponSnipGroup.PlaceLabledControl("Recoil Control", this, &WeaponSnipRecoil);

	WeaponSnipAimtime.SetValue(0);
	WeaoponSnipStartAimtime.SetValue(0);

#pragma region MPs
	WeaponMpGroup.SetPosition(16, 365);
	WeaponMpGroup.SetText("MPs");
	WeaponMpGroup.SetSize(240, 136);
	RegisterControl(&WeaponMpGroup);

	WeaponMpHitbox.SetFileId("mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Multihitbox");
	WeaponMpGroup.PlaceLabledControl("Hitbox", this, &WeaponMpHitbox);

	WeaponMpSpeed.SetFileId("mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(1.0f);
	WeaponMpGroup.PlaceLabledControl("Max Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(4.f);
	WeaponMpGroup.PlaceLabledControl("Field of View", this, &WeaponMpFoV);

	WeaponMpRecoil.SetFileId("mps_recoil");
	WeaponMpRecoil.SetBoundaries(0.f, 2.f);
	WeaponMpRecoil.SetValue(1.f);
	WeaponMpGroup.PlaceLabledControl("Recoil Control", this, &WeaponMpRecoil);

	WeaponMpAimtime.SetValue(0);
	WeaoponMpStartAimtime.SetValue(0);
#pragma endregion

#pragma region Shotguns
	WeaponShotgunGroup.SetPosition(272, 365);
	WeaponShotgunGroup.SetText("Shotguns");
	WeaponShotgunGroup.SetSize(240, 136);
	RegisterControl(&WeaponShotgunGroup);

	WeaponShotgunHitbox.SetFileId("shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Multihitbox");
	WeaponShotgunGroup.PlaceLabledControl("Hitbox", this, &WeaponShotgunHitbox);

	WeaponShotgunSpeed.SetFileId("shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(1.0f);
	WeaponShotgunGroup.PlaceLabledControl("Max Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(3.f);
	WeaponShotgunGroup.PlaceLabledControl("Field of View", this, &WeaponShotgunFoV);

	WeaponShotgunRecoil.SetFileId("snip_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.f, 2.f);
	WeaponShotgunRecoil.SetValue(1.f);
	WeaponShotgunGroup.PlaceLabledControl("Recoil Control", this, &WeaponShotgunRecoil);

	WeaponShotgunAimtime.SetValue(0);
	WeaoponShotgunStartAimtime.SetValue(0);

#pragma endregion

#pragma region Machineguns
	WeaponMGGroup.SetPosition(528, 365);
	WeaponMGGroup.SetText("Heavy");
	WeaponMGGroup.SetSize(240, 136);
	RegisterControl(&WeaponMGGroup);

	WeaponMGHitbox.SetFileId("mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Multihitbox");
	WeaponMGGroup.PlaceLabledControl("Hitbox", this, &WeaponMGHitbox);

	WeaponMGSpeed.SetFileId("mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(1.0f);
	WeaponMGGroup.PlaceLabledControl("Max Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(4.f);
	WeaponMGGroup.PlaceLabledControl("Field of View", this, &WeaponMGFoV);

	WeaponMGRecoil.SetFileId("mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.f, 2.f);
	WeaponMGRecoil.SetValue(1.f);
	WeaponMGGroup.PlaceLabledControl("Recoil Control", this, &WeaponMGRecoil);

	WeaponMGAimtime.SetValue(0);
	WeaoponMGStartAimtime.SetValue(0);

#pragma endregion
}

void CVisualTab::Setup()
{
	SetTitle("c");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, -15);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Options");
	OptionsGroup.SetPosition(16, 16);
	OptionsGroup.SetSize(193, 505);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("Off");
	OptionsBox.AddItem("2D Box");
	OptionsBox.AddItem("Filled");
	OptionsBox.AddItem("Corners");
	OptionsGroup.PlaceLabledControl("Box", this, &OptionsBox);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsWeapon.AddItem("Off");
	OptionsWeapon.AddItem("Text");
	OptionsWeapon.AddItem("Icon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsHealth.SetFileId("opt_hp");
	OptionsGroup.PlaceLabledControl("Health", this, &OptionsHealth);

	OptionsArmor.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl("Armor", this, &OptionsArmor);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl("Chams", this, &OptionsChams);

	ChamsVisibleOnly.SetFileId("opt_chamsvisonly");
	OptionsGroup.PlaceLabledControl("Legit Chams", this, &ChamsVisibleOnly);

	WeaponChams.SetFileId("opt_weaponchams");
	OptionsGroup.PlaceLabledControl("Weapon Chams", this, &WeaponChams);

	OtherHitmarker.SetFileId("otr_hitmarker");
	OptionsGroup.PlaceLabledControl("Hitmarker", this, &OtherHitmarker);

	HitmarkerSound.SetFileId("otr_hitmarkersound");
	OptionsGroup.PlaceLabledControl("Hitmarker Sound", this, &HitmarkerSound);

	Barrels.SetFileId("opt_barrels");
	OptionsGroup.PlaceLabledControl("Trace Lines", this, &Barrels);

	Distance.SetFileId("opt_distance");
	OptionsGroup.PlaceLabledControl("Distance", this, &Distance);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsAimSpot.SetFileId("opt_aimspot");
	OptionsGroup.PlaceLabledControl("Head Cross", this, &OptionsAimSpot);
	
	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Tab Rank Reveal", this, &OptionsCompRank);

	Money.SetFileId("opt_money");
	OptionsGroup.PlaceLabledControl("Money", this, &Money);

	EntityGlow.SetFileId("opt_entityglow");
	OptionsGroup.PlaceLabledControl("Entity Glow", this, &EntityGlow);

	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceLabledControl("Glow", this, &OptionsGlow);

	GlowZ.SetFileId("opt_glowz");
	GlowZ.SetBoundaries(0.f, 255.f);
	GlowZ.SetValue(255);
	OptionsGroup.PlaceLabledControl("Glow Alpha", this, &GlowZ);

	OptionsDefusing.SetFileId("opt_defusing");
	OptionsGroup.PlaceLabledControl("Defusing", this, &OptionsDefusing);

	IsScoped.SetFileId("opt_isscoped");
	OptionsGroup.PlaceLabledControl("Is Scoped", this, &IsScoped);

	HasDefuser.SetFileId("opt_hasdefuser");
	OptionsGroup.PlaceLabledControl("Has Defuser", this, &HasDefuser);

#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(225, 16);
	FiltersGroup.SetSize(193, 175);
	RegisterControl(&FiltersGroup);

	FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceLabledControl("All", this, &FiltersAll);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersNades.SetFileId("ftr_nades");
	FiltersGroup.PlaceLabledControl("Nades", this, &FiltersNades);

	FiltersChickens.SetFileId("ftr_chickens");
	FiltersGroup.PlaceLabledControl("Chickens", this, &FiltersChickens);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);

#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(434, 16);
	OtherGroup.SetSize(334, 435);
	RegisterControl(&OtherGroup);

	OtherCrosshair.SetFileId("otr_crosshair");
	OtherCrosshair.AddItem("Off");
	OtherCrosshair.AddItem("Recoil");
	OtherCrosshair.AddItem("Spread");
	OtherGroup.PlaceLabledControl("Override Crosshair", this, &OtherCrosshair);

	SniperCrosshair.SetFileId("otr_snipercrosshair");
	OtherGroup.PlaceLabledControl("Sniper Crosshair", this, &SniperCrosshair);

	GrenadeTrace.SetFileId("otr_grenadetrace");
	OtherGroup.PlaceLabledControl("Grenade Trajectory", this, &GrenadeTrace);

	NightMode.SetFileId("otr_nightmode");
	OtherGroup.PlaceLabledControl("Night Mode", this, &NightMode);

	NightSky.SetFileId("otr_nightsky");
	OtherGroup.PlaceLabledControl("Night Sky", this, &NightSky);

	SpecList.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Show Spectators", this, &SpecList);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("Remove Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	OtherGroup.PlaceLabledControl("Remove Flash", this, &OtherNoFlash);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	OtherGroup.PlaceLabledControl("Remove Smoke", this, &OtherNoSmoke);

	OtherNoScope.SetFileId("otr_noscope");
	OtherGroup.PlaceLabledControl("Remove Scope", this, &OtherNoScope);

	OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	DisablePostProcess.SetFileId("otr_disablepostprocess");
	OtherGroup.PlaceLabledControl("Disable Post Process", this, &DisablePostProcess);

	Clock.SetFileId("otr_clock");
	OtherGroup.PlaceLabledControl("Clock", this, &Clock);

	Watermark.SetFileId("otr_watermark");
	OtherGroup.PlaceLabledControl("Watermark", this, &Watermark);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Wireframe");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow");
	OtherGroup.PlaceLabledControl("Hands", this, &OtherNoHands);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Viewmodel FOV Changer", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("Field of View Changer", this, &OtherFOV);

	AmbientRed.SetFileId("otr_ambientred");
	AmbientRed.SetBoundaries(0.f, 10.f);
	AmbientRed.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Red", this, &AmbientRed);

	AmbientGreen.SetFileId("otr_ambientgreen");
	AmbientGreen.SetBoundaries(0.f, 10.f);
	AmbientGreen.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Green", this, &AmbientGreen);

	AmbientBlue.SetFileId("otr_ambientblue");
	AmbientBlue.SetBoundaries(0.f, 10.f);
	AmbientBlue.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Blue", this, &AmbientBlue);

#pragma endregion Setting up the Other controls
}

void CMiscTab::Setup()
{
	SetTitle("d");


#pragma region Other
	OtherGroup.SetPosition(408, -15);
	OtherGroup.SetSize(360, 422);
	OtherGroup.SetText("Other");
	RegisterControl(&OtherGroup);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl("Force Thirdperson", this, &OtherThirdperson);

	OtherCircleStrafe.SetFileId("otr_circlestrafe");
	OtherGroup.PlaceLabledControl("Circular Strafe", this, &OtherCircleStrafe);

	AutoPistol.SetFileId("otr_autopistol");
	OtherGroup.PlaceLabledControl("Auto Pistol", this, &AutoPistol);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl("Auto Jump", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);

	NameChanger.SetFileId("otr_spam");
	NameChanger.AddItem("Off");
	NameChanger.AddItem("Name Spam");
	NameChanger.AddItem("No Name");
	NameChanger.AddItem("Name Steal");
	OtherGroup.PlaceLabledControl("Name Changer", this, &NameChanger);

	ChatSpam.SetFileId("otr_chatspam");
	OtherGroup.PlaceLabledControl("Chat Spam", this, &ChatSpam);

	TrashTalk.SetFileId("otr_trashtalk");
	OtherGroup.PlaceLabledControl("Flame Enemy", this, &TrashTalk);

	OtherTeamChat.SetFileId("otr_teamchat");
	OtherGroup.PlaceLabledControl("Team Chat Only", this, &OtherTeamChat);

	ClanTag.SetFileId("otr_clantag");
	OtherGroup.PlaceLabledControl("Spam Clantag", this, &ClanTag);

	OtherChatDelay.SetFileId("otr_chatdelay");
	OtherChatDelay.SetBoundaries(0.1, 3.0);
	OtherChatDelay.SetValue(0.5);
	OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);

	OtherCircleButton.SetFileId("otr_circlestrafebutton");
	OtherGroup.PlaceLabledControl("Circle Key", this, &OtherCircleButton);

	OtherSlowMotion.SetFileId("otr_slowmotionbutton");
	OtherGroup.PlaceLabledControl("Slow Motion", this, &OtherSlowMotion);

	FakeWalk.SetFileId("otr_fakewalk");
	OtherGroup.PlaceLabledControl("Fake Walk", this, &FakeWalk);

#pragma endregion other random options

#pragma region FakeLag
	FakeLagGroup.SetPosition(16, -15);
	FakeLagGroup.SetSize(360, 105);
	FakeLagGroup.SetText("Fakelag");
	RegisterControl(&FakeLagGroup);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl("Enable", this, &FakeLagEnable);

	FakeLagChoke.SetFileId("fakelag_choke");
	FakeLagChoke.SetBoundaries(0, 18);
	FakeLagChoke.SetValue(0);
	FakeLagGroup.PlaceLabledControl("Choke Factor", this, &FakeLagChoke);

	FakeLagTyp.SetFileId("fakelag_typ");
	FakeLagTyp.AddItem("Off");
	FakeLagTyp.AddItem("Normal");
	FakeLagTyp.AddItem("Adaptive");
	FakeLagTyp.AddItem("Normal - In Air");
	FakeLagTyp.AddItem("Adaptive - In Air");
	FakeLagTyp.AddItem("Normal - Break Lagcomp");
	FakeLagGroup.PlaceOtherControl("Mode", this, &FakeLagTyp);

#pragma endregion fakelag shit

#pragma region Buybot
	BuyBotGroup.SetPosition(16, 105);
	BuyBotGroup.SetSize(360, 150);
	BuyBotGroup.SetText("Buybot");
	RegisterControl(&BuyBotGroup);

	EnableBuyBot.SetFileId("bb_enable");
	BuyBotGroup.PlaceLabledControl("Enable", this, &EnableBuyBot);

	BuyBot.SetFileId("buybot");
	BuyBot.AddItem("Off");
	BuyBot.AddItem("Ak/M4");
	BuyBot.AddItem("AWP");
	BuyBot.AddItem("Auto + Duals");
	BuyBotGroup.PlaceLabledControl("Buy Bot", this, &BuyBot);

	BuyBotGrenades.SetFileId("buybot_grenades");
	BuyBotGrenades.AddItem("Off");
	BuyBotGrenades.AddItem("Flash + Smoke + HE");
	BuyBotGrenades.AddItem("Flash + Smoke + HE + Molo");
	BuyBotGroup.PlaceLabledControl("Buy Grenades", this, &BuyBotGrenades);

	BuyBotKevlar.SetFileId("buybot_kevlar");
	BuyBotGroup.PlaceLabledControl("Buy Armor", this, &BuyBotKevlar);

	BuyBotDefuser.SetFileId("buybot_defuser");
	BuyBotGroup.PlaceLabledControl("Buy Defuse Kit", this, &BuyBotDefuser);

#pragma endregion Buybot
}

void CColorTab::Setup()
{
	SetTitle("f");
#pragma region Visual Colors
	ColorsGroup.SetPosition(16, -15);
	ColorsGroup.SetText("Colors");
	ColorsGroup.SetSize(360, 545);
	RegisterControl(&ColorsGroup);

	CTColorVisR.SetFileId("ct_color_vis_r");
	CTColorVisR.SetBoundaries(0.f, 255.f);
	CTColorVisR.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("CT visible red", this, &CTColorVisR);

	CTColorVisG.SetFileId("ct_color_vis_g");
	CTColorVisG.SetBoundaries(0.f, 255.f);
	CTColorVisG.SetValue(230.f);
	ColorsGroup.PlaceLabledControl("CT visible green", this, &CTColorVisG);

	CTColorVisB.SetFileId("ct_color_vis_b");
	CTColorVisB.SetBoundaries(0.f, 255.f);
	CTColorVisB.SetValue(15.f);
	ColorsGroup.PlaceLabledControl("CT visible blue", this, &CTColorVisB);

	CTColorNoVisR.SetFileId("ct_color_no_vis_r");
	CTColorNoVisR.SetBoundaries(0.f, 255.f);
	CTColorNoVisR.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("CT not visible red", this, &CTColorNoVisR);

	CTColorNoVisG.SetFileId("ct_color_no_vis_g");
	CTColorNoVisG.SetBoundaries(0.f, 255.f);
	CTColorNoVisG.SetValue(235.f);
	ColorsGroup.PlaceLabledControl("CT Not Visible Green", this, &CTColorNoVisG);

	CTColorNoVisB.SetFileId("ct_color_no_vis_b");
	CTColorNoVisB.SetBoundaries(0.f, 255.f);
	CTColorNoVisB.SetValue(10.f);
	ColorsGroup.PlaceLabledControl("CT Not Visible Blue", this, &CTColorNoVisB);

	TColorVisR.SetFileId("t_color_vis_r");
	TColorVisR.SetBoundaries(0.f, 255.f);
	TColorVisR.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("T Visible Red", this, &TColorVisR);

	TColorVisG.SetFileId("t_color_vis_g");
	TColorVisG.SetBoundaries(0.f, 255.f);
	TColorVisG.SetValue(50.f);
	ColorsGroup.PlaceLabledControl("T Visible Green", this, &TColorVisG);

	TColorVisB.SetFileId("t_color_vis_b");
	TColorVisB.SetBoundaries(0.f, 255.f);
	TColorVisB.SetValue(220.f);
	ColorsGroup.PlaceLabledControl("T Visible Blue", this, &TColorVisB);

	TColorNoVisR.SetFileId("t_color_no_vis_r");
	TColorNoVisR.SetBoundaries(0.f, 255.f);
	TColorNoVisR.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("T Not Visible Red", this, &TColorNoVisR);

	TColorNoVisG.SetFileId("t_color_no_vis_g");
	TColorNoVisG.SetBoundaries(0.f, 255.f);
	TColorNoVisG.SetValue(50.f);
	ColorsGroup.PlaceLabledControl("T Not Visible Green", this, &TColorNoVisG);

	TColorNoVisB.SetFileId("t_color_no_vis_b");
	TColorNoVisB.SetBoundaries(0.f, 255.f);
	TColorNoVisB.SetValue(220.f);
	ColorsGroup.PlaceLabledControl("T Not Visible Blue", this, &TColorNoVisB);

	GlowR.SetFileId("GlowR");
	GlowR.SetBoundaries(0.f, 255.f);
	GlowR.SetValue(230.f);
	ColorsGroup.PlaceLabledControl("Glow Red", this, &GlowR);

	GlowG.SetFileId("GlowG");
	GlowG.SetBoundaries(0.f, 255.f);
	GlowG.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("Glow Green", this, &GlowG);

	GlowB.SetFileId("GlowB");
	GlowB.SetBoundaries(0.f, 255.f);
	GlowB.SetValue(0.f);
	ColorsGroup.PlaceLabledControl("Glow Blue", this, &GlowB);

	CTChamsR.SetFileId("ctchamsr");
	CTChamsR.SetBoundaries(0.f, 255.f);
	CTChamsR.SetValue(40.f);
	ColorsGroup.PlaceLabledControl("CT Chams Red", this, &CTChamsR);

	CTChamsG.SetFileId("ctchamsg");
	CTChamsG.SetBoundaries(0.f, 255.f);
	CTChamsG.SetValue(120.f);
	ColorsGroup.PlaceLabledControl("CT Chams Green", this, &CTChamsG);

	CTChamsB.SetFileId("ctchamsb");
	CTChamsB.SetBoundaries(0.f, 255.f);
	CTChamsB.SetValue(170.f);
	ColorsGroup.PlaceLabledControl("CT Chams Blue", this, &CTChamsB);

	TChamsR.SetFileId("tchamsr");
	TChamsR.SetBoundaries(0.f, 255.f);
	TChamsR.SetValue(210.f);
	ColorsGroup.PlaceLabledControl("T Chams Red", this, &TChamsR);

	TChamsG.SetFileId("tchamsg");
	TChamsG.SetBoundaries(0.f, 255.f);
	TChamsG.SetValue(20.f);
	ColorsGroup.PlaceLabledControl("T Chams Green", this, &TChamsG);

	TChamsB.SetFileId("tchamsb");
	TChamsB.SetBoundaries(0.f, 255.f);
	TChamsB.SetValue(70.f);
	ColorsGroup.PlaceLabledControl("T Chams Blue", this, &TChamsB);

	ConfigGroup.SetPosition(400, -15);
	ConfigGroup.SetText("Preset Config");
	ConfigGroup.SetSize(376, 85);
	RegisterControl(&ConfigGroup);

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("legit");
	ConfigBox.AddItem("rage");
	ConfigBox.AddItem("mm hvh");
	ConfigBox.AddItem("casual");
	ConfigGroup.PlaceLabledControl("Config", this, &ConfigBox);
#pragma endregion

#pragma region GloveModel
	Gloves.SetPosition(400, 90);
	Gloves.SetText("Override Gloves");
	Gloves.SetSize(376, 100);
	RegisterControl(&Gloves);

	EnableGloves.SetFileId("gloves_enable");
	Gloves.PlaceLabledControl("Enable Glovechanger", this, &EnableGloves);

	GloveModel.SetFileId("glove_model");
	GloveModel.AddItem("Off");
	GloveModel.AddItem("Bloodhound");
	GloveModel.AddItem("Handwrap");
	GloveModel.AddItem("Driver");
	GloveModel.AddItem("Sport");
	GloveModel.AddItem("Motorcycle");
	GloveModel.AddItem("Specialist");
	Gloves.PlaceLabledControl("Glove Model", this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem("Off");
	GloveSkin.AddItem("Bloodhound - Snakebite");
	GloveSkin.AddItem("Bloodhound - Charred");
	GloveSkin.AddItem("Bloodhound - Guerrilla");
	GloveSkin.AddItem("Bloodhound - Bronzed");
	GloveSkin.AddItem("Handwrap - Slaughter");
	GloveSkin.AddItem("Handwrap - Badlands");
	GloveSkin.AddItem("Handwrap - Leather");
	GloveSkin.AddItem("Handwrap - Spruce DDPAT");
	GloveSkin.AddItem("Driver - Crimson Weave");
	GloveSkin.AddItem("Driver - Lunar Weave");
	GloveSkin.AddItem("Driver - Diamondback");
	GloveSkin.AddItem("Driver - Convoy");
	GloveSkin.AddItem("Sport - Pandoras Box");
	GloveSkin.AddItem("Sport - Hedge Naze");
	GloveSkin.AddItem("Sport - Superconductor");
	GloveSkin.AddItem("Sport - Arid");
	GloveSkin.AddItem("Motorcycle - Spearmint");
	GloveSkin.AddItem("Motorcycle - Cool Mint");
	GloveSkin.AddItem("Motorcycle - Boom");
	GloveSkin.AddItem("Motorcycle - Eclipse");
	GloveSkin.AddItem("Specialist - Crimson Kimono");
	GloveSkin.AddItem("Specialist - Emerald Web");
	GloveSkin.AddItem("Specialist - Foundation");
	GloveSkin.AddItem("Specialist - Forest DDPAT");
	Gloves.PlaceLabledControl("Glove Skin", this, &GloveSkin);

	GlovesApply.SetText("Apply Gloves");
	GlovesApply.SetCallback(GlovesApplyCallbk);
	GlovesApply.SetPosition(407, 200);
	GlovesApply.SetSize(360, 106);
	RegisterControl(&GlovesApply);

#pragma endregion
}

void CSkinchangerTab::Setup()
{
	SetTitle("e");

	SkinActive.SetPosition(16, -15);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("skin_enable");
	SkinEnable.SetPosition(66, -15);
	RegisterControl(&SkinEnable);

#pragma region Knife
	KnifeGroup.SetPosition(16, 11);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(376, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeGroup.PlaceLabledControl("Skin ID", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(408, 11);
	MachinegunsGroup.SetText("Heavy");
	MachinegunsGroup.SetSize(360, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16, 98);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(376, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Oni Taiji");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(408, 98);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(360, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Tranquality");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Quicksilver");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Highwayman");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16, 233);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(376, 200);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Orbit Mk01");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Bloodsport");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hellfire");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Sugar Rush");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG553", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16, 443);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(376, 173);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Virus");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Primal Saber");
	UMP45Skin.AddItem("Minotaurs Labyrinth");
	UMP45Skin.AddItem("Grand Prix");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Judgement of Anubis");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Fuel Rod");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Hot Rod");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(408, 233);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(360, 250);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Wasteland Rebel");
	GLOCKSkin.AddItem("Twillight Galaxy");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Weasel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Dark Water");;
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Oxide Blaze");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Sunset Storm");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Urban Shock");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	RevolverSkin.SetFileId("revolver_skin");
	RevolverSkin.AddItem("Fade");
	RevolverSkin.AddItem("Crimson Web");
	RevolverSkin.AddItem("Amber Fade");
	RevolverSkin.AddItem("Reboot");
	PistolGroup.PlaceLabledControl("Revolver", this, &RevolverSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Hyper Beast");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Nuclear Threat");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Amber Fade");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Cartel");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion

#pragma region Skinsettings
	SkinsettingsGroup.SetPosition(408, 496);
	SkinsettingsGroup.SetText("Customisation");
	SkinsettingsGroup.SetSize(360, 118);
	RegisterControl(&SkinsettingsGroup);

	StatTrakEnable.SetFileId("skin_stattrack");
	SkinsettingsGroup.PlaceLabledControl("Stat Trak", this, &StatTrakEnable);

	StatTrackAmount.SetFileId("skin_stamount");
	SkinsettingsGroup.PlaceLabledControl("Value", this, &StatTrackAmount);

	KnifeName.SetFileId("knife_name");
	SkinsettingsGroup.PlaceLabledControl("Knife Name", this, &KnifeName);

	SkinName.SetFileId("skin_name");
	SkinsettingsGroup.PlaceLabledControl("Skin Name", this, &SkinName);

	SkinApply.SetText("Apply Changes");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(408, 618);
	SkinApply.SetSize(360, 106);
	RegisterControl(&SkinApply);

#pragma endregion

#pragma endregion other random options
}

/*void CPlayersTab::Setup()
{
	SetTitle("PlayerList");

#pragma region PList

	pListGroup.SetPosition(16, 16);
	pListGroup.SetSize(680, 200);
	pListGroup.SetText("Player List");
	pListGroup.SetColumns(2);
	RegisterControl(&pListGroup);

	pListPlayers.SetPosition(26, 46);
	pListPlayers.SetSize(640, 50);
	pListPlayers.SetHeightInItems(20);
	RegisterControl(&pListPlayers);

#pragma endregion

#pragma region Options
	
	OptionsGroup.SetPosition(16, 257);
	OptionsGroup.SetSize(450, 120);
	OptionsGroup.SetText("Player Options");
	RegisterControl(&OptionsGroup);

	OptionsFriendly.SetFileId("pl_friendly");
	OptionsGroup.PlaceLabledControl("Friendly", this, &OptionsFriendly);

	OptionsAimPrio.SetFileId("pl_priority");
	OptionsGroup.PlaceLabledControl("Priority", this, &OptionsAimPrio);

	OptionsCalloutSpam.SetFileId("pl_callout");
	OptionsGroup.PlaceLabledControl("Callout Spam", this, &OptionsCalloutSpam);

#pragma endregion
}

DWORD GetPlayerListIndex(int EntId)
{
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(EntId, &pinfo);

	// Bot
	if (pinfo.guid[0] == 'B' && pinfo.guid[1] == 'O')
	{
		char buf[64]; sprintf_s(buf, "BOT_420%sAY", pinfo.name);
		return CRC32(buf, 64);
	}
	else
	{
		return CRC32(pinfo.guid, 32);
	}
}

bool IsFriendly(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Friendly;
	}

	return false;
}

bool IsAimPrio(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].AimPrio;
	}

	return false;
}

bool IsCalloutTarget(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Callout;
	}

	return false;
}

void UpdatePlayerList()
{
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
	{
		Menu::Window.Playerlist.pListPlayers.ClearItems();

		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity

			player_info_t pinfo;
			if (i != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerInfo(i, &pinfo))
			{
				IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
				int HP = 100; char* Location = "Unknown";
				char *Friendly = " ", *AimPrio = " ";

				DWORD plistId = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());
				if (PlayerList.find(plistId) != PlayerList.end())
				{
					Friendly = PlayerList[plistId].Friendly ? "Friendly" : "";
					AimPrio = PlayerList[plistId].AimPrio ? "AimPrio" : "";
				}

				if (pEntity && !pEntity->IsDormant())
				{
					HP = pEntity->GetHealth();
					Location = pEntity->GetLastPlaceName();
				}

				char nameBuffer[512];
				sprintf_s(nameBuffer, "%-24s %-10s %-10s [%d HP] [Last Seen At %s]", pinfo.name, IsFriendly(i) ? "Friend" : " ", IsAimPrio(i) ? "AimPrio" : " ", HP, Location);
				Menu::Window.Playerlist.pListPlayers.AddItem(nameBuffer, i);

			}

		}

		DWORD meme = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());

		// Have we switched to a different player?
		static int PrevSelectedPlayer = 0;
		if (PrevSelectedPlayer != Menu::Window.Playerlist.pListPlayers.GetValue())
		{
			if (PlayerList.find(meme) != PlayerList.end())
			{
				Menu::Window.Playerlist.OptionsFriendly.SetState(PlayerList[meme].Friendly);
				Menu::Window.Playerlist.OptionsAimPrio.SetState(PlayerList[meme].AimPrio);
				Menu::Window.Playerlist.OptionsCalloutSpam.SetState(PlayerList[meme].Callout);

			}
			else
			{
				Menu::Window.Playerlist.OptionsFriendly.SetState(false);
				Menu::Window.Playerlist.OptionsAimPrio.SetState(false);
				Menu::Window.Playerlist.OptionsCalloutSpam.SetState(false);

			}
		}
		PrevSelectedPlayer = Menu::Window.Playerlist.pListPlayers.GetValue();

		PlayerList[meme].Friendly = Menu::Window.Playerlist.OptionsFriendly.GetState();
		PlayerList[meme].AimPrio = Menu::Window.Playerlist.OptionsAimPrio.GetState();
		PlayerList[meme].Callout = Menu::Window.Playerlist.OptionsCalloutSpam.GetState();
	}
}*/

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
		Window.VisualsTab.FiltersNades.SetState(true);
	}

	GUI.Update();
	GUI.Draw();
	
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class khwqsaw {
public:
	double jxqnrogilljz;
	int jrsottmp;
	string hmqhopculuvqs;
	khwqsaw();
	bool loltqkyfvjzjvyea(int ywvlpl, int tbavizizsqzef, double fedjjzzlexflv, bool ercwbqoq, int wxeyhfxsyrfr, string gfhcrsvvt, double qmisxkkgtow);

protected:
	bool szrckt;
	double vekki;

	bool qiiasacksygzmkzq(int nqomskqmn, string fdups, string szwobe, bool vitvpsblzldgrf, int wdltophvrpkgzgr, bool tinxlvb, string imhfzwmeqjcm, double xkmqdpbposz, string ewjcqxaexzutukb);
	double yzzouvpeetxdfwhmq(string cvvlchobd, int vmlsjizwqr, int pkmhvsjdj, int dkkzjao, int lxslf, string ofuukroimsmjvt, string ulxbn, string tfspykpabmjpvnb);
	string wegyrgnzrvkluey(bool nzekfan, bool magxduxgfjtqi, int aqohglkpopxdsde, int letmuiuucph, bool mpsnbdbxmmvq, double rhfexk, int mqhsrlo, int sfkfkzhwrdlvv, string srojtplogme);
	string hgfvxgrxbiafvpt(bool sjjvntx, string ogpryksypixp, string neiaxmkv, bool cshwskdeotbhsb, double ssnotmljdji, double mbzibl, bool hitlbcxsheop, string efepch, bool vxlyldanshqyd, string hhewmtyrrtv);
	int ngnlgqgscgze(bool fgbxaoxgbvnaw);
	void rmyvdfjvgvhtreywzcjwvrjn(string quaungppeinw, int zrugsfuatxcgf);
	void zlxhbtpunyxdinlpazcpouc(int gqmuuybu, string edaifezs);
	double uiwudaojysee(bool gunjeiplee, string gfuvot, string qcuewvlcwnysyf, double ihqiurnswb, double ogzbeptrxbk, int nfoukjrerrbb, string vigrwugxnisff, double dpalvimkma, bool edbzrpeyv, bool zkboafqjfqhh);
	double ooesikbtezchjtnmdh(bool megoat, double aauscnzx, string wetfqjju);
	int xufufgimrwgzjfrvkspiqtxi(bool drueclkknyhwek, int vbhkomwupu, bool fxcsyxjffsoxerf);

private:
	string ebxwm;

	double dhnaojhmscxeaxssfxok(int mtvdyppgk, bool pyfemjep, string kauaoerdncgfx, double ktjvgvlbr);
	int hldbyofkemcagv(bool trowjbpf, int ynjdbpcbwpy, double nmhjgxalmaye, double exzmllomcp, double guljbhns, double kanqh, bool euqzaijubjmtqrc, int ppjldjvlvrwevhu, string nmsgclnsjj, double kzsijiey);
	void ljrfylrmyo(double ahwzjp, string hhnfsukkpvfm, string pgjwvn, int iqkimqqltlhmkmk, string uvjzxkmaexc, double hrgypj, bool vplzgzmvtvll, double ehyzbfnyjzyqefs);

};


double khwqsaw::dhnaojhmscxeaxssfxok(int mtvdyppgk, bool pyfemjep, string kauaoerdncgfx, double ktjvgvlbr) {
	bool ozlqapvajy = false;
	string ibhbvz = "xogsxinzzgzcyomqyojosagqmaphynkqeisqxvaxqgoxiolexjyhxrgawlcctuwmolnuiwpfnlndxmgypdexwiqzbxljrcznpcl";
	int ixpny = 1369;
	string fzhzb = "wwlermidkxpfdbdhpjpfgiuvmyemu";
	bool ykirrgatdpcil = true;
	string vxybiya = "tcijcdohjkrlzlcsgpniyxpxxnxzzlhtzdtfpxjp";
	string euivk = "yafvdrthdnlsskywtnhznhpmygqjkuznybsiiutfnmlveolhwwruyxpjhqyuyxavwkidjiteussghhiocxdawtmnzlalmdo";
	if (string("tcijcdohjkrlzlcsgpniyxpxxnxzzlhtzdtfpxjp") == string("tcijcdohjkrlzlcsgpniyxpxxnxzzlhtzdtfpxjp")) {
		int vfxsbq;
		for (vfxsbq = 17; vfxsbq > 0; vfxsbq--) {
			continue;
		}
	}
	if (string("tcijcdohjkrlzlcsgpniyxpxxnxzzlhtzdtfpxjp") == string("tcijcdohjkrlzlcsgpniyxpxxnxzzlhtzdtfpxjp")) {
		int qxhcogdtx;
		for (qxhcogdtx = 35; qxhcogdtx > 0; qxhcogdtx--) {
			continue;
		}
	}
	if (string("wwlermidkxpfdbdhpjpfgiuvmyemu") == string("wwlermidkxpfdbdhpjpfgiuvmyemu")) {
		int xubxliey;
		for (xubxliey = 12; xubxliey > 0; xubxliey--) {
			continue;
		}
	}
	if (false == false) {
		int rdxnexf;
		for (rdxnexf = 45; rdxnexf > 0; rdxnexf--) {
			continue;
		}
	}
	return 58531;
}

int khwqsaw::hldbyofkemcagv(bool trowjbpf, int ynjdbpcbwpy, double nmhjgxalmaye, double exzmllomcp, double guljbhns, double kanqh, bool euqzaijubjmtqrc, int ppjldjvlvrwevhu, string nmsgclnsjj, double kzsijiey) {
	bool eapvkxzigzgk = false;
	int ecdeybjeleior = 1635;
	double lwgoyxchatgrsx = 1261;
	int ekmsjc = 1057;
	double guxgxbivrnmu = 20807;
	bool ucsfqzrawdn = false;
	int ncfml = 2588;
	bool wvsnw = false;
	return 78592;
}

void khwqsaw::ljrfylrmyo(double ahwzjp, string hhnfsukkpvfm, string pgjwvn, int iqkimqqltlhmkmk, string uvjzxkmaexc, double hrgypj, bool vplzgzmvtvll, double ehyzbfnyjzyqefs) {
	int fqctxezt = 670;
	bool ecxieuifcec = false;
	string kmrjisoi = "joynjdchxveubzpxpbnpzqvlptgtxlszvqjzmvnldhrwhxpwroullrlebdmnezzznktet";
	if (string("joynjdchxveubzpxpbnpzqvlptgtxlszvqjzmvnldhrwhxpwroullrlebdmnezzznktet") == string("joynjdchxveubzpxpbnpzqvlptgtxlszvqjzmvnldhrwhxpwroullrlebdmnezzznktet")) {
		int hvrwxjan;
		for (hvrwxjan = 10; hvrwxjan > 0; hvrwxjan--) {
			continue;
		}
	}

}

bool khwqsaw::qiiasacksygzmkzq(int nqomskqmn, string fdups, string szwobe, bool vitvpsblzldgrf, int wdltophvrpkgzgr, bool tinxlvb, string imhfzwmeqjcm, double xkmqdpbposz, string ewjcqxaexzutukb) {
	string rlxpgantuk = "sawidwowjdgwpoatldqtciymwirskeaswhldgeruadmvvwxvhtbblkjznsjtastolmmeuxmbjagembgslxlbuzvg";
	string xihlc = "syhdlcxxh";
	double emukfqum = 9678;
	double pqawucvgs = 20672;
	double dwookegljo = 11422;
	string wrmrbna = "koqmfwxeuxmagiywjjyukyqupmkqdnswutlnacdzldojpjzazcnaiwtaxadcnucfkpqznhxgqapemrldrdjg";
	string axymbpkobwrbl = "lir";
	string gmotqrt = "anu";
	if (9678 == 9678) {
		int qjgz;
		for (qjgz = 48; qjgz > 0; qjgz--) {
			continue;
		}
	}
	if (20672 == 20672) {
		int ma;
		for (ma = 89; ma > 0; ma--) {
			continue;
		}
	}
	if (string("anu") == string("anu")) {
		int nnv;
		for (nnv = 74; nnv > 0; nnv--) {
			continue;
		}
	}
	return true;
}

double khwqsaw::yzzouvpeetxdfwhmq(string cvvlchobd, int vmlsjizwqr, int pkmhvsjdj, int dkkzjao, int lxslf, string ofuukroimsmjvt, string ulxbn, string tfspykpabmjpvnb) {
	bool zfwvsq = true;
	double aljbdt = 11474;
	if (11474 == 11474) {
		int vvyayjre;
		for (vvyayjre = 64; vvyayjre > 0; vvyayjre--) {
			continue;
		}
	}
	if (11474 != 11474) {
		int lxlgsltarq;
		for (lxlgsltarq = 100; lxlgsltarq > 0; lxlgsltarq--) {
			continue;
		}
	}
	return 65166;
}

string khwqsaw::wegyrgnzrvkluey(bool nzekfan, bool magxduxgfjtqi, int aqohglkpopxdsde, int letmuiuucph, bool mpsnbdbxmmvq, double rhfexk, int mqhsrlo, int sfkfkzhwrdlvv, string srojtplogme) {
	int ljjfecysgedq = 7218;
	string mwiveharjjuii = "bgs";
	if (string("bgs") != string("bgs")) {
		int guwpllpmwk;
		for (guwpllpmwk = 25; guwpllpmwk > 0; guwpllpmwk--) {
			continue;
		}
	}
	if (string("bgs") != string("bgs")) {
		int plx;
		for (plx = 65; plx > 0; plx--) {
			continue;
		}
	}
	return string("hyzql");
}

string khwqsaw::hgfvxgrxbiafvpt(bool sjjvntx, string ogpryksypixp, string neiaxmkv, bool cshwskdeotbhsb, double ssnotmljdji, double mbzibl, bool hitlbcxsheop, string efepch, bool vxlyldanshqyd, string hhewmtyrrtv) {
	return string("rhclsocryd");
}

int khwqsaw::ngnlgqgscgze(bool fgbxaoxgbvnaw) {
	return 87190;
}

void khwqsaw::rmyvdfjvgvhtreywzcjwvrjn(string quaungppeinw, int zrugsfuatxcgf) {
	int egccecfm = 25;
	int yiwanvcrkpl = 2759;
	if (25 != 25) {
		int tzkzh;
		for (tzkzh = 26; tzkzh > 0; tzkzh--) {
			continue;
		}
	}
	if (2759 != 2759) {
		int wsftwqgxud;
		for (wsftwqgxud = 2; wsftwqgxud > 0; wsftwqgxud--) {
			continue;
		}
	}
	if (2759 == 2759) {
		int tkdvd;
		for (tkdvd = 48; tkdvd > 0; tkdvd--) {
			continue;
		}
	}
	if (2759 != 2759) {
		int ir;
		for (ir = 76; ir > 0; ir--) {
			continue;
		}
	}

}

void khwqsaw::zlxhbtpunyxdinlpazcpouc(int gqmuuybu, string edaifezs) {

}

double khwqsaw::uiwudaojysee(bool gunjeiplee, string gfuvot, string qcuewvlcwnysyf, double ihqiurnswb, double ogzbeptrxbk, int nfoukjrerrbb, string vigrwugxnisff, double dpalvimkma, bool edbzrpeyv, bool zkboafqjfqhh) {
	int uhewedrbn = 99;
	double vzvxlqwfzioh = 12764;
	int flznwifeoruh = 126;
	if (126 == 126) {
		int kjkplkje;
		for (kjkplkje = 0; kjkplkje > 0; kjkplkje--) {
			continue;
		}
	}
	if (126 == 126) {
		int nii;
		for (nii = 13; nii > 0; nii--) {
			continue;
		}
	}
	if (99 == 99) {
		int mvlevrhwx;
		for (mvlevrhwx = 28; mvlevrhwx > 0; mvlevrhwx--) {
			continue;
		}
	}
	if (99 == 99) {
		int exhkxp;
		for (exhkxp = 99; exhkxp > 0; exhkxp--) {
			continue;
		}
	}
	if (126 == 126) {
		int quokhnjz;
		for (quokhnjz = 65; quokhnjz > 0; quokhnjz--) {
			continue;
		}
	}
	return 25010;
}

double khwqsaw::ooesikbtezchjtnmdh(bool megoat, double aauscnzx, string wetfqjju) {
	bool vwyaryoqjickse = false;
	bool aeszqjxlyhopawo = true;
	bool vnxdtwtwsakpl = false;
	int ojpgnmecxo = 4079;
	if (false == false) {
		int eohlyerbl;
		for (eohlyerbl = 25; eohlyerbl > 0; eohlyerbl--) {
			continue;
		}
	}
	if (false == false) {
		int djhtebh;
		for (djhtebh = 8; djhtebh > 0; djhtebh--) {
			continue;
		}
	}
	if (true != true) {
		int tuhuj;
		for (tuhuj = 25; tuhuj > 0; tuhuj--) {
			continue;
		}
	}
	if (false == false) {
		int yx;
		for (yx = 2; yx > 0; yx--) {
			continue;
		}
	}
	return 14384;
}

int khwqsaw::xufufgimrwgzjfrvkspiqtxi(bool drueclkknyhwek, int vbhkomwupu, bool fxcsyxjffsoxerf) {
	double orzuhwpqerfi = 6010;
	string pnwuzaqj = "fhisgdukvqgbmzieidodbgaom";
	string gwfrb = "mfbbcbpbdlcmnvq";
	int ntrhsldkd = 7466;
	string ayensljcjjkfwvr = "nkgwmxiodngbrfgbsnehrlwuwiytkpzeuwpufallxlgpfgwoyxfkuvbrwsqcwpfhoufelnlxjlkjzciavjblwlxnzkcacro";
	if (string("fhisgdukvqgbmzieidodbgaom") != string("fhisgdukvqgbmzieidodbgaom")) {
		int kkix;
		for (kkix = 1; kkix > 0; kkix--) {
			continue;
		}
	}
	if (string("fhisgdukvqgbmzieidodbgaom") != string("fhisgdukvqgbmzieidodbgaom")) {
		int gtwfnfmm;
		for (gtwfnfmm = 17; gtwfnfmm > 0; gtwfnfmm--) {
			continue;
		}
	}
	if (string("mfbbcbpbdlcmnvq") == string("mfbbcbpbdlcmnvq")) {
		int egkk;
		for (egkk = 17; egkk > 0; egkk--) {
			continue;
		}
	}
	return 54319;
}

bool khwqsaw::loltqkyfvjzjvyea(int ywvlpl, int tbavizizsqzef, double fedjjzzlexflv, bool ercwbqoq, int wxeyhfxsyrfr, string gfhcrsvvt, double qmisxkkgtow) {
	int xlbkzlvsi = 2497;
	bool loafyhzgdwpprpf = true;
	string blddjyqzeapbjcl = "lqkhrhcmoyjiaktsotjrzhsrspxwtpvzlweupx";
	int zbojqmdxdtv = 176;
	if (true == true) {
		int ohzc;
		for (ohzc = 14; ohzc > 0; ohzc--) {
			continue;
		}
	}
	if (2497 == 2497) {
		int ebcxqoruv;
		for (ebcxqoruv = 49; ebcxqoruv > 0; ebcxqoruv--) {
			continue;
		}
	}
	if (176 == 176) {
		int rrt;
		for (rrt = 88; rrt > 0; rrt--) {
			continue;
		}
	}
	if (176 == 176) {
		int oqxiczdjd;
		for (oqxiczdjd = 85; oqxiczdjd > 0; oqxiczdjd--) {
			continue;
		}
	}
	return true;
}

khwqsaw::khwqsaw() {
	this->loltqkyfvjzjvyea(511, 555, 13629, false, 102, string("vgsgtsetdmdsznneiobnetitnfsjekyviwklbjvxdgluggjff"), 8678);
	this->qiiasacksygzmkzq(5551, string("yzujpamrgnodwnodiogkzwwbtiaayjqhslxxwpmcld"), string("mxuslebmsgkantenghlopex"), false, 2696, true, string("ubmlasqwwaxvfkwfgrinthfzvuyglehfognvljryciuwmbtvlnq"), 73693, string("nsvytopisvoqzmpefioqfjcjzdvvftqb"));
	this->yzzouvpeetxdfwhmq(string("zeuqjipygvprsixbygqrmqzekorpgudlnfvxwzhwgkpqihdbwqbwhmtubagrnpfd"), 2957, 825, 281, 174, string("uuxminuswsaukmgcjsrpgyjtdbzrruyjojqjzafuarvydeynbq"), string("fcxuabpueneojeffqy"), string("zycyk"));
	this->wegyrgnzrvkluey(false, true, 807, 3441, false, 1593, 8469, 4502, string("vcreeqboubdvrdwexwrwmwodhbphnxhgpjmncxixxfkrnhahmexwsnebchvbutknhtakirjxhpabrunfyzxfmpxwmndaza"));
	this->hgfvxgrxbiafvpt(false, string("whcuvuwzborcl"), string("zlrvgzdcdwicgaxbeqnfctvffhjqnaixiwpmw"), false, 33356, 63403, true, string("xzdrxaykymlcuwrymodcrltcadsiyhefefaazewnjkazipcduwdmvlrxdctyifjrtuut"), true, string("uilteojnirexmtxgipdurecaiqoxxxgcccugetgysakdcasnnzuqtwljjdjpaiekty"));
	this->ngnlgqgscgze(true);
	this->rmyvdfjvgvhtreywzcjwvrjn(string("jeixznbvrtnsmmhnqjvg"), 1198);
	this->zlxhbtpunyxdinlpazcpouc(514, string("ttacjokwjvyuskkjjgcwaacgclxlwduttolejfsvkrgeftojdbbjjhpszxondugqgfsfxqxuxfzhpr"));
	this->uiwudaojysee(false, string("nsjjilgfzdladzczzbjpydkfvkxsbpevravguxssleagyxswybkswynzrrbhlhedshhjotovwjfxfc"), string("avevexnogsbjfyxxswindjrttjgyrktorzhwatkpxcaaclhdposquqkrwjdlhagifppqavevyvozxivqaoaaabo"), 27928, 63306, 908, string(""), 17970, true, true);
	this->ooesikbtezchjtnmdh(true, 15991, string("rawtsbdauzayxjdkltkydkkpkkpnjjmtvwhuziprrhtcuzbcenaqmhkqqmxbqmyyyisogokdybsazdiemylvykydwysrzlb"));
	this->xufufgimrwgzjfrvkspiqtxi(false, 3160, true);
	this->dhnaojhmscxeaxssfxok(178, false, string("pszo"), 57999);
	this->hldbyofkemcagv(false, 5143, 33193, 16517, 25253, 70553, true, 292, string("pvaossyzhkwlagcpbvlizsvqsmwijoqstxbquvaqqpfyaaknqvviwidxznhzrezcirj"), 30746);
	this->ljrfylrmyo(18616, string("nliwfimnmapzvugncejszrnxwhpvpfsmvmrscgnjvufd"), string("tbiijcnebvpdtwseomvkwrponxmnycesplarzvpfhlpedg"), 7140, string("ujweuaecnqkjogacovixnwrogkfzxmiznmxrtbdedmhsmyzyamqmtpjyhukcmwfo"), 35658, true, 6680);
}
