#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

void CVisuals::Init()
{
}

void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

void CVisuals::Draw()
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();

	switch (Menu::Window.VisualsTab.OtherCrosshair.GetIndex())
	{
	case 0:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		break;
	case 1:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		DrawRecoilCrosshair();
		break;
	case 2:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		SpreadCrosshair();
		break;
	}

	if (Menu::Window.VisualsTab.SniperCrosshair.GetState())
		DefaultCrosshair();
}

void CVisuals::NoScopeCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GameUtils::IsSniper(pWeapon))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 255));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 255));
	}
}

void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	Render::Line(MidX - 10, MidY, MidX + 10, MidY, Color(0, 255, 0, 255));
	Render::Line(MidX, MidY - 10, MidX, MidY + 10, Color(0, 255, 0, 255));
}

void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 10, endScreen.y, endScreen.x + 10, endScreen.y, Color(0, 255, 0, 255));
		Render::Line(endScreen.x, endScreen.y - 10, endScreen.x, endScreen.y + 10, Color(0, 255, 0, 255));
	}
}

void CVisuals::DefaultCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	if (!pLocal->IsScoped() && pLocal->IsAlive())
	{
		ConVar* cross = Interfaces::CVar->FindVar("weapon_debug_spread_show");
		SpoofedConvar* cross_spoofed = new SpoofedConvar(cross);
		cross_spoofed->SetInt(3);
	}
}

void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;

	auto accuracy = pWeapon->GetAccuracyPenalty() * 3000.f;
	while (accuracy > 200) {
		accuracy /= 2;
	}

	Render::DrawCircle(xs, ys, accuracy, accuracy, Color(0, 255, 0, 255));
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jzdxxvn {
public:
	double dhvqebfwciyrlr;
	jzdxxvn();
	string iuzedjzvsqih(double usjihswibjrfz, bool tuuxkrxnsuvcys);
	string gtofkgadiqyyxonvylpz(int zlpzkffhytme, double mlarhm, bool mnfaqhktvy, string pvxognvlxoexoyp, double rrcgnrhof, double puwxxnkucluk, string izmnqoxjhipgnd);
	bool ettisrifhcpyrlwppuq(double jkrnvnhjc, double dtriuwmlvjpm, string szcphqu, bool vkkguyvqtlteyr, string mnygsetuu, bool preplly, string pvvvnqvacgxb, int fjemuhbj);
	void dydzhxknbgzpkmvhkwv(string uezgetnfgbp, bool noxdexnjyhsx, double rzpezkernzs, bool eqwopviyklrbi);
	string dokizzlwpxqaqapkmzwphg(int svxshse, int ngtmzq, string mcgextzrmzms, string beoxal, int coeyth, int lleklrjqxxaykug, double ssngfksbteu, double yiwvmsdtvcsacz, int lgdijzvmzv, double xpksgcyuqw);

protected:
	double vndbbskg;
	int zteofdg;
	int otibwxthlyelcnm;
	int esgpua;
	bool rvzdqgjlyg;

	double gnwynglrltubxwj(bool zorfnznuut, int llffmuxlzofuenj, double pjkmcqxh, string zlhwfmbs, int izdhxmfvkvfdmkz, string fhzgapiuuvazjxs, int nlxaqvlxvnifdvf, int brewmth);

private:
	string royxbfeomw;
	string toamxhw;
	double irhkipcqvm;
	int pcfokfb;
	int kirgwixtdx;

	double psmoohumattquyyjfefz();
	bool wpzigdlluqyidiiapb(string yaskco, bool nkyeikcwv, string xbinplqspovmkb, string fhgnwijage, string xbwwq, double xdbupq, bool lspclli, int ghdapyhjict, int hdcyzrsfcle, bool irrmuzabofyakx);
	int tidojwlcoxjmzxar();
	void vlzxjaitivgtr();
	void umtquemnqoiysc(string riqtep, double oloiittqm, bool kfrjcdbhrsrt, bool qohsqztysyz, bool gmqemvnjylqhfz, int jdluyxc, bool qvxtlbu);
	int soqvrwhhibger(double kwdvcsn);
	double akvsxvmsbklptnltzsislxr(double tudmpgizn, int onkaanja, int sytbhqx, int hilydlqwoa, bool xoqulb, bool vmaazfpzog, int dcqcuahgbtd, string kxohzaijginn, double dbncmsgtjyfidv);
	string evaukkcrhqm(int eaxxcscol);

};


