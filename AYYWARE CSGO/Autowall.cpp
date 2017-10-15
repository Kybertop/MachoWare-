#include "AutoWall.h"
//#include "R.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

inline bool CGameTrace::DidHitWorld() const
{
	return m_pEnt->GetIndex() == 0;
}

inline bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data);

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 0.5f;
	case HITGROUP_HEAD:
		return 2.0f;
	case HITGROUP_CHEST:
		return 0.5f;
	case HITGROUP_STOMACH:
		return 0.75f;
	case HITGROUP_LEFTARM:
		return 0.5f;
	case HITGROUP_RIGHTARM:
		return 0.5f;
	case HITGROUP_LEFTLEG:
		return 0.375f;
	case HITGROUP_RIGHTLEG:
		return 0.375f;
	case HITGROUP_GEAR:
		return 0.5f;
	default:
		return 1.0f;

	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->ArmorValue() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= (weapon_armor_ratio);
		}
		else
		{
			current_damage *= (weapon_armor_ratio);
		}
	}
}

bool SimulateFireBullet(IClientEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data)
{
	data.penetrate_count = 4; // Max Amount Of Penitration
	data.trace_length = 0.0f; // wow what a meme
	auto *wpn_data = weapon->GetCSWpnData(); // Get Weapon Info
	data.current_damage = (float)wpn_data->m_iDamage;// Set Damage Memes
	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_flRange - data.trace_length;
		Vector End_Point = data.src + data.direction * data.trace_length_remaining;
		UTIL_TraceLine(data.src, End_Point, 0x4600400B, local, 0, &data.enter_trace);
		UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		if (data.enter_trace.fraction == 1.0f) break;
		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(wpn_data->m_flRangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->m_flArmorRatio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(wpn_data, data)) break;
	}
	return false;
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = Interfaces::PhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow(wpn_data->m_flRangeModifier, (data.trace_length * 0.002));
	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))data.penetrate_count = 0;
	if (data.penetrate_count <= 0)return false;
	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit)) return false;
	surfacedata_t *exit_surface_data = Interfaces::PhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;
	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71)) { combined_penetration_modifier = 3.0f; final_damage_modifier = 0.05f; }
	else { combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f; }
	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)combined_penetration_modifier = 2.0f;
	}
	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_flPenetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)return false;
	if (lost_damage >= 0.0f)data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f) return false;
	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}


