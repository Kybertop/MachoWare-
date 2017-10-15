#include "DLLMain.h"
#include "Utilities.h"

#include "INJ/ReflectiveLoader.h"

#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "AntiAntiAim.h"
#include "hitmarker.h"
#include "AntiLeak.h"

#include "Dumping.h"

#define ExampleHWID -1844215622


extern HINSTANCE hAppInstance;

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

HINSTANCE HThisModule;
bool DoUnload;

void CAntiLeak::ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0; 
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}

int InitialThread()
{
#ifdef Avoz_DEBUG
	Utilities::OpenConsole("SteamXYZ");
#endif
	PrintMetaHeader();

	Offsets::Initialise();
	Interfaces::Initialise(); 
	NetVar.RetrieveClasses();
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	hitmarker::singleton()->initialize();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyAAAHooks();

	//Dump::DumpClassIds();


	Utilities::Log("Ready");

	while (DoUnload == false)
	{
		Sleep(1000);
	}

	Hooks::UndoHooks();
	Sleep(2000); 
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		if (dwSerial == ExampleHWID)
		{
			Sleep(100);
		}
		else
		{
			// when HWID rejected
			MessageBox(NULL, "HWID doesn't match!", "MW", MB_OK);
			exit(0);
			return TRUE;
		}

		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class azvvacl {
public:
	int fgxnbvcb;
	double zzvwonte;
	int jvrfgul;
	string vblfqwoq;
	azvvacl();
	int heihocschxwrfdrvbwplqpvdf(string wylsynjvbktg, string nenmwta, int jqukczjfuf, int eemeitgsbd, int ponpevjwjierwab, bool blfzj, bool srihihlftmabtkk);
	bool emibaovapmjsmw();
	double jackiphqdzpb(bool thnua);

protected:
	string qrbcql;
	double ecelyrlh;
	double qwgtvummrpmg;
	int siiypiwodtjdgk;

	double bogtsvhvqwzzobrf(string danzhigexse, double xhejqcvzyglwua, string cpohhcs, string qrjpurksvmslpwk);
	string whzygfichitfoiyg(double rbkwajrpupmbrk, bool uzmawgthwmzl, int qzdmopkydxvdg, bool eazigfbuhcjgc, bool cuiqg, string mqkdc);
	void vjruopowaiqxnzt(int pviozkadprvj, bool ucvvervizhyig, bool vheeqy, double wtzowirroerbxzu, double vsofejzgyh, int svgkusgbneo, bool qeqtjjmnvg, string cyorhcwhmgou, bool rvcylfjsxwlehex);
	int kdnkuhbnwx(bool xfdworivngjkpd, double esrtympiiecf, double nimsqt, double drjxhwz, int dixbtavxqoh);
	string kkznoodvrjvc(string fdgki, bool czxvr, int uocti, string wctiyzppnjiee, int kssuypf, bool civwtlz, bool ormkkwkdznajb, double yvgpkbwwlsdtk, double hlvuflydpfperjt, string cewslzobpmrrt);

private:
	bool tauaxijvvm;
	double spjwlrzdzvt;
	int yrjiwneshef;

	void baahcmxczrevrvosz(int kjmhycmwnaqemut, int cwrnnnydvdbkk, double dofjlpahn, int zibrejn, int eemqt);
	void qsxcgmcfqqvluggqtinanyco(int jfyxvmplprmm, bool qaovgdzxagvumu, string kyrdxbkodnhg, double ygelaworebch, bool tzuxxagixuvfun, string flqfr, double bmksu, string hlgghctro, double chveaieu, string kypmy);
	double cgriqbrzzv(string wvdlzfxurbyuoy, int gqlxg, string mfmamxl, int jgjwkgw, int hwoqvog);
	bool mghqpsprsysbh(string zebcxijlimaheui, string ckiinclwnoa, double wnfbxmy);
	void dmhvqsxokkqernn(int kxpdafpy, bool slwjwsmevadeqk, int bckucaveky, string wxwbtrbor);
	int skzydrugnhvmmgwnlrahljxw(bool vxezmxmhmjmuy, int nlwdgfaqu, int pgmblrhsehky, double kcuzdn, double qaegtwt, int emhzvlmgifvge, bool nhrvbonvaymvsh, double fdivtf);
	void ozgrplytxtxvqximkgjjygu(string xjcplkyulglyy, int yaxqxdpl);
	bool jnemgfzbgmcwbxs(bool dcoocnzoruifwaj, bool ubuirasr, bool oefnr, double ophlqrragjoxxo, bool lxkuomccez, string tnzvdeistest, bool ngalphdrnmzvsrx, double wbmxgckubn);
	void lcfjctjxoqxkpklx(int hdwvjozauufggnq, double ideucxjpmqhdutm, string dvygeilsc);
	string eeeoarohvgohe(bool gahrqp, int kpzgnyf, bool jqjuzam, bool uvofosqdimrqetz);

};


void azvvacl::baahcmxczrevrvosz(int kjmhycmwnaqemut, int cwrnnnydvdbkk, double dofjlpahn, int zibrejn, int eemqt) {
	string cbndelnxzjyc = "czkjtvxihwgzuwjxruawqayuhqxwcndkhturcctspttkvcgzsjfwpeqgemaahvwradwznwrdivvxixhfakdzwqdics";
	string ozkyaujvqhqy = "nnwqlsddcxkulcnxrmgbu";
	bool tanecddzcvj = true;
	string xqmsitbq = "heqafsghrktwuksclqeibvenkjyyxibmctqdsiigbaqnensktjzgkn";
	string kehtgbwfkozki = "nhjeyugnzkvdlobvecpbecpnoghcearuhwmrgc";
	double xqcaqjejk = 23844;
	int ikqkirpl = 6472;
	string ldbzsc = "yhikpnsknramfjymxvbizujzgixuxyhdnfjgqgegzqrvbqxuacaxwfepgw";
	double vwlfrpgni = 62649;
	if (23844 != 23844) {
		int svsc;
		for (svsc = 95; svsc > 0; svsc--) {
			continue;
		}
	}
	if (string("yhikpnsknramfjymxvbizujzgixuxyhdnfjgqgegzqrvbqxuacaxwfepgw") == string("yhikpnsknramfjymxvbizujzgixuxyhdnfjgqgegzqrvbqxuacaxwfepgw")) {
		int vtqhiuehn;
		for (vtqhiuehn = 38; vtqhiuehn > 0; vtqhiuehn--) {
			continue;
		}
	}
	if (string("yhikpnsknramfjymxvbizujzgixuxyhdnfjgqgegzqrvbqxuacaxwfepgw") != string("yhikpnsknramfjymxvbizujzgixuxyhdnfjgqgegzqrvbqxuacaxwfepgw")) {
		int zhfbx;
		for (zhfbx = 2; zhfbx > 0; zhfbx--) {
			continue;
		}
	}
	if (6472 != 6472) {
		int wpyenvdbe;
		for (wpyenvdbe = 24; wpyenvdbe > 0; wpyenvdbe--) {
			continue;
		}
	}

}

void azvvacl::qsxcgmcfqqvluggqtinanyco(int jfyxvmplprmm, bool qaovgdzxagvumu, string kyrdxbkodnhg, double ygelaworebch, bool tzuxxagixuvfun, string flqfr, double bmksu, string hlgghctro, double chveaieu, string kypmy) {
	bool azbatsnnds = false;
	double ugzkjignfgoqp = 2689;
	int cknchfeurlnjk = 3686;
	string zqsum = "eesxihqqcowa";
	int qeqqudpjtlhqn = 2692;
	int bzwlvfmoxlhrvp = 787;
	string dukykviy = "ogrzxhwyjjwzpmrsyntcsqpxeixfbwpucbdoivrmesbiryumpphehvgfdmhgmtaefiwbe";
	string xlephn = "nelnjwjzponmidi";

}

double azvvacl::cgriqbrzzv(string wvdlzfxurbyuoy, int gqlxg, string mfmamxl, int jgjwkgw, int hwoqvog) {
	int kyjvnpqr = 7632;
	int wdbjdl = 1487;
	double qjiqqfatovy = 8357;
	double fdrsbcbpzefsun = 37602;
	bool mxevg = false;
	string yetebpcgg = "fagwmnwgtmobywmstpqyxtpzzmrexswtgjflzfwuuyzobcczzhgqgbw";
	double wjuwzel = 62396;
	string clnojk = "p";
	int xghvcvlc = 3666;
	if (1487 == 1487) {
		int cwewpg;
		for (cwewpg = 56; cwewpg > 0; cwewpg--) {
			continue;
		}
	}
	if (8357 != 8357) {
		int gif;
		for (gif = 19; gif > 0; gif--) {
			continue;
		}
	}
	return 73683;
}

bool azvvacl::mghqpsprsysbh(string zebcxijlimaheui, string ckiinclwnoa, double wnfbxmy) {
	int uxstfjieg = 489;
	int nkyjulvhbhslvov = 1043;
	string hmlsnrkwq = "dyzpxuswjfrylpdtszulwccyrxkhodrnzgdqguivmaxcnksvvmheavkeycpdufmivelxhekpbrrnjigilgokbpmie";
	double shucmroyelc = 29366;
	string wuincpfzmdxvbga = "wjjuzvmwgzurjmiifakvfbnmhfvjjdmkolcehapwvruhayjeplnbypsugfgldksgkhhpq";
	if (1043 != 1043) {
		int dwlytz;
		for (dwlytz = 28; dwlytz > 0; dwlytz--) {
			continue;
		}
	}
	if (string("dyzpxuswjfrylpdtszulwccyrxkhodrnzgdqguivmaxcnksvvmheavkeycpdufmivelxhekpbrrnjigilgokbpmie") == string("dyzpxuswjfrylpdtszulwccyrxkhodrnzgdqguivmaxcnksvvmheavkeycpdufmivelxhekpbrrnjigilgokbpmie")) {
		int svwgbyrim;
		for (svwgbyrim = 33; svwgbyrim > 0; svwgbyrim--) {
			continue;
		}
	}
	return true;
}

void azvvacl::dmhvqsxokkqernn(int kxpdafpy, bool slwjwsmevadeqk, int bckucaveky, string wxwbtrbor) {
	bool iqwlmiumwpvgw = false;
	double uzomilxaovpff = 20341;
	bool jopwynxgawnswyt = true;
	double rpphrtqga = 20057;
	int lkihqhebdyoesr = 3411;
	int dhuhyvrnf = 1854;
	int vwlzyyyomrhoe = 4961;
	int jngdowpidaytfn = 5385;
	string shsqivo = "tlpsypeubqvxduboopoihkcbsdyqkfwaypwuneaatbgromqmj";
	double iijqsf = 42859;
	if (false == false) {
		int emtochykoj;
		for (emtochykoj = 89; emtochykoj > 0; emtochykoj--) {
			continue;
		}
	}

}

int azvvacl::skzydrugnhvmmgwnlrahljxw(bool vxezmxmhmjmuy, int nlwdgfaqu, int pgmblrhsehky, double kcuzdn, double qaegtwt, int emhzvlmgifvge, bool nhrvbonvaymvsh, double fdivtf) {
	return 93560;
}

void azvvacl::ozgrplytxtxvqximkgjjygu(string xjcplkyulglyy, int yaxqxdpl) {
	string hqsey = "kpkxvdpuid";
	if (string("kpkxvdpuid") != string("kpkxvdpuid")) {
		int mwrfklwgv;
		for (mwrfklwgv = 29; mwrfklwgv > 0; mwrfklwgv--) {
			continue;
		}
	}

}

bool azvvacl::jnemgfzbgmcwbxs(bool dcoocnzoruifwaj, bool ubuirasr, bool oefnr, double ophlqrragjoxxo, bool lxkuomccez, string tnzvdeistest, bool ngalphdrnmzvsrx, double wbmxgckubn) {
	bool lsonctmkuheiwt = false;
	double eocagntmpb = 10114;
	double vjwuysssfudgb = 7097;
	double zjlbqyeliodhh = 19824;
	int zlhfwdavlehtgu = 684;
	double dnrjrzqpwn = 53782;
	int brxkqo = 1046;
	string fgixtxszds = "pmrimhlfydovxzorsreqiucjttkupxwtghyjkaplouufaaujjdypxkkclduiqzfoxypwyhrxkzoudd";
	double ieemoucoutj = 3352;
	return false;
}

void azvvacl::lcfjctjxoqxkpklx(int hdwvjozauufggnq, double ideucxjpmqhdutm, string dvygeilsc) {
	string gorjqezp = "bhgysrkllpbopxu";
	bool qfpvxo = false;
	double gcikxvmyhzoxrw = 9689;
	int qnrbzrzcjh = 3045;
	int onitmathvoftp = 3180;
	int jzvjoyg = 4102;
	int gqvinc = 2597;
	string llnmymake = "tepuzzshlnppzprptrkvmxty";
	string pcihdtzxrhijj = "dzwhgbzpnexqkldybvxguerjybvchhmjbwomdfzkmearectworoifsdalzktedvirysmkzajpsiykzrjkncefyndavgsrpfwlua";
	if (string("tepuzzshlnppzprptrkvmxty") == string("tepuzzshlnppzprptrkvmxty")) {
		int dcsf;
		for (dcsf = 46; dcsf > 0; dcsf--) {
			continue;
		}
	}
	if (2597 != 2597) {
		int dtmeifi;
		for (dtmeifi = 78; dtmeifi > 0; dtmeifi--) {
			continue;
		}
	}
	if (4102 != 4102) {
		int bpq;
		for (bpq = 11; bpq > 0; bpq--) {
			continue;
		}
	}
	if (string("tepuzzshlnppzprptrkvmxty") != string("tepuzzshlnppzprptrkvmxty")) {
		int vt;
		for (vt = 69; vt > 0; vt--) {
			continue;
		}
	}
	if (4102 != 4102) {
		int bjhy;
		for (bjhy = 69; bjhy > 0; bjhy--) {
			continue;
		}
	}

}

string azvvacl::eeeoarohvgohe(bool gahrqp, int kpzgnyf, bool jqjuzam, bool uvofosqdimrqetz) {
	int urkkwrkluhp = 260;
	string ylvuu = "eqywsvfhdnbwiwizto";
	bool zohsus = false;
	double ibdbxwxxfiskcjy = 66579;
	bool omyustrflyjcz = true;
	bool fbtmyiwonurm = false;
	string dyxigclbwb = "yp";
	int fjmonpkss = 1680;
	int yemqh = 653;
	if (653 != 653) {
		int mivobyv;
		for (mivobyv = 84; mivobyv > 0; mivobyv--) {
			continue;
		}
	}
	if (false != false) {
		int sd;
		for (sd = 63; sd > 0; sd--) {
			continue;
		}
	}
	if (string("yp") != string("yp")) {
		int pdmgsv;
		for (pdmgsv = 12; pdmgsv > 0; pdmgsv--) {
			continue;
		}
	}
	if (260 == 260) {
		int wl;
		for (wl = 41; wl > 0; wl--) {
			continue;
		}
	}
	if (false != false) {
		int hblnmhjnxj;
		for (hblnmhjnxj = 0; hblnmhjnxj > 0; hblnmhjnxj--) {
			continue;
		}
	}
	return string("zjkkmmqtecrpwdrxdefj");
}

double azvvacl::bogtsvhvqwzzobrf(string danzhigexse, double xhejqcvzyglwua, string cpohhcs, string qrjpurksvmslpwk) {
	bool rboociq = false;
	string omwsc = "gniyjrvmyvumrqjozxzlifngzxjumzslwaadveucqkyjimyfwomaoejetwdvyqfhwm";
	int sufhvzcvgy = 7107;
	int etmhijwqqgdsybl = 1615;
	bool sxvhlsxs = false;
	if (7107 != 7107) {
		int izy;
		for (izy = 32; izy > 0; izy--) {
			continue;
		}
	}
	return 65965;
}

string azvvacl::whzygfichitfoiyg(double rbkwajrpupmbrk, bool uzmawgthwmzl, int qzdmopkydxvdg, bool eazigfbuhcjgc, bool cuiqg, string mqkdc) {
	string bgpzvrzt = "szvaxqffxihyuonyevjatzmujuatkchbsiswnyqsdeersyhgbcagiwsztftrcgckpyklhtvfvjfmfmsfxf";
	string soiicrpvmfdwobx = "uaaeaurd";
	double euzxb = 56967;
	string pwhlbajkfdetehs = "xptikrjgosonxrjzdbygavffebenvboszqbuknioddrxlkkyttgyyvkcwewojekrlhavp";
	string ohgnkgpocndpw = "crshafmx";
	int zulmlpkdu = 4836;
	double mqqdfjtte = 1586;
	int fketdf = 99;
	if (string("crshafmx") == string("crshafmx")) {
		int cwugsuo;
		for (cwugsuo = 84; cwugsuo > 0; cwugsuo--) {
			continue;
		}
	}
	if (99 == 99) {
		int wiybpoo;
		for (wiybpoo = 40; wiybpoo > 0; wiybpoo--) {
			continue;
		}
	}
	return string("yimdmwfzao");
}

void azvvacl::vjruopowaiqxnzt(int pviozkadprvj, bool ucvvervizhyig, bool vheeqy, double wtzowirroerbxzu, double vsofejzgyh, int svgkusgbneo, bool qeqtjjmnvg, string cyorhcwhmgou, bool rvcylfjsxwlehex) {
	bool cczdqlujtxjeze = false;
	bool fbyuupyicdj = true;
	int gvfvntaywxv = 3777;
	int omxnpvm = 2727;
	string uiszumzfrjfs = "nizulwciyhfujlctnnbanxkdkaoljucmkvutvumjbcdl";
	double sbxweeqvwhbylj = 9796;
	bool zvonzlbkdfcltm = true;
	bool xzlcpcycim = true;
	int iszpzdywviel = 93;

}

int azvvacl::kdnkuhbnwx(bool xfdworivngjkpd, double esrtympiiecf, double nimsqt, double drjxhwz, int dixbtavxqoh) {
	string pelwjtm = "uvhgcjidqj";
	if (string("uvhgcjidqj") != string("uvhgcjidqj")) {
		int tmnkkhmljd;
		for (tmnkkhmljd = 77; tmnkkhmljd > 0; tmnkkhmljd--) {
			continue;
		}
	}
	if (string("uvhgcjidqj") != string("uvhgcjidqj")) {
		int dcuvpiwl;
		for (dcuvpiwl = 82; dcuvpiwl > 0; dcuvpiwl--) {
			continue;
		}
	}
	if (string("uvhgcjidqj") != string("uvhgcjidqj")) {
		int cphotkgiml;
		for (cphotkgiml = 67; cphotkgiml > 0; cphotkgiml--) {
			continue;
		}
	}
	if (string("uvhgcjidqj") == string("uvhgcjidqj")) {
		int umcdjugtat;
		for (umcdjugtat = 85; umcdjugtat > 0; umcdjugtat--) {
			continue;
		}
	}
	return 97435;
}

string azvvacl::kkznoodvrjvc(string fdgki, bool czxvr, int uocti, string wctiyzppnjiee, int kssuypf, bool civwtlz, bool ormkkwkdznajb, double yvgpkbwwlsdtk, double hlvuflydpfperjt, string cewslzobpmrrt) {
	double sxroxnsimmbmppm = 64473;
	double dpdfkrdyp = 374;
	int tusburaylnffjy = 46;
	bool irsenfhs = true;
	if (374 == 374) {
		int ubckyvs;
		for (ubckyvs = 94; ubckyvs > 0; ubckyvs--) {
			continue;
		}
	}
	return string("nygdg");
}

int azvvacl::heihocschxwrfdrvbwplqpvdf(string wylsynjvbktg, string nenmwta, int jqukczjfuf, int eemeitgsbd, int ponpevjwjierwab, bool blfzj, bool srihihlftmabtkk) {
	int rwqio = 5926;
	bool ctlulgmtrtllqy = false;
	double lyghnoccjcfwg = 52870;
	bool dbciozsunsx = true;
	double nrtzivxlote = 74874;
	bool aljvhqb = false;
	bool yygtzsmjnsjgv = true;
	int gstmof = 3106;
	string dkowxbwwnueev = "ctnsbubgmrrnwrdlwdqsmthzygffvoudbgvhkrctvszjozatoczfthgt";
	if (false != false) {
		int fkfdonvaot;
		for (fkfdonvaot = 56; fkfdonvaot > 0; fkfdonvaot--) {
			continue;
		}
	}
	if (false != false) {
		int dqjlco;
		for (dqjlco = 95; dqjlco > 0; dqjlco--) {
			continue;
		}
	}
	if (5926 != 5926) {
		int ela;
		for (ela = 31; ela > 0; ela--) {
			continue;
		}
	}
	return 67294;
}

bool azvvacl::emibaovapmjsmw() {
	bool lycked = false;
	bool mumoitkcncrmn = false;
	double dgmgkuyvtzu = 9403;
	string qocnggfrkf = "gzwfiucnfpjkyzdsqni";
	if (false == false) {
		int rgx;
		for (rgx = 31; rgx > 0; rgx--) {
			continue;
		}
	}
	if (false == false) {
		int gmfawcuoz;
		for (gmfawcuoz = 24; gmfawcuoz > 0; gmfawcuoz--) {
			continue;
		}
	}
	if (9403 == 9403) {
		int elhyicb;
		for (elhyicb = 43; elhyicb > 0; elhyicb--) {
			continue;
		}
	}
	if (false == false) {
		int ca;
		for (ca = 33; ca > 0; ca--) {
			continue;
		}
	}
	return false;
}

double azvvacl::jackiphqdzpb(bool thnua) {
	string asxiesz = "nihbimhqwmachpointwareetlwxzgyvoqwewunmxs";
	double zgoha = 28119;
	int lukrmi = 2395;
	int gtdnsg = 2369;
	string fnpupbmgczpjffb = "mhcmtedhotpxwyquqecvk";
	int dxiavijv = 4383;
	int scildpkmqzqjqh = 2376;
	string edlbzsydlkz = "tzgekrdmogpjofs";
	if (2369 == 2369) {
		int onuzyxkv;
		for (onuzyxkv = 91; onuzyxkv > 0; onuzyxkv--) {
			continue;
		}
	}
	if (4383 == 4383) {
		int xv;
		for (xv = 54; xv > 0; xv--) {
			continue;
		}
	}
	return 54455;
}

azvvacl::azvvacl() {
	this->heihocschxwrfdrvbwplqpvdf(string("zuiwpfsimonkonhynrlrpwinlehhasb"), string("svrsquheauxvcygdsoeteydxczyngxpzegvszyzoitdrsgxqhkmwefrfslrgxjdcsze"), 2001, 1610, 5021, false, false);
	this->emibaovapmjsmw();
	this->jackiphqdzpb(true);
	this->bogtsvhvqwzzobrf(string("fmsvzjoxltlcowlnqfibgiegayiowzqyuvnzsjmluziewxldmqaewrqugjbjkoyaofgqzsomivcqdwpzfrsqluudcvrd"), 21724, string("oqvdubndlqvpmpbvekhephec"), string("lxsycnlpqzzqtdvgdhecwsajakxodfsltiwihedfrpkdazmcim"));
	this->whzygfichitfoiyg(45767, true, 2754, true, false, string("v"));
	this->vjruopowaiqxnzt(2654, false, true, 38605, 21899, 2614, false, string("fwrvdxhbthbpdlgwbcwmwipmjtdempjeifezefazrfpyypimbavkzqhgtvdj"), false);
	this->kdnkuhbnwx(false, 3838, 50000, 36578, 3611);
	this->kkznoodvrjvc(string("srkfdeprdfeujgibojsig"), false, 621, string("bfxgarkjmqhptslanvptymkybbkeukqrksajb"), 5703, true, false, 21220, 8831, string("mzfpwqgyobzexejwtxewwkqtgwmqkqlikykqsjstutyjakmqtln"));
	this->baahcmxczrevrvosz(611, 777, 11533, 1271, 2013);
	this->qsxcgmcfqqvluggqtinanyco(273, false, string("scowkfrnylciieelikigwecqacrepguorivawndnqevpskoxsihavsrijrrfrronazs"), 3618, true, string("ehihbbkpseqrtutrezwdajgnhjlsmfmtfscmievbiclkwzkkkdimcnypilnhhnnjuitfnlzut"), 25353, string("vmhearomvantmvikanwqrpgnyxmkihaczqpoacvpaycksbmmhftbkiaswfbthhbnzerwyixxojihdnxzkluovvtd"), 81185, string("nrgijwwbdvcupohkhchgfitinrnwpcxmoxwnxvjramobpjylovmtpabtfakoivftorwcjhvpkhksbcspah"));
	this->cgriqbrzzv(string("ipcqbkhwmrroojzcvofypvmsgokzlrcagqjvxytsuvlnhouqnzghdybyqxyzuwxnmtylwe"), 3055, string("lkunfqxsdxwudyduqyudsgonvcokvexlcifkvtdludqq"), 2009, 836);
	this->mghqpsprsysbh(string("fyyauoclkrgkiuccenusxxifcdyrzlxrknnydhptunbdxadlygxajfqqlxjkefqqwtigxjpxcmecczkuivgsnhewz"), string("kcojnozduniyfyddoctugwngqbt"), 11332);
	this->dmhvqsxokkqernn(4831, true, 2883, string("uypvbuyywvbetqejfsydnoxzrhnyrtzgoy"));
	this->skzydrugnhvmmgwnlrahljxw(false, 3632, 3304, 7310, 19725, 2104, true, 28471);
	this->ozgrplytxtxvqximkgjjygu(string("hamhwwezihmwapviuhifhghrpsyuqkoryblhpudtvzeilvdhvemtumvzwhovuctsjcunmgvpsmfcimlsfzytihofa"), 906);
	this->jnemgfzbgmcwbxs(false, true, false, 5090, true, string("itdoykbatokspfcd"), false, 4438);
	this->lcfjctjxoqxkpklx(1422, 8133, string("eswhgotjfyovneqvjkxrejvuqbjnxfknoopyw"));
	this->eeeoarohvgohe(false, 659, true, true);
}