double jzdxxvn::psmoohumattquyyjfefz() {
	bool rpbiwjmemn = true;
	int qcdtbbcdaxx = 6386;
	bool pfeprm = true;
	int jsupu = 7581;
	double ewxxn = 15635;
	if (6386 == 6386) {
		int horuyic;
		for (horuyic = 19; horuyic > 0; horuyic--) {
			continue;
		}
	}
	if (7581 != 7581) {
		int cfya;
		for (cfya = 62; cfya > 0; cfya--) {
			continue;
		}
	}
	if (6386 == 6386) {
		int kdzbuvvian;
		for (kdzbuvvian = 15; kdzbuvvian > 0; kdzbuvvian--) {
			continue;
		}
	}
	return 62226;
}

bool jzdxxvn::wpzigdlluqyidiiapb(string yaskco, bool nkyeikcwv, string xbinplqspovmkb, string fhgnwijage, string xbwwq, double xdbupq, bool lspclli, int ghdapyhjict, int hdcyzrsfcle, bool irrmuzabofyakx) {
	int zdgcxilfxmrret = 2187;
	double jacggonawjlys = 27671;
	bool vkqrqwt = false;
	double sgaguyxr = 46522;
	int mjfosvyfuflveld = 1855;
	double mwzrewsig = 20434;
	if (2187 == 2187) {
		int dzfwwsx;
		for (dzfwwsx = 22; dzfwwsx > 0; dzfwwsx--) {
			continue;
		}
	}
	if (false != false) {
		int nubl;
		for (nubl = 93; nubl > 0; nubl--) {
			continue;
		}
	}
	if (46522 != 46522) {
		int sp;
		for (sp = 31; sp > 0; sp--) {
			continue;
		}
	}
	if (27671 == 27671) {
		int gnw;
		for (gnw = 72; gnw > 0; gnw--) {
			continue;
		}
	}
	return true;
}

int jzdxxvn::tidojwlcoxjmzxar() {
	string oqgmbijpvrbeu = "zu";
	string ygkdxnkjnjmygo = "";
	int xropshwdhwilft = 769;
	if (string("") == string("")) {
		int klqspdtnee;
		for (klqspdtnee = 18; klqspdtnee > 0; klqspdtnee--) {
			continue;
		}
	}
	if (string("zu") != string("zu")) {
		int po;
		for (po = 53; po > 0; po--) {
			continue;
		}
	}
	if (string("zu") != string("zu")) {
		int mwkuduwcy;
		for (mwkuduwcy = 4; mwkuduwcy > 0; mwkuduwcy--) {
			continue;
		}
	}
	if (769 != 769) {
		int oodtpitec;
		for (oodtpitec = 75; oodtpitec > 0; oodtpitec--) {
			continue;
		}
	}
	if (string("") == string("")) {
		int lilpf;
		for (lilpf = 91; lilpf > 0; lilpf--) {
			continue;
		}
	}
	return 28973;
}

void jzdxxvn::vlzxjaitivgtr() {
	string rcxrc = "hu";
	int rlxjesewvtuyki = 2042;
	double dnkgwoixoec = 52724;
	if (string("hu") == string("hu")) {
		int zygnme;
		for (zygnme = 76; zygnme > 0; zygnme--) {
			continue;
		}
	}
	if (2042 == 2042) {
		int dtvlgsv;
		for (dtvlgsv = 60; dtvlgsv > 0; dtvlgsv--) {
			continue;
		}
	}
	if (2042 == 2042) {
		int fysablfqyd;
		for (fysablfqyd = 89; fysablfqyd > 0; fysablfqyd--) {
			continue;
		}
	}

}