/*
*    CanHit() - example of how to use the code
*     @in  point: target hitbox vector
*     @out damage_given: amount of damage the shot would do
*/
bool CanHit(const Vector &point, float *damage_given)
{
	//Utils::ToLog("CANHIT");
	auto *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto data = FireBulletData(local->GetOrigin() + local->GetViewOffset());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
	CalcAngle(data.src, point, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(local, (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle()), data))
	{
		*damage_given = data.current_damage;
		//Utils::ToLog("CANHIT END");
		return true;
	}

	return false;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class spbljvl {
public:
	double wzdwf;
	string uguer;
	bool ziikwtelmmk;
	spbljvl();
	double bysolzlebnlhninxxyllfute(double zyhfalszpltll, bool tqcieypoubotf, string jinaybaipnqdtd);
	void foltyfukgqohxloovoaarhvhh(int fczomvdxxuor);
	void cnuzentlfqmcqnp(bool swrgo, bool wljzicfrp, bool jowtpbjg);
	double hbgpzuvmfzrjflcpmz(bool xckwkdcvjkt, string liiis, double smgkdwhwmshn, string gfqsdkxts, string nxqegwhgudni, int fhxbgtsncg, string tocwreemyio, string dbhzcolqefor, bool npzcuenowkehgu);

protected:
	string spsvdu;
	double scuxocwlovsi;
	bool mxuzmvfhzswe;
	int roeyjriv;
	string mvuhqlr;

	double ualmpolenvhqpysxx(double bpiuyr, string ftnbkrifotsk, string eogbpqs, bool zunoyx, double zvrcxbfbdwuovbc, double kqrhetvhyhsnxn, bool hqqpgtzzl, int nywit, double czdmsqigpggnfod, double gqcvhfbtil);
	bool wmccuqqjmaorfdlqntw(string cljuyjlwpoojzcf, double rjqusfbwulaajao, string dwzrjr, double jvjcxm);
	int pnraggsyxsdhvjwtjtobye(bool mfmbpxaup, string tyjisf, bool fhmkzzr, bool oqjhtkhe, double mdinz, int cwbdif, double bqwwwlshekqy, string vocgcufut);
	int vgljtoxzblilimddhmu(double hbecvkjihj, double waktszsxzpbwrw, bool igvfbhozhq, bool odcvpwbspdizd);
	bool mdcjxdtqlrgcqdx(double givylzwivkimvo);
	void snrtvfzgfztkjdrui(string blkxhfxlmt, bool zvpeixgml);
	void jzaamjejlkidjhzreqewfeb(double qqrohmadg);
	double mnqkixfcqtvruackjc(bool yotgcvfeybgoenk, double cfzzhpnjm, string rqfeyxslmayzn, string yeycwxcamtdx, double kqakfiilo, double wslyycfylhv, double ckptvmhazyke, string bqcfbqennsqmpc);
	int bsirfgdxnmvob(int fgisuxjlwlv, int iwjjbqlicfv, bool hfohoadzkvln, int gptjnghgvhyj, int wdchq, double krgsczxdjjfw, int jpmsjju, double izuuijvnc, string rbfvabw);
	int aqtsdwdtxyucctaztqtivuay(int kmbdgiesam, double oqycmqzgwx, int asjfexcswtjwey, int raizyuyes, int bovyckjvx, string cuuqlnhw, int orcdbncynqvz, double dosay, int zrcksqavnc);

private:
	bool bciydaropo;
	bool bzoqjcjadn;

	double kpbleimvyyttp(int dwkywpbcryhl, bool mvfvhknujllymfx);
	bool tkoxmdbhaifbyruehunl(string dgxxogfze, int hckgc, string hpmshbkwagx, bool visiaqxgcbswiy, double lxyvk);
	double zhlhgscinicojfvjzfzbrmjm(string olteglwtbzgys);

};


double spbljvl::kpbleimvyyttp(int dwkywpbcryhl, bool mvfvhknujllymfx) {
	double kvhjruwvg = 71940;
	string nvohrmg = "mndwxz";
	int ulyfhqti = 4587;
	if (4587 != 4587) {
		int dklwlfos;
		for (dklwlfos = 29; dklwlfos > 0; dklwlfos--) {
			continue;
		}
	}
	if (71940 != 71940) {
		int dwnazfunom;
		for (dwnazfunom = 34; dwnazfunom > 0; dwnazfunom--) {
			continue;
		}
	}
	if (4587 == 4587) {
		int hsp;
		for (hsp = 57; hsp > 0; hsp--) {
			continue;
		}
	}
	return 38065;
}

bool spbljvl::tkoxmdbhaifbyruehunl(string dgxxogfze, int hckgc, string hpmshbkwagx, bool visiaqxgcbswiy, double lxyvk) {
	int bblqhjq = 1409;
	if (1409 != 1409) {
		int wzspgmk;
		for (wzspgmk = 11; wzspgmk > 0; wzspgmk--) {
			continue;
		}
	}
	if (1409 != 1409) {
		int gdqndkhx;
		for (gdqndkhx = 67; gdqndkhx > 0; gdqndkhx--) {
			continue;
		}
	}
	if (1409 == 1409) {
		int lhuttw;
		for (lhuttw = 77; lhuttw > 0; lhuttw--) {
			continue;
		}
	}
	if (1409 == 1409) {
		int eweoqvfium;
		for (eweoqvfium = 19; eweoqvfium > 0; eweoqvfium--) {
			continue;
		}
	}
	return true;
}

double spbljvl::zhlhgscinicojfvjzfzbrmjm(string olteglwtbzgys) {
	return 6950;
}

double spbljvl::ualmpolenvhqpysxx(double bpiuyr, string ftnbkrifotsk, string eogbpqs, bool zunoyx, double zvrcxbfbdwuovbc, double kqrhetvhyhsnxn, bool hqqpgtzzl, int nywit, double czdmsqigpggnfod, double gqcvhfbtil) {
	int anxyjkxq = 51;
	double kpksotfeirog = 20335;
	if (51 != 51) {
		int nskrtaa;
		for (nskrtaa = 77; nskrtaa > 0; nskrtaa--) {
			continue;
		}
	}
	if (51 != 51) {
		int twbbmqajk;
		for (twbbmqajk = 96; twbbmqajk > 0; twbbmqajk--) {
			continue;
		}
	}
	if (20335 != 20335) {
		int wxbjpahwme;
		for (wxbjpahwme = 72; wxbjpahwme > 0; wxbjpahwme--) {
			continue;
		}
	}
	if (20335 == 20335) {
		int rwrrhzuk;
		for (rwrrhzuk = 64; rwrrhzuk > 0; rwrrhzuk--) {
			continue;
		}
	}
	return 54924;
}

bool spbljvl::wmccuqqjmaorfdlqntw(string cljuyjlwpoojzcf, double rjqusfbwulaajao, string dwzrjr, double jvjcxm) {
	bool bjwpiqabark = false;
	double ewvdehogalbphdq = 28553;
	double ewnmnngybbmjpz = 3800;
	int rdkpmqs = 2846;
	if (3800 != 3800) {
		int oavdeocu;
		for (oavdeocu = 20; oavdeocu > 0; oavdeocu--) {
			continue;
		}
	}
	return true;
}

int spbljvl::pnraggsyxsdhvjwtjtobye(bool mfmbpxaup, string tyjisf, bool fhmkzzr, bool oqjhtkhe, double mdinz, int cwbdif, double bqwwwlshekqy, string vocgcufut) {
	int dskcqzbm = 104;
	string lsbtnonha = "zjoxzgkgmcmtwtxnkfnxyubsadahcppgcqckumhdxtbtaru";
	if (string("zjoxzgkgmcmtwtxnkfnxyubsadahcppgcqckumhdxtbtaru") != string("zjoxzgkgmcmtwtxnkfnxyubsadahcppgcqckumhdxtbtaru")) {
		int cjgmqnzz;
		for (cjgmqnzz = 98; cjgmqnzz > 0; cjgmqnzz--) {
			continue;
		}
	}
	if (104 != 104) {
		int yoxptvd;
		for (yoxptvd = 43; yoxptvd > 0; yoxptvd--) {
			continue;
		}
	}
	if (string("zjoxzgkgmcmtwtxnkfnxyubsadahcppgcqckumhdxtbtaru") == string("zjoxzgkgmcmtwtxnkfnxyubsadahcppgcqckumhdxtbtaru")) {
		int zbgugbgyra;
		for (zbgugbgyra = 74; zbgugbgyra > 0; zbgugbgyra--) {
			continue;
		}
	}
	return 91339;
}

int spbljvl::vgljtoxzblilimddhmu(double hbecvkjihj, double waktszsxzpbwrw, bool igvfbhozhq, bool odcvpwbspdizd) {
	double flunnlyuix = 25186;
	string feksukm = "ffuvyblprxbhjihupioslmfvpjkdxclpxbqeudfoxaournxvmbilfoxcibmcibijcasoeaeojnloidyrsnksycsvrzkqxmrkrdj";
	bool qnkbvddasbbhx = false;
	double nbbpd = 48584;
	double bycydmkoalvrysd = 54590;
	if (string("ffuvyblprxbhjihupioslmfvpjkdxclpxbqeudfoxaournxvmbilfoxcibmcibijcasoeaeojnloidyrsnksycsvrzkqxmrkrdj") == string("ffuvyblprxbhjihupioslmfvpjkdxclpxbqeudfoxaournxvmbilfoxcibmcibijcasoeaeojnloidyrsnksycsvrzkqxmrkrdj")) {
		int oleiem;
		for (oleiem = 68; oleiem > 0; oleiem--) {
			continue;
		}
	}
	if (25186 != 25186) {
		int di;
		for (di = 46; di > 0; di--) {
			continue;
		}
	}
	if (25186 != 25186) {
		int wzmkyjiwjq;
		for (wzmkyjiwjq = 2; wzmkyjiwjq > 0; wzmkyjiwjq--) {
			continue;
		}
	}
	if (25186 == 25186) {
		int bfjtic;
		for (bfjtic = 50; bfjtic > 0; bfjtic--) {
			continue;
		}
	}
	return 1648;
}

bool spbljvl::mdcjxdtqlrgcqdx(double givylzwivkimvo) {
	string eyhrrfccg = "igawtwkozghugldifetazjqkcsucrfptoevanulvlcndeuaqwzfvuqcwnvfntsfficzvdtrpvgusdbdcdpu";
	string znetez = "exibyxtfwhhhfaziqfwpoqdbjtusmnpixkdtctrmvomebkrkgajlwhduzhpddfjusedymavfudczvjvgnfgcbqtrunucv";
	bool sxnstva = true;
	bool ramegic = true;
	bool dqtgknwp = false;
	if (true != true) {
		int qscmwwkhej;
		for (qscmwwkhej = 83; qscmwwkhej > 0; qscmwwkhej--) {
			continue;
		}
	}
	if (string("exibyxtfwhhhfaziqfwpoqdbjtusmnpixkdtctrmvomebkrkgajlwhduzhpddfjusedymavfudczvjvgnfgcbqtrunucv") == string("exibyxtfwhhhfaziqfwpoqdbjtusmnpixkdtctrmvomebkrkgajlwhduzhpddfjusedymavfudczvjvgnfgcbqtrunucv")) {
		int zslqz;
		for (zslqz = 43; zslqz > 0; zslqz--) {
			continue;
		}
	}
	return true;
}

void spbljvl::snrtvfzgfztkjdrui(string blkxhfxlmt, bool zvpeixgml) {

}

void spbljvl::jzaamjejlkidjhzreqewfeb(double qqrohmadg) {
	double ecyta = 30684;
	bool fpiak = false;
	bool dwtpfrrvdgk = false;
	bool vjpedhk = false;
	double dltlludxlf = 55637;
	string hohrimcgcusxvmd = "wnfdmqbujowdfmdrpcexwlegijgewghacdulvpwdlbtqmhbuybikglcrcalvepkn";
	string nbuanrfzdincvyx = "quzmsdawdsmnyfqlfdhjinxxzhbaseybjleyzxfvexcoaiwjg6146cqizkd";
	bool bsoxqo = false;
	if (55637 == 55637) {
		int ekhiotdaww;
		for (ekhiotdaww = 96; ekhiotdaww > 0; ekhiotdaww--) {
			continue;
		}
	}
	if (false == false) {
		int xziw;
		for (xziw = 81; xziw > 0; xziw--) {
			continue;
		}
	}
	if (string("quzmsdawdsmnyfqlfdhjinxxzhbaseybjleyzxfvexcowurmbmqinhvynnircqizkd") == string("quzmsdawdsmnyfqlfdhjinxxzhbaseybjleyzxfvexcowurmbmqinhvynnircqizkd")) {
		int qcaohi;
		for (qcaohi = 84; qcaohi > 0; qcaohi--) {
			continue;
		}
	}
	if (55637 != 55637) {
		int pylkchph;
		for (pylkchph = 67; pylkchph > 0; pylkchph--) {
			continue;
		}
	}

}

double spbljvl::mnqkixfcqtvruackjc(bool yotgcvfeybgoenk, double cfzzhpnjm, string rqfeyxslmayzn, string yeycwxcamtdx, double kqakfiilo, double wslyycfylhv, double ckptvmhazyke, string bqcfbqennsqmpc) {
	bool klcasuwczgsjjr = false;
	bool mfdopbp = false;
	int xlxls = 4372;
	int yveseenkpsunqgq = 4295;
	bool yjorlydafzhkhn = false;
	bool eibobki = false;
	double gwveyihrocw = 38731;
	bool xzblczknjcaw = false;
	int oymcxdpuzq = 1111;
	return 64639;
}

int spbljvl::bsirfgdxnmvob(int fgisuxjlwlv, int iwjjbqlicfv, bool hfohoadzkvln, int gptjnghgvhyj, int wdchq, double krgsczxdjjfw, int jpmsjju, double izuuijvnc, string rbfvabw) {
	string fitzwon = "ezxnscyygiz";
	int vblmcrlluwct = 6172;
	string jddtdhiq = "katiyzxkiuxvnvsjvdx";
	bool dodtefdcjmzxqr = true;
	int pnmkfjiarzr = 5510;
	if (true == true) {
		int arg;
		for (arg = 60; arg > 0; arg--) {
			continue;
		}
	}
	return 30874;
}

int spbljvl::aqtsdwdtxyucctaztqtivuay(int kmbdgiesam, double oqycmqzgwx, int asjfexcswtjwey, int raizyuyes, int bovyckjvx, string cuuqlnhw, int orcdbncynqvz, double dosay, int zrcksqavnc) {
	int uthdqr = 4701;
	string fbquoiclt = "qmqgjgndu";
	int fykgbdnqdkkrtq = 4798;
	double wtblkonqamggxn = 1694;
	string hqzpvqvf = "kmhhczciqgsqelnylycxoinpokjyvfgjszayaqglnuqkfvyfwvshqn";
	string cwnymw = "mwqqedprbeuoopshntspprhycfijrlzgqhs";
	string ynsbsbgobhgpmxk = "foayvodedbxysxyshooggssgctystmdccgkhodokkqulucpawwglwagunxycbpzykpclzmevcpybweklo";
	bool ezyivasmvcydqd = true;
	if (1694 != 1694) {
		int mgynidya;
		for (mgynidya = 2; mgynidya > 0; mgynidya--) {
			continue;
		}
	}
	if (4798 == 4798) {
		int vzmqtyqs;
		for (vzmqtyqs = 32; vzmqtyqs > 0; vzmqtyqs--) {
			continue;
		}
	}
	return 19868;
}

double spbljvl::bysolzlebnlhninxxyllfute(double zyhfalszpltll, bool tqcieypoubotf, string jinaybaipnqdtd) {
	string izjwq = "bjkoepfrjpgamtcnxagtrsojoefeulrogsjzc";
	string dmeotxihbx = "rqusycbcgmdjsjxagqydtseihnwrulxgsgegtyfvxezpxesqdnjkqioksrjidysvfrultrdaksphwfucxjrvqxbpqospmcatwa";
	int hfnnuazjwchwi = 3117;
	int ifebaaweaq = 2354;
	string wcsgkviju = "muowvahbwctixhmdqmcqhlpmaylavzsbfaijykhxmwdiydyyfimjhmorhkxyxbffdmur";
	bool alluabeqqw = true;
	string gwovbawub = "aeffkdkivuxqtwm";
	string ahevf = "qwozxvvzmxmhthuwvorqcqhyvsquujyqlfultgxbhvddg";
	if (string("bjkoepfrjpgamtcnxagtrsojoefeulrogsjzc") != string("bjkoepfrjpgamtcnxagtrsojoefeulrogsjzc")) {
		int euxsdz;
		for (euxsdz = 3; euxsdz > 0; euxsdz--) {
			continue;
		}
	}
	if (string("aeffkdkivuxqtwm") != string("aeffkdkivuxqtwm")) {
		int nxhx;
		for (nxhx = 86; nxhx > 0; nxhx--) {
			continue;
		}
	}
	if (string("bjkoepfrjpgamtcnxagtrsojoefeulrogsjzc") != string("bjkoepfrjpgamtcnxagtrsojoefeulrogsjzc")) {
		int reax;
		for (reax = 12; reax > 0; reax--) {
			continue;
		}
	}
	if (string("rqusycbcgmdjsjxagqydtseihnwrulxgsgegtyfvxezpxesqdnjkqioksrjidysvfrultrdaksphwfucxjrvqxbpqospmcatwa") != string("rqusycbcgmdjsjxagqydtseihnwrulxgsgegtyfvxezpxesqdnjkqioksrjidysvfrultrdaksphwfucxjrvqxbpqospmcatwa")) {
		int kqsgq;
		for (kqsgq = 76; kqsgq > 0; kqsgq--) {
			continue;
		}
	}
	if (string("muowvahbwctixhmdqmcqhlpmaylavzsbfaijykhxmwdiydyyfimjhmorhkxyxbffdmur") != string("muowvahbwctixhmdqmcqhlpmaylavzsbfaijykhxmwdiydyyfimjhmorhkxyxbffdmur")) {
		int rwrfsrd;
		for (rwrfsrd = 94; rwrfsrd > 0; rwrfsrd--) {
			continue;
		}
	}
	return 28281;
}

void spbljvl::foltyfukgqohxloovoaarhvhh(int fczomvdxxuor) {
	string tmsfknzlsydnjto = "kxvytuvdisjmnyauhqgztezgrvnxmuapb";
	int ugyfzajhkjely = 876;
	double thqvtnohrkgpnca = 7174;
	int fhiugzxoycwayq = 4869;
	bool xxggckzyverow = false;
	double jnqarbhvyasrgtg = 10135;
	if (4869 == 4869) {
		int tdprajwsw;
		for (tdprajwsw = 22; tdprajwsw > 0; tdprajwsw--) {
			continue;
		}
	}

}

void spbljvl::cnuzentlfqmcqnp(bool swrgo, bool wljzicfrp, bool jowtpbjg) {
	string ofzth = "mkguiptdyginhmkglgqfyxssdfyyvtyjlberkwrq";
	bool jgiepkw = true;
	double lefeuhcmx = 79383;
	string ohrdoe = "rrclfcympzgomjtprlwnfqlouxmetfbmwvvgcjcznndpkycoxgydyxqsynjqmqwlsim";
	bool audifoicymuvuau = true;
	int zdlvoiwf = 3030;
	if (string("mkguiptdyginhmkglgqfyxssdfyyvtyjlberkwrq") == string("mkguiptdyginhmkglgqfyxssdfyyvtyjlberkwrq")) {
		int wydqty;
		for (wydqty = 68; wydqty > 0; wydqty--) {
			continue;
		}
	}
	if (3030 == 3030) {
		int bbtiulpm;
		for (bbtiulpm = 43; bbtiulpm > 0; bbtiulpm--) {
			continue;
		}
	}
	if (string("mkguiptdyginhmkglgqfyxssdfyyvtyjlberkwrq") != string("mkguiptdyginhmkglgqfyxssdfyyvtyjlberkwrq")) {
		int mzqyvjeb;
		for (mzqyvjeb = 43; mzqyvjeb > 0; mzqyvjeb--) {
			continue;
		}
	}

}

double spbljvl::hbgpzuvmfzrjflcpmz(bool xckwkdcvjkt, string liiis, double smgkdwhwmshn, string gfqsdkxts, string nxqegwhgudni, int fhxbgtsncg, string tocwreemyio, string dbhzcolqefor, bool npzcuenowkehgu) {
	double jynkoywcuqrqlf = 707;
	int huqvshirmpj = 2664;
	bool quqzjed = false;
	string kajrbacqbibmgmh = "ubbbexqxsflbrcaiymozlsvtnxmbfxsjjmzzzkjukqtrcqjgnjtzylnwkhvkwkgjzn";
	string nqjcwowdfu = "cvrpmkixiczumpsurhwspcothcbpshjpuypfuydliwcpzwbcrpkydmahflqxulcaukxfnhkeqchtsbo";
	int ugygxkzbjkfjyro = 3156;
	int scpuvh = 1913;
	string whdei = "xllnxwbbyfnntsdlcrabuiaxablipjflmptbizqcogznwwtdsy";
	string nvejfhgjmp = "edmrftwbpizpxcwawih";
	if (707 != 707) {
		int xvevfqot;
		for (xvevfqot = 96; xvevfqot > 0; xvevfqot--) {
			continue;
		}
	}
	if (string("cvrpmkixiczumpsurhwspcothcbpshjpuypfuydliwcpzwbcrpkydmahflqxulcaukxfnhkeqchtsbo") != string("cvrpmkixiczumpsurhwspcothcbpshjpuypfuydliwcpzwbcrpkydmahflqxulcaukxfnhkeqchtsbo")) {
		int godllepncl;
		for (godllepncl = 14; godllepncl > 0; godllepncl--) {
			continue;
		}
	}
	if (3156 == 3156) {
		int shgdbyt;
		for (shgdbyt = 32; shgdbyt > 0; shgdbyt--) {
			continue;
		}
	}
	if (string("cvrpmkixiczumpsurhwspcothcbpshjpuypfuydliwcpzwbcrpkydmahflqxulcaukxfnhkeqchtsbo") == string("cvrpmkixiczumpsurhwspcothcbpshjpuypfuydliwcpzwbcrpkydmahflqxulcaukxfnhkeqchtsbo")) {
		int hhofj;
		for (hhofj = 25; hhofj > 0; hhofj--) {
			continue;
		}
	}
	if (2664 == 2664) {
		int hdrfkqv;
		for (hdrfkqv = 94; hdrfkqv > 0; hdrfkqv--) {
			continue;
		}
	}
	return 56779;
}

spbljvl::spbljvl() {
	this->bysolzlebnlhninxxyllfute(40306, true, string("nnrelkbbytekvctdyhipx"));
	this->foltyfukgqohxloovoaarhvhh(2324);
	this->cnuzentlfqmcqnp(false, false, false);
	this->hbgpzuvmfzrjflcpmz(true, string("sk"), 10780, string("ulywsywcnohbelqbacbhopvmkgklsjlidqhswerdmpcpw"), string("djcitsvzhwnkipjjyxpmvtwabgjvptqczgsjynmgtpnjwzahrghxscuzhamacbczamxzzcymvzevmplrmqiohwxwclllrqbderl"), 1499, string("fhwpyxkoelgxzyhhp"), string("jdouilxuczp"), false);
	this->ualmpolenvhqpysxx(4679, string("qdhhjjhgitmigjmrjhbtlqbgdalennxapkvbbuoqeyxxl"), string("nklvfankctncjrovddvglgiuhyziqwhnqqhazyfppbfe"), false, 31195, 77498, true, 96, 36255, 59600);
	this->wmccuqqjmaorfdlqntw(string("xtrtdhuvemvrxbmhsqsiqtzvglpsyzuvpsoymqzqowlhyqygpcrijwyvnturdnvumeawmy"), 39287, string("flyhwjrbpmgmxjypgpqbbxnzskrpvpuzvszgmacwssmnbiml"), 33512);
	this->pnraggsyxsdhvjwtjtobye(true, string("dmluxnkvvqjzuabpttebuxrlabwy"), true, false, 39797, 1206, 8859, string("gsqcrxicwtzzflqktyjwuywbctvruyxdsp"));
	this->vgljtoxzblilimddhmu(53071, 572, false, false);
	this->mdcjxdtqlrgcqdx(668);
	this->snrtvfzgfztkjdrui(string("zletrhindwnubhjjodmfvmyifhlmiauvhqeahuhukocqdnbbjajl"), true);
	this->jzaamjejlkidjhzreqewfeb(37272);
	this->mnqkixfcqtvruackjc(true, 66870, string("flecfhvsdarzpmfpfiwmpsevlqrterc"), string("doujnbvtvnajwjysdfstjgkugvnjjavevseyfyqdroolacmcpcuuesaexvkorznwnqhqeciooviwnurx"), 967, 35773, 21496, string("dwvhigiwsghfhffnbsjgzjfcpp"));
	this->bsirfgdxnmvob(7175, 3694, true, 4774, 492, 39395, 2739, 59618, string("pxjjlvemvarglkuwaplemteqfiqmwhpugovlxmchjzivbxddfyimkx"));
	this->aqtsdwdtxyucctaztqtivuay(229, 58871, 1884, 2663, 1283, string("oldmuovjavitkuhnzqgxteixhfgnzyfdwnnviiatykfgbbduuvap"), 1775, 27462, 1455);
	this->kpbleimvyyttp(690, false);
	this->tkoxmdbhaifbyruehunl(string(""), 3746, string("lmfcnatonmjslyhyqhjxtmferttcahrsvssdxfbsspeslakwmjdhzlareqqmxyvdklbojbfnc"), true, 1402);
	this->zhlhgscinicojfvjzfzbrmjm(string("eutwtrlietbfuuavidxlysrkuifatpbnmginzpdivoyvuxhgxfjgyc"));
}
