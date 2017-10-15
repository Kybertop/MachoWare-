#include "Interfaces.h"
#include "Utilities.h"

typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

CreateInterface_t EngineFactory = NULL; 
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUISurfaceFactory = NULL;
CreateInterface_t VGUI2Factory = NULL; 
CreateInterface_t MatFactory = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;

void Interfaces::Initialise()
{
	EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");

	char* ClientPredictionInterface = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)Utilities::Memory::FindTextPattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)Utilities::Memory::FindTextPattern("client.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VEngineCvar00");
	char* CHLClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)Utilities::Memory::FindTextPattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)Utilities::Memory::FindTextPattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)Utilities::Memory::FindTextPattern("client.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)Utilities::Memory::FindTextPattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)Utilities::Memory::FindTextPattern("engine.dll","VEngineClient0");

	Client = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	Engine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	Panels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	Surface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	EntList = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	DebugOverlay = (IVDebugOverlay*)EngineFactory(EngineDebugThingInterface, NULL);
	Prediction = (DWORD*)ClientFactory(ClientPredictionInterface, NULL);
	MaterialSystem = (CMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	RenderView = (CVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	ModelRender = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	ModelInfo = (CModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	Trace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	PhysProps = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	CVar = (ICVar*)StdFactory(VEngineCvarName, NULL);
	EventManager = (IGameEventManager2*)EngineFactory("GAMEEVENTSMANAGER002", NULL);


	ClientMode = **(IClientModeShared***)((*(uintptr_t**)Client)[10] + 0x5);

	Globals = **reinterpret_cast< CGlobalVarsBase*** >((*reinterpret_cast< DWORD** >(Client))[0] + 0x1B);
	PDWORD pdwClientVMT = *(PDWORD*)Client;
	pInput = *(CInput**)((*(DWORD**)Client)[15] + 0x1);

	Utilities::Log("Interfaces Ready");
}

namespace Interfaces
{
	IBaseClientDLL* Client;
	IVEngineClient* Engine;
	IPanel* Panels;
	IClientEntityList* EntList;
	ISurface* Surface;
	IVDebugOverlay* DebugOverlay;
	IClientModeShared *ClientMode;
	CGlobalVarsBase *Globals;
	DWORD *Prediction;
	CMaterialSystem* MaterialSystem;
	CVRenderView* RenderView;
	IVModelRender* ModelRender;
	CModelInfo* ModelInfo;
	IEngineTrace* Trace;
	IPhysicsSurfaceProps* PhysProps;
	ICVar *CVar;
	CInput* pInput;
	IGameEventManager2 *EventManager;
	HANDLE __FNTHANDLE;
	IMoveHelper* MoveHelper;
	IPrediction *Prediction1;
	IGameMovement* GameMovement;
};


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kleiems {
public:
	string thqiyeivggoevm;
	bool cmtlryeefd;
	int zvmqyrzqjnifo;
	string imnlna;
	string yxehtmg;
	kleiems();
	bool qqggrmaulwda(bool ybazmdtf, string udfqldusbw, bool iaobyrmoudv, string yfxvmezblzhex, string zfquqacsbq, string qljywnssekgqfi, int qhymelkknpfubit, int wpqzyhooe);
	double qqiqkmgiubncjefxdmvbcln(int kymwlvlfdkrolz);
	string xmzbdczcukvagggytgor(string vscyfallhzq, double lpeqofnisqhp, double xflqondpswbuqs, bool wmudytuin, string rzbthphcmfg, int bxwqjmjxk, string oeghylr);
	string lczhutpajtyr(bool mixbgncqcq, string zamfg, string mdfoltaviowco, string hapdytuyuv, bool mipnwiefm, int bwzgz, int toqsiqz, double ibtvsvovncxo, string krpmjrwju);
	void osfnesgltcaq(bool qoscgrxaoc, int sjdcmmpgkwczws, bool mzseollujdpbfrb, bool fgpusqqggfqglh, int atlpcrvabvjoo, int kumjxlsq, double myvxaejmsb, string ivoeugcdghdj);
	void lvvthnurrbklfbeqfp(double jxrsdijhtlr, string rlzznximdny, string vlszp, bool dhwfmuipbg);
	int isnviypxnjbykllrpdvodnv(bool tnnynxozmhzoyzn, double cgynnocb);
	bool gmiwgsdbpdswgsizqwtgc(int dlxxqvzile, double xaaxufkmpmifdd, int vqtutudkkqjox, string cgmikfybjodirv, bool obdaiptuacaogh, double rgefx);

protected:
	double ztbfdqbipd;
	string qokucnz;
	bool pqbwf;

	int yeltwhngefwzkmz();
	double brjtkyxlbyncfiojsgtnuyrs(string vdppgbgx, bool nxockczgsesamo, double miihoccplwp, string fewmrvfztreibqg, int htfkgacccgiac, int jfxdkxrbgkzipeh, double nkwpqnu);
	bool xribquyfzxkjvblvgg(string swmfajclpgxro, bool fzupgrwuff, string myveizsldae, int bwzuetxrdzt, double qrxuqg, double uxlkkiq);
	double jlwweugqpibulwlte(int cvhdgvmjn, string eizry, string ctjbeziehudyidw, double cbbphwwkyaz, int alnyyazsixzro, string gswyjcbbaah);
	int depjrvnvfo(double snlcofdng, string ngwyvcolflkj, double trbvbjctzqsbt, double hmboueybauxqey, double dcdpzdgcsbsej, int tdexrxm, int yoorymzut, string czhrymtrdum, string ucpsr);
	bool zdxnqerglwfo(int gipsso, bool gnbwvjwxk, int cdjnl, int tnopneovzcqb, string vxhsvupwuijennx, string ryolyvw);
	double atixuyugfkjytfkbraxo(int uftxdkzy, int zwvfcxi, int insnqubammkp, bool fwciplgekyab);

private:
	string ktwgdqrfr;

	int lzwihtwhxyhju(int claflyhamoxpi);
	string ctkbwdibogulckfiz();
	bool bzmsdubplmohnklpf();
	double ubermlfhuvt(bool kiaxxnrog, double oldxsqzcxgqslud, bool anlvl, int knavdklmphg, string wexjr, double ajfbqdi, bool ftvudbzfsnp);
	void xxwxupiggivgnpvo(string nsedav);
	void xjnqkawxzlxhtkuxxbf(bool jufla, double chpedmgn, bool suyjwxw, int gayenaxnlkgeuc, int yepubegrunz, string gmkcbvlflvihix, double ibkdcklm, double oqbbqyxol, int pqytwibqhjp, double lgayctioxdxkr);

};


int kleiems::lzwihtwhxyhju(int claflyhamoxpi) {
	int xfvokpqgivfueya = 633;
	int saemkwmieb = 2634;
	int puwjtcfls = 3448;
	double iklnw = 49933;
	string bjhwrwrqvw = "payxnhylzqfnbbgegthmeddotwxocjfkzafjbsujlirmynzgztunyszxktv";
	if (3448 == 3448) {
		int zr;
		for (zr = 97; zr > 0; zr--) {
			continue;
		}
	}
	if (3448 == 3448) {
		int dqmv;
		for (dqmv = 81; dqmv > 0; dqmv--) {
			continue;
		}
	}
	if (string("payxnhylzqfnbbgegthmeddotwxocjfkzafjbsujlirmynzgztunyszxktv") != string("payxnhylzqfnbbgegthmeddotwxocjfkzafjbsujlirmynzgztunyszxktv")) {
		int ukfx;
		for (ukfx = 63; ukfx > 0; ukfx--) {
			continue;
		}
	}
	return 61861;
}

string kleiems::ctkbwdibogulckfiz() {
	double fnpckdnre = 60930;
	string cfcwy = "kmvpnmdxwvrpmpkgcltamoqvsvmmnmqbwxhmaqxthjymmaxewqd";
	if (string("kmvpnmdxwvrpmpkgcltamoqvsvmmnmqbwxhmaqxthjymmaxewqd") != string("kmvpnmdxwvrpmpkgcltamoqvsvmmnmqbwxhmaqxthjymmaxewqd")) {
		int vuwssnt;
		for (vuwssnt = 96; vuwssnt > 0; vuwssnt--) {
			continue;
		}
	}
	if (60930 != 60930) {
		int hdemwu;
		for (hdemwu = 50; hdemwu > 0; hdemwu--) {
			continue;
		}
	}
	return string("oog");
}

bool kleiems::bzmsdubplmohnklpf() {
	bool cvxdgr = true;
	int znyimetqyrpttqk = 1897;
	string vbpeiihaluwzyrl = "lyiwphmgjhotsneegmueoqrwvdszxxmhpuxoardccfhgpgitudyqfof";
	int dzxmgotskycowwg = 1527;
	string aowpggjpidnn = "jkzuuycgjyziwwupvngwpsjuqnkkwhmzsayzquotmmwwvxljwkxzeaomqku";
	double dddotykrgf = 730;
	int sunjsdjgofvel = 1831;
	string piuyxcsmtjmin = "lhacqbdrymeuqvzpchpixuhktocv";
	if (string("lyiwphmgjhotsneegmueoqrwvdszxxmhpuxoardccfhgpgitudyqfof") == string("lyiwphmgjhotsneegmueoqrwvdszxxmhpuxoardccfhgpgitudyqfof")) {
		int owgauczjna;
		for (owgauczjna = 30; owgauczjna > 0; owgauczjna--) {
			continue;
		}
	}
	if (1897 == 1897) {
		int xurdfh;
		for (xurdfh = 92; xurdfh > 0; xurdfh--) {
			continue;
		}
	}
	if (1831 != 1831) {
		int jwvplnitc;
		for (jwvplnitc = 46; jwvplnitc > 0; jwvplnitc--) {
			continue;
		}
	}
	return false;
}

double kleiems::ubermlfhuvt(bool kiaxxnrog, double oldxsqzcxgqslud, bool anlvl, int knavdklmphg, string wexjr, double ajfbqdi, bool ftvudbzfsnp) {
	double vinmsa = 32780;
	bool ipxbxwd = true;
	double jsqpsoylo = 36051;
	double iucswaeej = 613;
	double vjoisuscpfk = 19595;
	string kpqhbjsyuxpwol = "plwlpfdwqn";
	int awwvoihdnn = 3063;
	bool krnioegwcn = true;
	if (613 == 613) {
		int onlywwhz;
		for (onlywwhz = 100; onlywwhz > 0; onlywwhz--) {
			continue;
		}
	}
	if (19595 == 19595) {
		int jrdjnz;
		for (jrdjnz = 39; jrdjnz > 0; jrdjnz--) {
			continue;
		}
	}
	return 53939;
}

void kleiems::xxwxupiggivgnpvo(string nsedav) {
	double gznvlcvnpvssio = 30998;
	bool bfsecx = false;
	bool ojatdpp = true;
	if (false == false) {
		int rwquzl;
		for (rwquzl = 90; rwquzl > 0; rwquzl--) {
			continue;
		}
	}

}

void kleiems::xjnqkawxzlxhtkuxxbf(bool jufla, double chpedmgn, bool suyjwxw, int gayenaxnlkgeuc, int yepubegrunz, string gmkcbvlflvihix, double ibkdcklm, double oqbbqyxol, int pqytwibqhjp, double lgayctioxdxkr) {
	string dqhky = "twuvltcdk";
	if (string("twuvltcdk") != string("twuvltcdk")) {
		int gd;
		for (gd = 25; gd > 0; gd--) {
			continue;
		}
	}

}

int kleiems::yeltwhngefwzkmz() {
	string cfgjvszw = "tbxofwwmxljxtjxaukasfmepfqgnohdzonyiaxxvruldtstuxsperuhxnrhgpjfkfafwdebeqrbpbzhaqolhldxlpnvrtcepn";
	int lmezrivolwkpm = 3722;
	int njhlczy = 2417;
	string shbmfpmiyeltu = "rjhmtvbildlyckjymovxgzlfwlzbyoomnezyhevpvolbifllqvqdbegcjokyejfmpnsehneprmjaezrcjj";
	double adefwu = 43095;
	bool yjmvq = true;
	int lvdjhgiliqb = 4141;
	bool cskir = true;
	bool irnlvnvkwzoxt = true;
	string kwgcrjaiybnb = "qqalfpdklrnqywxknjqhkceeypdjgze";
	if (3722 != 3722) {
		int pnxtqymw;
		for (pnxtqymw = 74; pnxtqymw > 0; pnxtqymw--) {
			continue;
		}
	}
	if (3722 == 3722) {
		int eiwhaqg;
		for (eiwhaqg = 35; eiwhaqg > 0; eiwhaqg--) {
			continue;
		}
	}
	return 64793;
}

double kleiems::brjtkyxlbyncfiojsgtnuyrs(string vdppgbgx, bool nxockczgsesamo, double miihoccplwp, string fewmrvfztreibqg, int htfkgacccgiac, int jfxdkxrbgkzipeh, double nkwpqnu) {
	bool sdjdglj = true;
	string fsohvfgmtdke = "lfqzccvkicooqzxjzyuzhiypmvfgqwcjhuacnqmngupzabphoyaljytdsrcplyljufzdcqdjxcqjkabiztnruumtcvoszgz";
	string caalvprotkvcrht = "vbxsyhicpxqexzcgmxteutofcralnxhirhzvrztfmstmcgojkndhpnzlcpfolsdgksjfivikvdbpmmhbfyorn";
	string lesguesxx = "cfzwsaodaywjiwlwpxwxkjtdjclxygfsnnumztqcdnrwmffxlyn";
	if (string("vbxsyhicpxqexzcgmxteutofcralnxhirhzvrztfmstmcgojkndhpnzlcpfolsdgksjfivikvdbpmmhbfyorn") != string("vbxsyhicpxqexzcgmxteutofcralnxhirhzvrztfmstmcgojkndhpnzlcpfolsdgksjfivikvdbpmmhbfyorn")) {
		int yiqwcjgcd;
		for (yiqwcjgcd = 44; yiqwcjgcd > 0; yiqwcjgcd--) {
			continue;
		}
	}
	return 80365;
}

bool kleiems::xribquyfzxkjvblvgg(string swmfajclpgxro, bool fzupgrwuff, string myveizsldae, int bwzuetxrdzt, double qrxuqg, double uxlkkiq) {
	bool elgysb = true;
	double hirowkhet = 78304;
	int bqxytnjeu = 2096;
	if (true != true) {
		int vts;
		for (vts = 57; vts > 0; vts--) {
			continue;
		}
	}
	if (78304 != 78304) {
		int kgwmk;
		for (kgwmk = 94; kgwmk > 0; kgwmk--) {
			continue;
		}
	}
	if (true == true) {
		int nnkhju;
		for (nnkhju = 76; nnkhju > 0; nnkhju--) {
			continue;
		}
	}
	return true;
}

double kleiems::jlwweugqpibulwlte(int cvhdgvmjn, string eizry, string ctjbeziehudyidw, double cbbphwwkyaz, int alnyyazsixzro, string gswyjcbbaah) {
	double zashazlxkxs = 15129;
	bool hfrpt = true;
	int vdhyrhdtypcj = 1226;
	if (1226 != 1226) {
		int cghmi;
		for (cghmi = 27; cghmi > 0; cghmi--) {
			continue;
		}
	}
	if (1226 != 1226) {
		int vgsmat;
		for (vgsmat = 44; vgsmat > 0; vgsmat--) {
			continue;
		}
	}
	return 20753;
}

int kleiems::depjrvnvfo(double snlcofdng, string ngwyvcolflkj, double trbvbjctzqsbt, double hmboueybauxqey, double dcdpzdgcsbsej, int tdexrxm, int yoorymzut, string czhrymtrdum, string ucpsr) {
	string duqsqub = "uokbutojsyxfliwriywrducmgbystwbzolotwboozcepdlm";
	string wdabd = "qnxqkzwlpdggaaonpkwrenxrqyhyqamxrztjpdhiablupxnmwrawtzdbpziifjtrckzeyhiixkzrpprghrmglzouonth";
	string bwilmwywtqdlaqr = "lexwordlzwmpecnldjoebgkjnugachencwgajiozqdsrsuvdxwfswblqgfkyadc";
	return 17327;
}

bool kleiems::zdxnqerglwfo(int gipsso, bool gnbwvjwxk, int cdjnl, int tnopneovzcqb, string vxhsvupwuijennx, string ryolyvw) {
	bool hsfspsodlnx = true;
	int rhkztuhtiyluppm = 871;
	int jsduh = 7812;
	double ivhznoxz = 41797;
	int ubdrufottwzlukx = 2616;
	bool hiljeoaxvkvd = true;
	bool ajrgsqty = false;
	int udmdevjyi = 1466;
	int oyjyzjvtrqoal = 3068;
	if (true == true) {
		int oeg;
		for (oeg = 90; oeg > 0; oeg--) {
			continue;
		}
	}
	if (7812 != 7812) {
		int nh;
		for (nh = 0; nh > 0; nh--) {
			continue;
		}
	}
	if (3068 == 3068) {
		int zuskrzjaov;
		for (zuskrzjaov = 50; zuskrzjaov > 0; zuskrzjaov--) {
			continue;
		}
	}
	if (7812 == 7812) {
		int qa;
		for (qa = 4; qa > 0; qa--) {
			continue;
		}
	}
	if (1466 == 1466) {
		int tn;
		for (tn = 86; tn > 0; tn--) {
			continue;
		}
	}
	return true;
}

double kleiems::atixuyugfkjytfkbraxo(int uftxdkzy, int zwvfcxi, int insnqubammkp, bool fwciplgekyab) {
	bool wunbxwuxf = true;
	string dfdgxhjcnwbrlli = "camgtzjpdjlaimraxhcguqlruehkoylirrfegagoazmrnnhttqlexyspsbt";
	bool vyuavedrfk = true;
	string atpkwxbpivz = "devnfhdpvvhvqpilrvnodpctyuyzcrvxjdjqmnzdfizemazyoshgtczwrpktnezwwhmlglrkdjlu";
	double zxgoqmadd = 27802;
	bool bwvwjahg = false;
	double bhgjeqllduchvix = 35772;
	bool dyzjtb = true;
	if (string("camgtzjpdjlaimraxhcguqlruehkoylirrfegagoazmrnnhttqlexyspsbt") != string("camgtzjpdjlaimraxhcguqlruehkoylirrfegagoazmrnnhttqlexyspsbt")) {
		int doy;
		for (doy = 98; doy > 0; doy--) {
			continue;
		}
	}
	return 26227;
}

bool kleiems::qqggrmaulwda(bool ybazmdtf, string udfqldusbw, bool iaobyrmoudv, string yfxvmezblzhex, string zfquqacsbq, string qljywnssekgqfi, int qhymelkknpfubit, int wpqzyhooe) {
	string dxheakbawe = "zyyusbnkcthtyfgwkvaslgsxrwutxrrckakfcgcyvomhhzgdl";
	int mmemnidjlsaqjlv = 264;
	double quyuz = 9092;
	string nyqkocmbey = "wouujnyrwqhvplascrngubqbjvkfnfdjjwboykggpf";
	bool leffsjrkvkz = true;
	int lflgzuclevpfidu = 1333;
	bool basbsqzrhnpfa = true;
	double hcndzhzxh = 26327;
	if (string("zyyusbnkcthtyfgwkvaslgsxrwutxrrckakfcgcyvomhhzgdl") == string("zyyusbnkcthtyfgwkvaslgsxrwutxrrckakfcgcyvomhhzgdl")) {
		int ehprasufw;
		for (ehprasufw = 90; ehprasufw > 0; ehprasufw--) {
			continue;
		}
	}
	return false;
}

double kleiems::qqiqkmgiubncjefxdmvbcln(int kymwlvlfdkrolz) {
	int qluyhlhvktm = 1042;
	string trjinx = "plocgkxetjlkbodularzszyssyhtyqnvgqxxmmtsxqyxxkkvasyydiuyhndwhbqbidhwgslcsa";
	int knjdbfw = 135;
	double rrfairxecq = 8785;
	string risflgbpzwlevij = "xmjjpnhjum";
	string hdxkjwkmvkxec = "rohzbcwbfzkhlkwjcrjumfiydtdtoiwvrluthuqfglinptgntifvwwobexasdczcan";
	bool itrmrymvkmpyiv = true;
	string nqmkmcqcnpcesez = "mdwydaznvdwkarlqbqwvhpxvbbyleewjstkytqbhstlyfjhelhfizxfzdhsjlntqywcyungmoe";
	bool dqnesrfwharvh = false;
	if (135 == 135) {
		int nvxlbj;
		for (nvxlbj = 45; nvxlbj > 0; nvxlbj--) {
			continue;
		}
	}
	if (string("xmjjpnhjum") == string("xmjjpnhjum")) {
		int qvhmrfss;
		for (qvhmrfss = 51; qvhmrfss > 0; qvhmrfss--) {
			continue;
		}
	}
	if (135 == 135) {
		int rc;
		for (rc = 1; rc > 0; rc--) {
			continue;
		}
	}
	if (string("mdwydaznvdwkarlqbqwvhpxvbbyleewjstkytqbhstlyfjhelhfizxfzdhsjlntqywcyungmoe") != string("mdwydaznvdwkarlqbqwvhpxvbbyleewjstkytqbhstlyfjhelhfizxfzdhsjlntqywcyungmoe")) {
		int hvjl;
		for (hvjl = 55; hvjl > 0; hvjl--) {
			continue;
		}
	}
	return 90307;
}

string kleiems::xmzbdczcukvagggytgor(string vscyfallhzq, double lpeqofnisqhp, double xflqondpswbuqs, bool wmudytuin, string rzbthphcmfg, int bxwqjmjxk, string oeghylr) {
	bool xkwashmlwknrbg = false;
	int sgfoctgvpdsmw = 2902;
	bool bxjsxqgkwomc = true;
	string hggkzmmaje = "xgbmbrxelnmdsnfopdqmahwwi";
	if (true == true) {
		int hogutuqs;
		for (hogutuqs = 38; hogutuqs > 0; hogutuqs--) {
			continue;
		}
	}
	if (true == true) {
		int xqhrs;
		for (xqhrs = 69; xqhrs > 0; xqhrs--) {
			continue;
		}
	}
	if (true != true) {
		int aizb;
		for (aizb = 83; aizb > 0; aizb--) {
			continue;
		}
	}
	if (string("xgbmbrxelnmdsnfopdqmahwwi") != string("xgbmbrxelnmdsnfopdqmahwwi")) {
		int db;
		for (db = 34; db > 0; db--) {
			continue;
		}
	}
	if (true != true) {
		int osehwws;
		for (osehwws = 92; osehwws > 0; osehwws--) {
			continue;
		}
	}
	return string("zszinkwgoodpwyjr");
}

string kleiems::lczhutpajtyr(bool mixbgncqcq, string zamfg, string mdfoltaviowco, string hapdytuyuv, bool mipnwiefm, int bwzgz, int toqsiqz, double ibtvsvovncxo, string krpmjrwju) {
	double wnofkwdntsp = 15961;
	int ycnyhdda = 2128;
	int sfbnsuudyjnlge = 5325;
	int jlcpksbg = 4025;
	bool bvjqvufzvmcf = false;
	double brqelhr = 51594;
	return string("ikfovcgtmdu");
}

void kleiems::osfnesgltcaq(bool qoscgrxaoc, int sjdcmmpgkwczws, bool mzseollujdpbfrb, bool fgpusqqggfqglh, int atlpcrvabvjoo, int kumjxlsq, double myvxaejmsb, string ivoeugcdghdj) {
	string ejxzlkwd = "ytskcspgambvrvjfnfifwjcufkhszmnbbdkhxhlixfifpeqclaadkpladciejcevvvyhnxaiywumsklwtvismsoxbrfmqoeoyvcc";
	bool blovdncpn = true;
	string klrkbc = "w";
	double mmditjwkughqbge = 6945;
	string opcbpr = "jewfowwktqwaejbmsnqebvzastjwtvhptcwjwgjayejoblzjwjcbvnaaxwqrfkaugiutrxhxkeaxxblbgmro";
	string zqwaufihtcd = "fclpwbrlislkbjxmnigvollhcoyjwpoxfxbecbhrapjtwffikse";
	if (6945 != 6945) {
		int nhvs;
		for (nhvs = 7; nhvs > 0; nhvs--) {
			continue;
		}
	}

}

void kleiems::lvvthnurrbklfbeqfp(double jxrsdijhtlr, string rlzznximdny, string vlszp, bool dhwfmuipbg) {
	double vciurz = 6570;
	int mkmghmffs = 319;
	int oxbujpkmvokvhyi = 4335;
	double vqxyse = 19245;
	double ovuaneteqglg = 12830;
	int qtnpuwobsd = 489;
	double rrwksnov = 48249;
	double pcfvridalclwt = 61228;
	int vdkhdrqnwgvx = 23;
	bool ncyqomyiwonax = true;
	if (61228 == 61228) {
		int dfjduyr;
		for (dfjduyr = 16; dfjduyr > 0; dfjduyr--) {
			continue;
		}
	}
	if (4335 != 4335) {
		int iadafxzzt;
		for (iadafxzzt = 4; iadafxzzt > 0; iadafxzzt--) {
			continue;
		}
	}

}

int kleiems::isnviypxnjbykllrpdvodnv(bool tnnynxozmhzoyzn, double cgynnocb) {
	bool vpeqnt = false;
	int hrakwop = 2152;
	string pilhbljncautt = "dxswlzzohjkqlzyqeamqtqidspvdmyjdzv";
	int bhdnir = 6866;
	double xoaufelv = 19193;
	int tgmkgfvtgjuf = 7797;
	double bshbaff = 16765;
	bool aexhqey = false;
	double mkkgtkmursgci = 91976;
	string xtidfjbobzonwe = "oneivyapiwswlempeoxtqqcpuzfyjnsjthdbomtdjhyjiccztuajov";
	return 39372;
}

bool kleiems::gmiwgsdbpdswgsizqwtgc(int dlxxqvzile, double xaaxufkmpmifdd, int vqtutudkkqjox, string cgmikfybjodirv, bool obdaiptuacaogh, double rgefx) {
	string nladtw = "ojxmsbhhfkklacjmnpjgkttrvsczjmod";
	string vrdroixksqfgtw = "foaasthunbuifqmqegovfztxqzyfohcpdfdxcjrkwymnylsjvxehpriydlrkyikpnkantownagmwp";
	double zznlvwjaa = 7801;
	int snuboh = 3956;
	bool zukupiepcxccrd = false;
	double hhwbdlftvxdbvjt = 36285;
	double vwgiejxjo = 28628;
	double ozmfoeooccatzq = 3571;
	bool madefxgsgw = false;
	return false;
}

kleiems::kleiems() {
	this->qqggrmaulwda(false, string("qnerqwjpczrvaufwlaywkwqaicywycagnxtbcpubxyghywfqgsovsfbphy"), true, string("btucfyhrzewjqa"), string("vnwhkjofhthlhoewpxzbaalwx"), string("nognoqtzaxpcagfzugolvseujfxrpzulgglqsyqv"), 1118, 2028);
	this->qqiqkmgiubncjefxdmvbcln(4568);
	this->xmzbdczcukvagggytgor(string("khbfuqmimvdqhwxsjbkkjnhdtvisfsmcsqdptvkedzranehsddazdhbfihceloaxbs"), 36016, 11578, true, string("pbjnaqhxzpycjpysgbespcmymxbdvmjuorcn"), 1325, string("bzdonbiimtlpbptfgyaqwugooeagdsiuglsmcplyjfwyilxugfustxaoz"));
	this->lczhutpajtyr(false, string("bylhkerauvrftfbyazwkepasbzriidocuhxoljrsgpgsilcnubplpfljbjwhgcossuiwe"), string("ihbouplmbuyqtzfobpgorppmvvzjovnozctivnbzzgkrfelmgwhgjcprurtqwzqljnhefhb"), string("ihtwhprbetcgekcezhmcgsfuibtrvzolnlfcjtqunzlgxsvehysaueambdsxsba"), true, 2935, 2369, 76147, string("gegckkqmwvtvtvwvyxxbcvouypoklori"));
	this->osfnesgltcaq(true, 6519, false, false, 239, 1602, 47045, string("vvdjcirqfardztogkqmkwytcfrgzixldykxdicjf"));
	this->lvvthnurrbklfbeqfp(80546, string("ytxzcaklqyalkejbn"), string(""), true);
	this->isnviypxnjbykllrpdvodnv(true, 6106);
	this->gmiwgsdbpdswgsizqwtgc(4204, 49949, 594, string("alwecmxjffhvclsesztcpexcrliqitpecdabmhopemsktmbnrvhpndzeugzuyiro"), false, 5770);
	this->yeltwhngefwzkmz();
	this->brjtkyxlbyncfiojsgtnuyrs(string("mrqjgoqxhvewwezexszhzjfgyzwfsoymvxpcrtmtsyzmklhwwtvotavbyaqirxmlezzogznmmcpcnbxkdtdgly"), true, 7758, string("ktrzbfvwdljnpoxpvkjydje"), 5066, 4717, 24154);
	this->xribquyfzxkjvblvgg(string("spclxtkzawezrwga"), true, string("kjdigmmffloenxwjwikbszhaoocirndlzlymwwwovsyukwvrphcjosojcmpenblincpr"), 2741, 58985, 27390);
	this->jlwweugqpibulwlte(4727, string("moylawefowisuq"), string("slhzgbyuarpkcoeayjkbdvuhktswlnrbflrodufvo"), 31228, 1963, string("avrmbcmiqisampqxpythfigvrnbnavlisnfcczvlhfecrraaglpxgmbweiksk"));
	this->depjrvnvfo(4757, string("savniueerugvbgnrcpxclwpnopzxyrsywtewsqpbdsqyogwdvrqsobmdeybcykmyg"), 23239, 88320, 18344, 6791, 1130, string("zlqtyerhdzseljevqpbnxrgwhqvrokilifpoyxzpceyotltjeknfsaxmh"), string("ycgjszznhhhimrwpgdikalaiihusqufdtzlspgdtcmnyajuldgrmnuuilpjqadmstsbew"));
	this->zdxnqerglwfo(1664, true, 4335, 648, string("olqoxghydsultwultlkddnvgedrmttcgoswoeuzobnpupuxrinprzdojmkv"), string("gxdkfvcollcskxbykcuadzpcnfgkubkivbzwyxphvmlmkgbxehfnc"));
	this->atixuyugfkjytfkbraxo(1584, 1148, 3642, false);
	this->lzwihtwhxyhju(1677);
	this->ctkbwdibogulckfiz();
	this->bzmsdubplmohnklpf();
	this->ubermlfhuvt(true, 52582, false, 2139, string("agbvehallxczmeqhybuxxvxkpaxznvgqoabynujigcqyhvfcburqqfuuxhdwzhfqnkjor"), 7378, false);
	this->xxwxupiggivgnpvo(string("uqsgpurdysnir"));
	this->xjnqkawxzlxhtkuxxbf(true, 33460, true, 4839, 347, string("gbpllbdpzmrzjyeaxdrvxfvhmjuvgqpcqjyrt"), 11177, 3666, 778, 35133);
}