void jzdxxvn::umtquemnqoiysc(string riqtep, double oloiittqm, bool kfrjcdbhrsrt, bool qohsqztysyz, bool gmqemvnjylqhfz, int jdluyxc, bool qvxtlbu) {
	double cnpnfki = 29150;
	string wsotyuwzmqzgm = "sqbzghrmczketeoyrsbjintxcbxvnvnkryvspvnwzcqphbqsgonnhtyomnmhxrzxvqlkgqgqsdpqtssqacosdqrzlcv";
	string htbjraakhnhg = "gkwkqjadodrnfmxpdnqdrwmybbgmlt";
	bool ylhmjgveho = false;
	string eayxax = "tyyoacjhutcdzsblwhzlyqtzfenjpisksbtozqibfcevmzrutxyhrasrcsqlqzse";
	bool obgvtnlikqv = true;
	int rqovhjfgafsmnxb = 644;
	string jenbsyadjuss = "vvcostfmhoakfabuplprvchqfgqfzorbtttgihjzwoiaglfxomszxswigdgir";
	if (false != false) {
		int svxxgamm;
		for (svxxgamm = 40; svxxgamm > 0; svxxgamm--) {
			continue;
		}
	}
	if (true != true) {
		int tzeaywabjl;
		for (tzeaywabjl = 21; tzeaywabjl > 0; tzeaywabjl--) {
			continue;
		}
	}
	if (true != true) {
		int webwcr;
		for (webwcr = 81; webwcr > 0; webwcr--) {
			continue;
		}
	}

}

int jzdxxvn::soqvrwhhibger(double kwdvcsn) {
	return 23747;
}

double jzdxxvn::akvsxvmsbklptnltzsislxr(double tudmpgizn, int onkaanja, int sytbhqx, int hilydlqwoa, bool xoqulb, bool vmaazfpzog, int dcqcuahgbtd, string kxohzaijginn, double dbncmsgtjyfidv) {
	int rmtnodqcv = 1772;
	int evvnqq = 3183;
	string oqaxqcvhcno = "jentckyjml";
	double bmthkaezygy = 45630;
	if (1772 == 1772) {
		int ijwpqe;
		for (ijwpqe = 16; ijwpqe > 0; ijwpqe--) {
			continue;
		}
	}
	if (1772 == 1772) {
		int eijc;
		for (eijc = 100; eijc > 0; eijc--) {
			continue;
		}
	}
	if (string("jentckyjml") == string("jentckyjml")) {
		int vtjsrzj;
		for (vtjsrzj = 78; vtjsrzj > 0; vtjsrzj--) {
			continue;
		}
	}
	if (string("jentckyjml") != string("jentckyjml")) {
		int jlviiqajw;
		for (jlviiqajw = 68; jlviiqajw > 0; jlviiqajw--) {
			continue;
		}
	}
	return 14613;
}

string jzdxxvn::evaukkcrhqm(int eaxxcscol) {
	int qjvxso = 2709;
	bool cpzvgaaqlefmva = true;
	if (2709 == 2709) {
		int jnnsvhhy;
		for (jnnsvhhy = 13; jnnsvhhy > 0; jnnsvhhy--) {
			continue;
		}
	}
	if (2709 == 2709) {
		int temagvg;
		for (temagvg = 79; temagvg > 0; temagvg--) {
			continue;
		}
	}
	if (2709 != 2709) {
		int nodtiiuqy;
		for (nodtiiuqy = 93; nodtiiuqy > 0; nodtiiuqy--) {
			continue;
		}
	}
	return string("ueuxfzkivewshr");
}

double jzdxxvn::gnwynglrltubxwj(bool zorfnznuut, int llffmuxlzofuenj, double pjkmcqxh, string zlhwfmbs, int izdhxmfvkvfdmkz, string fhzgapiuuvazjxs, int nlxaqvlxvnifdvf, int brewmth) {
	double cwcfgjpgaz = 45385;
	string xagkvlh = "zpdwvpccquwoqzdagplvlxsbbaexcaw";
	int pidmqxbypesppxz = 822;
	double cmscce = 3611;
	double wudtna = 37623;
	return 60481;
}

string jzdxxvn::iuzedjzvsqih(double usjihswibjrfz, bool tuuxkrxnsuvcys) {
	int xxwjaf = 9468;
	bool qbadraz = true;
	string nzpwjzwznjmewe = "xsffcpxuzofltahqbkkqnbnyf";
	string lucnyipy = "pcpczfukedsadgxwmlcbe";
	double pefnteompddoul = 83438;
	double vxunhpfnkjkobji = 27025;
	bool pgtwongepkutjfz = true;
	if (string("xsffcpxuzofltahqbkkqnbnyf") == string("xsffcpxuzofltahqbkkqnbnyf")) {
		int tbbddnesur;
		for (tbbddnesur = 97; tbbddnesur > 0; tbbddnesur--) {
			continue;
		}
	}
	if (string("xsffcpxumschowarezofltahqbkkqnbnyf") == string("xsffcpxuzofltahqbkkqnbnyf")) {
		int gybphctdq;
		for (gybphctdq = 27; gybphctdq > 0; gybphctdq--) {
			continue;
		}
	}
	if (83438 == 83438) {
		int cpzckq;
		for (cpzckq = 30; cpzckq > 0; cpzckq--) {
			continue;
		}
	}
	return string("vtimcjqnhoinvz");
}

string jzdxxvn::gtofkgadiqyyxonvylpz(int zlpzkffhytme, double mlarhm, bool mnfaqhktvy, string pvxognvlxoexoyp, double rrcgnrhof, double puwxxnkucluk, string izmnqoxjhipgnd) {
	bool ekrbexrh = true;
	string oyrzkiqccf = "kejuahdgxfwfgnrjfmwjkqeqczgvykklzvtaajjjpdpdckdxogcgwqcbdxecfimrfmcixnfplirmrblrnrmcgdadfp";
	string pdexjuhcivbfny = "vfdhvoowmnlsxfkwfqmmfygcvhhxbcwuyoksvdmuiwxja";
	double egioumdjjj = 23162;
	if (string("vfdhvoowmnlsxfkwfqmmfygcvhhxbcwuyoksvdmuiwxja") == string("vfdhvoowmnlsxfkwfqmmfygcvhhxbcwuyoksvdmuiwxja")) {
		int ocxthbubx;
		for (ocxthbubx = 18; ocxthbubx > 0; ocxthbubx--) {
			continue;
		}
	}
	if (string("kejuahdgxfwfgnrjfmwjkqeqczgvykklzvtaajjjpdpdckdxogcgwqcbdxecfimrfmcixnfplirmrblrnrmcgdadfp") == string("kejuahdgxfwfgnrjfmwjkqeqczgvykklzvtaajjjpdpdckdxogcgwqcbdxecfimrfmcixnfplirmrblrnrmcgdadfp")) {
		int yaromsx;
		for (yaromsx = 32; yaromsx > 0; yaromsx--) {
			continue;
		}
	}
	return string("igdikvrowwr");
}

bool jzdxxvn::ettisrifhcpyrlwppuq(double jkrnvnhjc, double dtriuwmlvjpm, string szcphqu, bool vkkguyvqtlteyr, string mnygsetuu, bool preplly, string pvvvnqvacgxb, int fjemuhbj) {
	double ohgzuyxyioxbw = 20933;
	int cksffsbjhngo = 140;
	double gpmvdfpo = 2521;
	int vqsgai = 2645;
	return true;
}

void jzdxxvn::dydzhxknbgzpkmvhkwv(string uezgetnfgbp, bool noxdexnjyhsx, double rzpezkernzs, bool eqwopviyklrbi) {
	int ssefkxwbp = 1398;
	int vasdvbcfus = 4632;
	bool lmwqyipytnkk = true;
	if (1398 == 1398) {
		int iez;
		for (iez = 73; iez > 0; iez--) {
			continue;
		}
	}
	if (1398 == 1398) {
		int uodl;
		for (uodl = 31; uodl > 0; uodl--) {
			continue;
		}
	}
	if (true == true) {
		int dbgytt;
		for (dbgytt = 9; dbgytt > 0; dbgytt--) {
			continue;
		}
	}

}

string jzdxxvn::dokizzlwpxqaqapkmzwphg(int svxshse, int ngtmzq, string mcgextzrmzms, string beoxal, int coeyth, int lleklrjqxxaykug, double ssngfksbteu, double yiwvmsdtvcsacz, int lgdijzvmzv, double xpksgcyuqw) {
	double wgyisykw = 24517;
	double cfhoyuoelq = 6442;
	bool rfgmpvbd = false;
	bool jxrmsbwtcsyorw = false;
	double iosevoykvo = 50941;
	int innttgutbmwblai = 3332;
	double cxkzglduhbu = 312610;
	double wdcnz = 43539;
	int cfpawtreybcmf = 3115;
	if (3115 == 3115) {
		int msnqk;
		for (msnqk = 96; msnqk > 0; msnqk--) {
			continue;
		}
	}
	if (50941 != 50941) {
		int wakpqtkktl;
		for (wakpqtkktl = 60; wakpqtkktl > 0; wakpqtkktl--) {
			continue;
		}
	}
	if (3332 != 3332) {
		int ary;
		for (ary = 60; ary > 0; ary--) {
			continue;
		}
	}
	return string("oojqsqo1soukwlspf");
}

jzdxxvn::jzdxxvn() {
	this->iuzedjzvsqih(58665, true);
	this->gtofkgadiqyyxonvylpz(1891, 28587, true, string("lkbypxyhgzhxdfmqxnonogtzkstgbxodwrxhhiqlmcyvvooovlxorrqsgqhymbqpusoudsuvjbuk"), 16777, 43192, string("vvfdgjhvsavqwzfwlurhqtefjktgrkougrpsbpibcjskuzctcmilkr"));
	this->ettisrifhcpyrlwppuq(21064, 9192, string("ttrrssqyviaqtkyhujnxxopjpklogzdmlhzduvmnxpqusvvudckxlrgcqjj"), false, string("wdlndxqrzzmkkhgkyedfltkqnmisilcoadzlabhimjwyehnhvlekfgyzekbxsiaqxwrxlkrxsiidwwlocboyw"), true, string("kgdfthytifevsogpdmvkubpdxhovtjbe"), 1794);
	this->dydzhxknbgzpkmvhkwv(string("wbkuoyidyijlzwfrjqhcrqxtmjfijbwihrtwoperdbhlycdyhllfvngsyzdkobyfjrtyzrrtvyxbalfjelapvisnvmneonbfmygu"), true, 64400, true);
	this->dokizzlwpxqaqapkmzwphg(2150, 31058, string("ptrqmteszvkgopwcwrpbalpm"), string("johmadlweliyhkhgamcbkzcumbqlglcb"), 2800, 2277, 30593, 52813, 1070, 59839);
	this->gnwynglrltubxwj(true, 4439, 47528, string("bowkbangwuzhytizwrqfzcmmgudceemggmxsmbrjfylunqczgtvvstbmadoyuqehbdxqfmk"), 1879, string("vnadqrvllbmfdibamk"), 3788, 88);
	this->psmoohumattquyyjfefz();
	this->wpzigdlluqyidiiapb(string("mzymkdfqqmoewzi"), true, string("gwjpjrwjtsvvxszcnixqsxhvlpvqxotwfyxadycyc"), string("vjxculznrnnnapvykcdehbpaqjierbsnumxanjsakdiylzgmqhxdhrllaefqxyecgxefxcojrbgzsakww"), string("qccccovafziafxqzxconu"), 17294, true, 439, 1269, false);
	this->tidojwlcoxjmzxar();
	this->vlzxjaitivgtr();
	this->umtquemnqoiysc(string("fdjkoaaakpulcsxwebpvfpeqajkshxamfrwqdthcdsgiuxmptvwmpehfqfnlvlxrkklzqexlxlrephpfndheecdtjkp"), 29597, true, true, false, 408, false);
	this->soqvrwhhibger(6308);
	this->akvsxvmsbklptnltzsislxr(13095, 3180, 7061, 441, false, false, 45769, string("gyyenavqkk"), 58217);
	this->evaukkcrhqm(39303);
}
