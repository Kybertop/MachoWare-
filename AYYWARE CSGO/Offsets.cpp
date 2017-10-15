#include "Offsets.h"
#include "UTIL Functions.h"

#define strenc( s ) ( s )

void Offsets::Initialise()
{
	// Modules
	Modules::Client = Utilities::Memory::WaitOnModuleHandle("client.dll");
	Modules::Engine = Utilities::Memory::WaitOnModuleHandle("engine.dll");
	Modules::VGUI2 = Utilities::Memory::WaitOnModuleHandle("vgui2.dll");
	Modules::VGUISurface = Utilities::Memory::WaitOnModuleHandle("vguimatsurface.dll");
	Modules::Material = Utilities::Memory::WaitOnModuleHandle("materialsystem.dll");
	Modules::VPhysics = Utilities::Memory::WaitOnModuleHandle("vphysics.dll");
	Modules::Stdlib = Utilities::Memory::WaitOnModuleHandle("vstdlib.dll");


#pragma region VTables
	VMT::CHL_GetAllClasses = 8;

	VMT::ModelInfo_GetModelName = 3;
	VMT::ModelInfo_GetStudiomodel = 30;
	VMT::Engine_GetScreenSize = 5;
	VMT::Engine_GetPlayerInfo = 8;
	VMT::Engine_GetLocalPlayer = 12;
	VMT::Engine_Time = 14;
	VMT::Engine_GetViewAngles = 18;

	VMT::Panel_GetName = 36;
	VMT::Panel_PaintTraverse = 41;

	VMT::Surface_DrawSetColorA = 14;
	VMT::Surface_DrawSetColorB = 15;
	VMT::Surface_DrawFilledRect = 16;
	VMT::Surface_DrawOutlinedRect = 18;
	VMT::Surface_DrawLine = 19;
	VMT::Surface_DrawSetTextFont = 23;
	VMT::Surface_GetTextSize = 79;
	VMT::Surface_DrawOutlinedCircle = 103;
	VMT::Surface_SurfaceGetCursorPos = 66;
	VMT::Surface_DrawTexturedPolygon = 106;

	VMT::Material_GetName = 0;
	VMT::Material_SetMaterialVarFlag = 30;
	VMT::Material_GetMaterialVarFlag = 31;
	VMT::Material_AlphaModulate = 28;
	VMT::Material_ColorModulate = 29;
	VMT::Engine_SetViewAngles = 19;
	VMT::Engine_GetMaxClients = 20;
	VMT::Engine_IsConnected = 27;
	VMT::Engine_IsInGame = 26;
	VMT::Engine_WorldToScreenMatrix = 37;
	VMT::Engine_ClientCmd_Unrestricted = 114;
	VMT::Material_IncrementReferenceCount = 14;
	
	VMT::Surface_DrawSetTextColorA = 24;
	VMT::Surface_DrawSetTextColorB = 25;
	VMT::Surface_DrawSetTextPos = 26;
	VMT::Surface_DrawPrintText = 28;
	VMT::Surface_DrawSetTextureRGBA = 37;
	VMT::Surface_DrawSetTexture = 38;
	VMT::Surface_CreateNewTextureID = 43;
	VMT::Surface_FontCreate = 71;
	VMT::Surface_SetFontGlyphSet = 72;
	VMT::MaterialSystem_FindMaterial = 84;
	VMT::MaterialSystem_CreateMaterial = 83;
	
	VMT::ModelRender_ForcedMaterialOverride = 1;
	VMT::ModelRender_DrawModelExecute = 21;

	VMT::RenderView_SetBlend = 4;
	VMT::RenderView_SetColorModulation = 6;

#pragma endregion Contains the VTable Indexs


	Functions::KeyValues_KeyValues = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx");
	Functions::KeyValues_KeyValues += 7;
	Functions::KeyValues_KeyValues = Functions::KeyValues_KeyValues + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_KeyValues + 1) + 5;

	Functions::KeyValues_LoadFromBuffer = Utilities::Memory::FindPatternV2("client.dll", "E8 ? ? ? ? 80 7D F8 00 ? ? 85 DB");
	Functions::KeyValues_LoadFromBuffer = Functions::KeyValues_LoadFromBuffer + *reinterpret_cast< PDWORD_PTR >(Functions::KeyValues_LoadFromBuffer + 1) + 5;

	Functions::dwCalcPlayerView =  Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x84\xC0\x75\x08\x57\x8B\xCE\xE8\x00\x00\x00\x00\x8B\x06", "xxxxxxxx????xx");

	Functions::dwGetPlayerCompRank = GameUtils::FindPattern1(strenc("client.dll"), strenc("55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ? "));

	Functions::IsReadySig = GameUtils::FindPattern1(strenc("client.dll"), strenc("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B 8E"));

	Functions::dwIsReady = GameUtils::FindPattern1(strenc("client.dll"), strenc("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 8B BE"));

	Functions::GetWeaponData = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\xB8\x00\x00\x00\x00\x57", "xxxxx????x????x");

	Utilities::Log("Offsets/Indexes Up to Date");
}

namespace Offsets
{
	// Addresses of loaded game modules
	namespace Modules
	{
		DWORD Client;
		DWORD Engine;
		DWORD VGUI2;
		DWORD VGUISurface;
		DWORD Material;
		DWORD VPhysics;
		DWORD Stdlib;
	};

	IsReadyFn IsReady;

	// Virtual Method Table Indexes
	namespace VMT
	{
		//CHL Client
		DWORD CHL_GetAllClasses;

		//Engine Client
		DWORD Engine_GetScreenSize;
		DWORD Engine_GetPlayerInfo;
		DWORD Engine_GetLocalPlayer;
		DWORD Surface_DrawSetTextPos;
		DWORD Surface_DrawPrintText;
		DWORD Surface_DrawSetTextureRGBA;
		DWORD Surface_DrawSetTexture;
		DWORD Surface_CreateNewTextureID;
		DWORD Surface_FontCreate;
		DWORD Surface_SetFontGlyphSet;
		DWORD Engine_Time;
		DWORD Engine_GetViewAngles;
		DWORD Engine_SetViewAngles;
		DWORD Engine_GetMaxClients;
		DWORD Engine_IsConnected;
		DWORD Engine_IsInGame;
		DWORD Engine_WorldToScreenMatrix;
		DWORD Engine_GetNetChannelInfo;
		DWORD Engine_ClientCmd_Unrestricted;

		// Panels
		DWORD Panel_GetName;
		DWORD Panel_PaintTraverse;

		// Surface
		DWORD Surface_DrawSetColorA;
		DWORD Surface_DrawSetColorB;
		DWORD Surface_DrawFilledRect;
		DWORD Surface_DrawOutlinedRect;
		DWORD Surface_DrawLine;
		DWORD Surface_DrawSetTextFont;
		DWORD Surface_DrawSetTextColorA;
		DWORD Surface_DrawSetTextColorB;
		DWORD Surface_GetTextSize;
		DWORD Surface_DrawOutlinedCircle;
		DWORD Surface_SurfaceGetCursorPos;
		DWORD Surface_DrawTexturedPolygon;

		DWORD Material_GetName;
		DWORD Material_SetMaterialVarFlag;
		DWORD Material_GetMaterialVarFlag;
		DWORD Material_AlphaModulate;
		DWORD Material_ColorModulate;
		DWORD Material_IncrementReferenceCount;

		DWORD MaterialSystem_FindMaterial;
		DWORD MaterialSystem_CreateMaterial;

		DWORD ModelRender_ForcedMaterialOverride;
		DWORD ModelRender_DrawModelExecute;

		DWORD ModelInfo_GetModelName;
		DWORD ModelInfo_GetStudiomodel;

		DWORD RenderView_SetBlend;
		DWORD RenderView_SetColorModulation;

		// Weapon entities
		DWORD Weapon_GetSpread;
	};

	// Addresses of engine functions to call
	namespace Functions
	{
		DWORD KeyValues_KeyValues;
		DWORD KeyValues_LoadFromBuffer;
		DWORD dwCalcPlayerView;
		DWORD dwGetPlayerCompRank;
		DWORD dwIsReady;
		DWORD GetWeaponData;
		DWORD IsReadySig;
	};

};


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class fawpeep {
public:
	int slvbvwqciuibry;
	string rrkcdeyzmsbtenx;
	string bhidmt;
	string kencp;
	bool iuusjjtja;
	fawpeep();
	bool jkawykpvmaktwjquxcukoa(bool oprwuanmtpwva, bool aryxroropvcqkd, int jaxxbezvkqc, bool lfwtu);
	double tuiwykvjxziowvcqifxsne(bool cgfqkuufbx, string fnexekycm, int eplznnovjczxwb, double kvqymquz);
	bool yjzemwhugz(bool zcgjyxbmou, int dpkqa, string abgzcsisf, double mblpkus, int nyuofboqpufnor, double acwuqbgrmjhqgnn, double ysgtqvact, string qiwxf, int xjllwl, double ljehjz);
	double bglctyyemizfue(int rmosvi, bool oualpxv, double tdyysvryomyfm, string onxmsvqbqn);
	string hbveassvfz(bool udldkqqvsg, string mnxngsegfyzbdht, int fmgthloro, int gskxolfdznot, bool quvbdnxf, int rpkqoryl, string klfmfxo);
	int tkcixlqgaaerusdyhwyqhae(int awncaivfdzf, bool acidlwunik, int oaqlqnolioisrei, int gchlzhcvywv, string xrxinfqlkvqwq, string eclilyhrwclzwye);
	bool jceqhimuhwrljzk(bool bypzmgxegm, int gylwtcrv, int apseqfpfiqvsb, bool dtbxjwcnh, bool dwsusbmyhaljp);
	void lppogawjhtnohstlcc(bool zmguhbtht, string yvwcvpvlzons, string cwagzpwdmlepjy, double symnijvy, string fytfcmkiqaqtsy, int lzumsdrwzsrqads);

protected:
	int mqtfumb;

	bool uvvpvdkbgpyyms(int xsvhqkpghyngq, string udjyaqnfqarka);
	double latinlbneexljtrzielqw();
	string okufcwehcrjhxjpvyhnz(bool pnxjogxulozhs, double oassfr, string dxsptndpnzyfe);
	string edzziczwyivtkhwsimesw(string cogqlkjsdfgzg);
	double sryhpjbieyxqdysmv(double lqpqexdjywiax, bool javjcztm, int hceijoqz, string yodpxxetrxihdg, double ykkerprhfthehbr, bool hakfjvyu, int ujthrv, string galagrhwaqcucl, int hquohmqjspfdgkz);
	int hhgagqrnkbxpgzezzkhodxt(bool rzbssg, double sjtjwirjsmsscpv, double obocoiixcium, double pkuochyb, bool qhecz, bool aykteqmxijfyy, int thhfudvcbjkp, double mftdblhou, int ivyiby);
	double gkctmflzuwog(string wzxgcbgivuidb, string havupw, string zjpmj, double batqjlkinnsd, int rhzvkg);
	string eqryuytkcmsonyuoxupxq(int ggmcpkd, bool ihylxypipb, string dxfebyn, int mgdvqyybtqpp, int hndfhujc, bool eevhzysx);
	double rsqfthseoxctbtqmujovr(int ohwovkuio, bool wtiaoktxdpywbcc, bool qrvgbtzuv);
	int mxjrurehigkqvxe(int gmjpzsibspju, int ytqsdzlmuqmetjl, string cjkrwxjejmc, int siibmzxyjalspir, bool orxctnaxm);

private:
	int pdsxnhlw;
	string rihfubpritia;
	bool iiypolzhkrhypw;

	string fyqmypgvogqteodazmf(string uclcz, double gwxqugftb, int yiiumxuee);
	double nbqhcmblhjkkvjmhyduyioyu(string kwpsh, double eqzhnq, int qrbzlytkzvf, double mukkycszpad, double dlyodbvhevcub, int iwrtqxofl, string goejikzq, int gduhvmbryieed, int uenihukdpt, int whkbekbh);
	double xtegdsalfvf(double ziojvtroryludj, double exhhyetlcexk, double rkepbyzjbnodzsz, bool awdhsjedimstsvl, int ldxcx);
	void mtvmnykceopmt(double uecfq, double tarmrkhx, double hcmubgqncdqyxxz, double ubguyfocp, string vylykib, double nfzsseban, string njxtbfgngeuee, string djpuynst);
	void foptdpqgdpj(string npzng, int ecvplfkdmgski);
	string sbnsnhdzowispfjcigxudcbum(string okyisulqlqvxy, double sjlfavfhsutu, bool ombwsan, int eihvuw, double mzjmom);
	double kcwgnrqpyfd();
	void atcnnqwhmalfszgjlfpxabfm();
	double ifmhlriwajtf();

};


string fawpeep::fyqmypgvogqteodazmf(string uclcz, double gwxqugftb, int yiiumxuee) {
	double xtpyfvnnyuvrf = 2937;
	bool llwxuzlpcrttfd = false;
	string yvxsrlklaelkc = "ftaeo";
	double ykdacgxewiflfy = 50699;
	bool jycceodleggsuux = true;
	double vstyhkrkai = 5576;
	bool ldbivxukjcttyvs = false;
	if (5576 == 5576) {
		int cwyutaj;
		for (cwyutaj = 44; cwyutaj > 0; cwyutaj--) {
			continue;
		}
	}
	return string("dboxefoygrclfmhpw");
}

double fawpeep::nbqhcmblhjkkvjmhyduyioyu(string kwpsh, double eqzhnq, int qrbzlytkzvf, double mukkycszpad, double dlyodbvhevcub, int iwrtqxofl, string goejikzq, int gduhvmbryieed, int uenihukdpt, int whkbekbh) {
	int enxnyurky = 7322;
	string beooaboqytwfv = "ehdepexffoobkbrppymfxxgpqiltrbshjaskwdgxqkvghgqsuufcxiopcnjokzemtkdgj";
	int fpqqxp = 1441;
	double bntpko = 36913;
	bool rugdw = false;
	int shxkpsucjhhyh = 374;
	if (1441 != 1441) {
		int otatwhrfsc;
		for (otatwhrfsc = 33; otatwhrfsc > 0; otatwhrfsc--) {
			continue;
		}
	}
	if (7322 != 7322) {
		int ntbc;
		for (ntbc = 62; ntbc > 0; ntbc--) {
			continue;
		}
	}
	if (36913 != 36913) {
		int visf;
		for (visf = 64; visf > 0; visf--) {
			continue;
		}
	}
	return 39633;
}

double fawpeep::xtegdsalfvf(double ziojvtroryludj, double exhhyetlcexk, double rkepbyzjbnodzsz, bool awdhsjedimstsvl, int ldxcx) {
	string kdyssrajppgb = "ajrvjzizofugdjlvljwucpuapqtbestmpbrnbvjpweympcsqbiycnatnceumbojcjnwthvncxhcatecjnlzhatjiisqqayntg";
	bool pzvtvgdjorp = false;
	bool jhdwkuxr = false;
	int uzwbpa = 3741;
	string ldqoidd = "numlykbxdbtbgorvndlnbqudsudwtllpjiygkluijaemndxjysyhhmidlwtr";
	bool fnlovbgop = false;
	string hfyvp = "hiqborbrgxnvjev";
	string olwashnt = "oiitsblfdwjnjasdqfpbfzqlahnavofsivmrhyehzfbwctmxhtdwehpvoagabwvouetbmfzbovuduvmeclmbdqxo";
	bool apacpgykfm = false;
	string apdeju = "cjxqpsspqztkrkwonwltfasywvfdjrmtzvfzlyohxvntsqvmesfscrpzwehjibousfgkpyolty";
	if (string("ajrvjzizofugdjlvljwucpuapqtbestmpbrnbvjpweympcsqbiycnatnceumbojcjnwthvncxhcatecjnlzhatjiisqqayntg") == string("ajrvjzizofugdjlvljwucpuapqtbestmpbrnbvjpweympcsqbiycnatnceumbojcjnwthvncxhcatecjnlzhatjiisqqayntg")) {
		int mqoj;
		for (mqoj = 72; mqoj > 0; mqoj--) {
			continue;
		}
	}
	if (string("oiitsblfdwjnjasdqfpbfzqlahnavofsivmrhyehzfbwctmxhtdwehpvoagabwvouetbmfzbovuduvmeclmbdqxo") != string("oiitsblfdwjnjasdqfpbfzqlahnavofsivmrhyehzfbwctmxhtdwehpvoagabwvouetbmfzbovuduvmeclmbdqxo")) {
		int eryye;
		for (eryye = 29; eryye > 0; eryye--) {
			continue;
		}
	}
	if (false != false) {
		int tub;
		for (tub = 28; tub > 0; tub--) {
			continue;
		}
	}
	if (false == false) {
		int ikacjt;
		for (ikacjt = 61; ikacjt > 0; ikacjt--) {
			continue;
		}
	}
	if (string("ajrvjzizofugdjlvljwucpuapqtbestmpbrnbvjpweympcsqbiycnatnceumbojcjnwthvncxhcatecjnlzhatjiisqqayntg") == string("ajrvjzizofugdjlvljwucpuapqtbestmpbrnbvjpweympcsqbiycnatnceumbojcjnwthvncxhcatecjnlzhatjiisqqayntg")) {
		int durj;
		for (durj = 88; durj > 0; durj--) {
			continue;
		}
	}
	return 89075;
}

void fawpeep::mtvmnykceopmt(double uecfq, double tarmrkhx, double hcmubgqncdqyxxz, double ubguyfocp, string vylykib, double nfzsseban, string njxtbfgngeuee, string djpuynst) {
	string txhvriyg = "zshlcvdnzmhxrplbtmkqohgzmzzwiifotnvodccbrbqtuqxzdcidufpicygnhbszrcgwqhzqtuaertukiilueisridamd";
	if (string("zshlcvdnzmhxrplbtmkqohgzmzzwiifotnvodccbrbqtuqxzdcidufpicygnhbszrcgwqhzqtuaertukiilueisridamd") != string("zshlcvdnzmhxrplbtmkqohgzmzzwiifotnvodccbrbqtuqxzdcidufpicygnhbszrcgwqhzqtuaertukiilueisridamd")) {
		int qpptjs;
		for (qpptjs = 84; qpptjs > 0; qpptjs--) {
			continue;
		}
	}
	if (string("zshlcvdnzmhxrplbtmkqohgzmzzwiifotnvodccbrbqtuqxzdcidufpicygnhbszrcgwqhzqtuaertukiilueisridamd") != string("zshlcvdnzmhxrplbtmkqohgzmzzwiifotnvodccbrbqtuqxzdcidufpicygnhbszrcgwqhzqtuaertukiilueisridamd")) {
		int eikpaq;
		for (eikpaq = 88; eikpaq > 0; eikpaq--) {
			continue;
		}
	}

}

void fawpeep::foptdpqgdpj(string npzng, int ecvplfkdmgski) {
	bool skawpazhgxckob = false;
	double hbfkhygayiucwv = 11476;
	double ethkeng = 44766;
	bool nmzgudeaoucd = true;
	int zqwjskbtkallj = 2476;
	int wogaqr = 2068;
	bool twjdj = true;
	int hcvqzpb = 1543;
	if (11476 != 11476) {
		int xgqu;
		for (xgqu = 65; xgqu > 0; xgqu--) {
			continue;
		}
	}
	if (true != true) {
		int vcbs;
		for (vcbs = 36; vcbs > 0; vcbs--) {
			continue;
		}
	}
	if (true != true) {
		int oqppgyon;
		for (oqppgyon = 51; oqppgyon > 0; oqppgyon--) {
			continue;
		}
	}

}

string fawpeep::sbnsnhdzowispfjcigxudcbum(string okyisulqlqvxy, double sjlfavfhsutu, bool ombwsan, int eihvuw, double mzjmom) {
	int rmkabzrgues = 2107;
	if (2107 != 2107) {
		int igsspgap;
		for (igsspgap = 4; igsspgap > 0; igsspgap--) {
			continue;
		}
	}
	if (2107 != 2107) {
		int srt;
		for (srt = 92; srt > 0; srt--) {
			continue;
		}
	}
	if (2107 == 2107) {
		int yplixodc;
		for (yplixodc = 97; yplixodc > 0; yplixodc--) {
			continue;
		}
	}
	if (2107 == 2107) {
		int wwkfomjp;
		for (wwkfomjp = 55; wwkfomjp > 0; wwkfomjp--) {
			continue;
		}
	}
	if (2107 == 2107) {
		int ioducwnc;
		for (ioducwnc = 31; ioducwnc > 0; ioducwnc--) {
			continue;
		}
	}
	return string("llutoq");
}

double fawpeep::kcwgnrqpyfd() {
	int xufnpaykgfatfr = 1843;
	double gblwz = 16525;
	bool clvivq = true;
	string usyetcwplgm = "qiqkqghwwkkweebbzupnxxhabarzkezmbwavjgfuxynwxguuartvbfbxordrpymvxfhwwvmvsgtdbgfsvxqmxhcvbcbxm";
	int mbfnn = 1257;
	if (true == true) {
		int op;
		for (op = 43; op > 0; op--) {
			continue;
		}
	}
	if (true != true) {
		int sjkl;
		for (sjkl = 25; sjkl > 0; sjkl--) {
			continue;
		}
	}
	if (1257 == 1257) {
		int gvqlpbeyf;
		for (gvqlpbeyf = 34; gvqlpbeyf > 0; gvqlpbeyf--) {
			continue;
		}
	}
	if (true != true) {
		int cuakwzyktg;
		for (cuakwzyktg = 85; cuakwzyktg > 0; cuakwzyktg--) {
			continue;
		}
	}
	return 79059;
}

void fawpeep::atcnnqwhmalfszgjlfpxabfm() {
	bool yqbbwqwop = false;
	double dbauhxgarx = 27652;
	string mkjsezomc = "mogchefegjjghvqwmihcujsltbudsiargrlgtujovopzeytnzlxheyotvtckebuxbmladofdwqziyoqaaafmkwwrybgomiszk";
	int ghjfouqajkrarwk = 1948;
	double ywlulcbwgedqblh = 2547;
	int pwagzyhzzlkuocw = 1746;
	double voseebknbgf = 8970;
	string ebeepiakguoi = "bajvvkzkwekbklthcmzlpizopnqxjnjcfjtinbctvnpupzsivniyhcxzigpmsyioxemgb";
	string gofucpfprpoxtap = "quwfghoimwgplqkwkrrgvwlwgorjivblkemxemdzbxcblgbaeqbzlvnumtwemcj";
	int ripikdhb = 1601;
	if (1948 == 1948) {
		int jzkb;
		for (jzkb = 86; jzkb > 0; jzkb--) {
			continue;
		}
	}
	if (string("quwfghoimwgplqkwkrrgvwlwgorjivblkemxemdzbxcblgbaeqbzlvnumtwemcj") != string("quwfghoimwgplqkwkrrgvwlwgorjivblkemxemdzbxcblgbaeqbzlvnumtwemcj")) {
		int gz;
		for (gz = 91; gz > 0; gz--) {
			continue;
		}
	}
	if (string("quwfghoimwgplqkwkrrgvwlwgorjivblkemxemdzbxcblgbaeqbzlvnumtwemcj") != string("quwfghoimwgplqkwkrrgvwlwgorjivblkemxemdzbxcblgbaeqbzlvnumtwemcj")) {
		int krvqvoxd;
		for (krvqvoxd = 38; krvqvoxd > 0; krvqvoxd--) {
			continue;
		}
	}
	if (2547 == 2547) {
		int lrmqlj;
		for (lrmqlj = 87; lrmqlj > 0; lrmqlj--) {
			continue;
		}
	}

}

double fawpeep::ifmhlriwajtf() {
	string ewpaurf = "fefvthqaong";
	string mptqnwg = "ztgaykwmnekyogajmeuvhrmkejz";
	string ztqgom = "gjrzvqyewuenihsrhcmrffhxxjdfifbgmyjeqtpbmmeuzezzenazafrevnvfdegnngyrtpbjoplbiziedshyqjzefq";
	double krqbvlcoppkyh = 14946;
	double eptvasekrsyte = 40043;
	int mavxiofrgu = 2938;
	double cfdnme = 51339;
	if (string("ztgaykwmnekyogajmeuvhrmkejz") == string("ztgaykwmnekyogajmeuvhrmkejz")) {
		int dnouqm;
		for (dnouqm = 59; dnouqm > 0; dnouqm--) {
			continue;
		}
	}
	if (14946 == 14946) {
		int xgzufk;
		for (xgzufk = 39; xgzufk > 0; xgzufk--) {
			continue;
		}
	}
	if (14946 == 14946) {
		int wzez;
		for (wzez = 82; wzez > 0; wzez--) {
			continue;
		}
	}
	if (string("fefvthqaong") == string("fefvthqaong")) {
		int jwvq;
		for (jwvq = 7; jwvq > 0; jwvq--) {
			continue;
		}
	}
	return 75874;
}

bool fawpeep::uvvpvdkbgpyyms(int xsvhqkpghyngq, string udjyaqnfqarka) {
	string lkzzh = "kzuar";
	double rbowjscfvqfb = 12396;
	bool weiibhwoid = false;
	double iqbgboqour = 38184;
	double kuesknlvmyi = 55668;
	int fkkoydsdwpbunr = 2351;
	if (55668 == 55668) {
		int lwit;
		for (lwit = 66; lwit > 0; lwit--) {
			continue;
		}
	}
	if (false != false) {
		int cdszd;
		for (cdszd = 64; cdszd > 0; cdszd--) {
			continue;
		}
	}
	return false;
}

double fawpeep::latinlbneexljtrzielqw() {
	bool pgfuczulaojxnp = true;
	string lulxtmei = "zkxatsnurwgsgegibohqiwdvtzljmkedkjhucuqwpmtqryfytotwxcyexriusqygemjjvswnyklbfilqvszap";
	return 67663;
}

string fawpeep::okufcwehcrjhxjpvyhnz(bool pnxjogxulozhs, double oassfr, string dxsptndpnzyfe) {
	string xwfeosdj = "ijzxylqsdklcmu";
	if (string("ijzxylqsdklcmu") == string("ijzxylqsdklcmu")) {
		int va;
		for (va = 63; va > 0; va--) {
			continue;
		}
	}
	if (string("ijzxylqsdklcmu") != string("ijzxylqsdklcmu")) {
		int xld;
		for (xld = 20; xld > 0; xld--) {
			continue;
		}
	}
	if (string("ijzxylqsdklcmu") != string("ijzxylqsdklcmu")) {
		int sqbcm;
		for (sqbcm = 88; sqbcm > 0; sqbcm--) {
			continue;
		}
	}
	if (string("ijzxylqsdklcmu") == string("ijzxylqsdklcmu")) {
		int lows;
		for (lows = 29; lows > 0; lows--) {
			continue;
		}
	}
	return string("qscsopm");
}

string fawpeep::edzziczwyivtkhwsimesw(string cogqlkjsdfgzg) {
	double xagcoh = 16823;
	bool skmkulowd = false;
	string ecktdua = "dhmdnrmcs";
	int fdchsszt = 2896;
	bool yxmtnvqp = true;
	if (16823 == 16823) {
		int szpkdheaj;
		for (szpkdheaj = 38; szpkdheaj > 0; szpkdheaj--) {
			continue;
		}
	}
	if (2896 != 2896) {
		int dqhlgn;
		for (dqhlgn = 3; dqhlgn > 0; dqhlgn--) {
			continue;
		}
	}
	if (16823 == 16823) {
		int iw;
		for (iw = 48; iw > 0; iw--) {
			continue;
		}
	}
	return string("jxc");
}

double fawpeep::sryhpjbieyxqdysmv(double lqpqexdjywiax, bool javjcztm, int hceijoqz, string yodpxxetrxihdg, double ykkerprhfthehbr, bool hakfjvyu, int ujthrv, string galagrhwaqcucl, int hquohmqjspfdgkz) {
	bool koexlbxldsad = true;
	double npfivm = 3851;
	string bnkss = "mnlawssfvpuaxhwwidxpduscgtncswfwyiaiodmegbimlyjyazjtyfcqjmwaglugnjghbpvqpxeqxqpicggnzqsjvio";
	bool miipqmvfzsuaew = false;
	double lsfaft = 2914;
	double jfrmajmlyjh = 2966;
	double stazofqwxlxl = 58100;
	string aeouf = "zukccxa";
	double osiyvjkyyyyrbqz = 3786;
	if (2914 == 2914) {
		int vaim;
		for (vaim = 40; vaim > 0; vaim--) {
			continue;
		}
	}
	return 55603;
}

int fawpeep::hhgagqrnkbxpgzezzkhodxt(bool rzbssg, double sjtjwirjsmsscpv, double obocoiixcium, double pkuochyb, bool qhecz, bool aykteqmxijfyy, int thhfudvcbjkp, double mftdblhou, int ivyiby) {
	bool mxkzwlhvin = true;
	string bbtdp = "sfnyfmeoyfevwkobxnnzsyyojltbljqtooswewkipyfgutadflzbexhtrshmpqd";
	string ydneczkjnc = "pvyfrfnhrheweqesamhytsuegsdenqcelupmouxfqzvtlzaexriwyezskstdknpkfvqmvswifvbpbmridjzurdli";
	if (string("sfnyfmeoyfevwkobxnnzsyyojltbljqtooswewkipyfgutadflzbexhtrshmpqd") == string("sfnyfmeoyfevwkobxnnzsyyojltbljqtooswewkipyfgutadflzbexhtrshmpqd")) {
		int pwrvvoxs;
		for (pwrvvoxs = 31; pwrvvoxs > 0; pwrvvoxs--) {
			continue;
		}
	}
	return 84235;
}

double fawpeep::gkctmflzuwog(string wzxgcbgivuidb, string havupw, string zjpmj, double batqjlkinnsd, int rhzvkg) {
	int latobtrlbv = 2133;
	double nbbpghykfw = 27023;
	bool ewsrbwzxfq = true;
	double jtzhezvcccaqa = 28154;
	int slstddzselvb = 4648;
	double skdlcysavpe = 51608;
	if (4648 == 4648) {
		int lm;
		for (lm = 9; lm > 0; lm--) {
			continue;
		}
	}
	if (2133 != 2133) {
		int moshep;
		for (moshep = 92; moshep > 0; moshep--) {
			continue;
		}
	}
	if (4648 != 4648) {
		int kkr;
		for (kkr = 29; kkr > 0; kkr--) {
			continue;
		}
	}
	if (2133 == 2133) {
		int oagoirq;
		for (oagoirq = 40; oagoirq > 0; oagoirq--) {
			continue;
		}
	}
	return 85090;
}

string fawpeep::eqryuytkcmsonyuoxupxq(int ggmcpkd, bool ihylxypipb, string dxfebyn, int mgdvqyybtqpp, int hndfhujc, bool eevhzysx) {
	return string("mqnlrbrzyavxs");
}

double fawpeep::rsqfthseoxctbtqmujovr(int ohwovkuio, bool wtiaoktxdpywbcc, bool qrvgbtzuv) {
	double kicronkvmbyv = 72157;
	double zbklxmrnbeub = 2527;
	string tlwxihogdbgmq = "kubf";
	double wikqawbq = 10592;
	if (72157 != 72157) {
		int us;
		for (us = 47; us > 0; us--) {
			continue;
		}
	}
	if (10592 != 10592) {
		int ln;
		for (ln = 80; ln > 0; ln--) {
			continue;
		}
	}
	if (72157 != 72157) {
		int wanekh;
		for (wanekh = 95; wanekh > 0; wanekh--) {
			continue;
		}
	}
	return 22732;
}

int fawpeep::mxjrurehigkqvxe(int gmjpzsibspju, int ytqsdzlmuqmetjl, string cjkrwxjejmc, int siibmzxyjalspir, bool orxctnaxm) {
	return 66753;
}

bool fawpeep::jkawykpvmaktwjquxcukoa(bool oprwuanmtpwva, bool aryxroropvcqkd, int jaxxbezvkqc, bool lfwtu) {
	bool qnnsexpijqyh = false;
	if (false != false) {
		int cp;
		for (cp = 59; cp > 0; cp--) {
			continue;
		}
	}
	if (false == false) {
		int ogpx;
		for (ogpx = 83; ogpx > 0; ogpx--) {
			continue;
		}
	}
	if (false == false) {
		int gk;
		for (gk = 96; gk > 0; gk--) {
			continue;
		}
	}
	if (false == false) {
		int kfftfy;
		for (kfftfy = 5; kfftfy > 0; kfftfy--) {
			continue;
		}
	}
	if (false != false) {
		int ar;
		for (ar = 95; ar > 0; ar--) {
			continue;
		}
	}
	return false;
}

double fawpeep::tuiwykvjxziowvcqifxsne(bool cgfqkuufbx, string fnexekycm, int eplznnovjczxwb, double kvqymquz) {
	int fmhuzh = 1151;
	int waifiq = 352;
	int zcrkrbsqbdun = 7299;
	double jvyunrg = 49133;
	int jrjfbvofetrzhuu = 4270;
	int kwzpcbddtbsoab = 2440;
	double vufgkqr = 19479;
	int kbfjsqsqohoatmb = 8112;
	if (19479 != 19479) {
		int tohsd;
		for (tohsd = 82; tohsd > 0; tohsd--) {
			continue;
		}
	}
	if (8112 == 8112) {
		int cmupc;
		for (cmupc = 26; cmupc > 0; cmupc--) {
			continue;
		}
	}
	return 78558;
}

bool fawpeep::yjzemwhugz(bool zcgjyxbmou, int dpkqa, string abgzcsisf, double mblpkus, int nyuofboqpufnor, double acwuqbgrmjhqgnn, double ysgtqvact, string qiwxf, int xjllwl, double ljehjz) {
	bool tuvtwfckyhopcbg = true;
	int wnjrdmewtf = 2747;
	int sqdkz = 3186;
	return false;
}

double fawpeep::bglctyyemizfue(int rmosvi, bool oualpxv, double tdyysvryomyfm, string onxmsvqbqn) {
	int nmijz = 48;
	int tntqqeyttbg = 1789;
	string tsflqdznxlp = "cnvhupbzwneueyfuglcjjuynvebfebfuscflkjmcvzwoxdimbdikrfverlnlzuddtgpycku";
	bool iajntlwhsgqf = false;
	double rmoxilpiinqkujh = 10518;
	int kfsftcwhryxkqh = 3947;
	string rcgvnfddheqlp = "bxaasbefpublifembqvjeojsxqyradcckkmqywejtcbebvltln";
	double iwyynra = 76563;
	int dvrtgjsg = 744;
	string nakazpxnta = "dlijismcxsjutpcorxmaumycwmxtmlmcmvkzhrkceczyxfbmsiimxxuldkichrfxheydmyunod";
	return 34652;
}

string fawpeep::hbveassvfz(bool udldkqqvsg, string mnxngsegfyzbdht, int fmgthloro, int gskxolfdznot, bool quvbdnxf, int rpkqoryl, string klfmfxo) {
	int mfwwusywmn = 5754;
	double yzzhmuhfshjeqv = 58819;
	string gboefoktxtlj = "uccdgnuolxdezfobnymfucbstpemcuitizvefsgcbbzcvjmbudpkdkjyslcrktcojsghitwtkfjxcitfcikgfwyxtpcemxihoj";
	bool vminatsxnhsrw = true;
	bool fknuknvfzf = true;
	int dikypuz = 4302;
	if (string("uccdgnuolxdezfobnymfucbstpemcuitizvefsgcbbzcvjmbudpkdkjyslcrktcojsghitwtkfjxcitfcikgfwyxtpcemxihoj") != string("uccdgnuolxdezfobnymfucbstpemcuitizvefsgcbbzcvjmbudpkdkjyslcrktcojsghitwtkfjxcitfcikgfwyxtpcemxihoj")) {
		int mxkyvrx;
		for (mxkyvrx = 7; mxkyvrx > 0; mxkyvrx--) {
			continue;
		}
	}
	if (true == true) {
		int ay;
		for (ay = 81; ay > 0; ay--) {
			continue;
		}
	}
	if (4302 != 4302) {
		int zkwrauuqws;
		for (zkwrauuqws = 80; zkwrauuqws > 0; zkwrauuqws--) {
			continue;
		}
	}
	if (58819 == 58819) {
		int ughrmckp;
		for (ughrmckp = 22; ughrmckp > 0; ughrmckp--) {
			continue;
		}
	}
	return string("y");
}

int fawpeep::tkcixlqgaaerusdyhwyqhae(int awncaivfdzf, bool acidlwunik, int oaqlqnolioisrei, int gchlzhcvywv, string xrxinfqlkvqwq, string eclilyhrwclzwye) {
	string yxxfksetmumtf = "zjsupjngopxrazvxlpethcqonoocsujoyejmty";
	int ciolfs = 420;
	int bmopuyypzspt = 454;
	int ahrtuxluolnpelj = 7032;
	double lelcxwtehltffti = 37287;
	double ggbycwrojlbj = 11433;
	int opxngebv = 1811;
	return 22299;
}

bool fawpeep::jceqhimuhwrljzk(bool bypzmgxegm, int gylwtcrv, int apseqfpfiqvsb, bool dtbxjwcnh, bool dwsusbmyhaljp) {
	int gwvvcoq = 1012;
	bool khykyxmafx = true;
	string qqeqdi = "pqpnfufkskkuprlszzesjongauskiauegktbcxyxhxmoflognwbm";
	if (1012 == 1012) {
		int sgfwjsj;
		for (sgfwjsj = 36; sgfwjsj > 0; sgfwjsj--) {
			continue;
		}
	}
	if (1012 != 1012) {
		int cais;
		for (cais = 46; cais > 0; cais--) {
			continue;
		}
	}
	if (string("pqpnfufkskkuprlszzesjongauskiauegktbcxyxhxmoflognwbm") != string("pqpnfufkskkuprlszzesjongauskiauegktbcxyxhxmoflognwbm")) {
		int tbymsexb;
		for (tbymsexb = 69; tbymsexb > 0; tbymsexb--) {
			continue;
		}
	}
	if (1012 == 1012) {
		int ojlintdqu;
		for (ojlintdqu = 2; ojlintdqu > 0; ojlintdqu--) {
			continue;
		}
	}
	return false;
}

void fawpeep::lppogawjhtnohstlcc(bool zmguhbtht, string yvwcvpvlzons, string cwagzpwdmlepjy, double symnijvy, string fytfcmkiqaqtsy, int lzumsdrwzsrqads) {
	double rehenw = 34881;

}

fawpeep::fawpeep() {
	this->jkawykpvmaktwjquxcukoa(true, false, 2819, false);
	this->tuiwykvjxziowvcqifxsne(false, string("cuzcekmryoqxbkvwqthgqydygysopwxttnrkodcuaxaahfqfdzjlwozdfpcgmdwnosxoyqzib"), 848, 4189);
	this->yjzemwhugz(false, 2191, string("zwlvlvlhoskepzcrvbxvdxtmnvencvptmwsqgrbptfezfiduewurthzyhuflhlsxqimtmjvcwlxiemqyqfdardinpk"), 21150, 355, 31337, 4811, string("sppqagxxrhypjjriwmlbligicxljlrqgppwcmsabhiwfvsrvntmdczutuuctbqrdwvf"), 526, 3122);
	this->bglctyyemizfue(1717, true, 9940, string("ervqdsnprjueiwfkiygcufmkpwsiwedwiaauwyrnkdgpmxcrnwcfcnjzidnygpxuzhctkgivtbywotjd"));
	this->hbveassvfz(false, string("wrdmd"), 2716, 4211, false, 1343, string("rfvdnbgdbhwxwrczldpepimihwsirrpbhatdyqqblkoehxsrgxtyfhfuzsiqmqbwvloupfslqspkokatdpyw"));
	this->tkcixlqgaaerusdyhwyqhae(4715, true, 1587, 5726, string("ziqhdyjvzmy"), string("aztprrznshjyrftfyungtealuoheslndtybwnpgdubpjumblrncpzdsultuaprytdumpyetgmjobmffagiwdxhfaqlifcqpci"));
	this->jceqhimuhwrljzk(true, 1284, 1626, false, false);
	this->lppogawjhtnohstlcc(true, string("oriwzqgjlssdnzuhclnhljzwdzagiordixjrtristgpmmaewhvqvptclkhzbpaugnzpnpgssostkiatqreyvlvgsjehjkxsvxvri"), string("ktctoqyjgfaepfjxncvpwtpzqzechtlyryzkaxlhazvcwtxoomehgauklqxtvrwdayfwljkbqhknn"), 26028, string("sxgjaulwjsllhusedxjvremvqucfmqfigxhsozaxuwkeyqwjg"), 5656);
	this->uvvpvdkbgpyyms(5731, string("tfrowrhjoyjyb"));
	this->latinlbneexljtrzielqw();
	this->okufcwehcrjhxjpvyhnz(false, 10024, string("fgwfzczghrooiyebxuzftpwhxzodirolhmcslgtydkebv"));
	this->edzziczwyivtkhwsimesw(string("ybyfioyjjdlxuohpngyalumfjsvoujmiqfeijsqrsjzayfxidjyaggfsiqacmqjsfaeekrfhigfzyomfwsozai"));
	this->sryhpjbieyxqdysmv(12578, false, 363, string("dhlwkfudwcjneyyxinqch"), 40658, true, 1392, string("skevunnahqgatqfuytckbwfjaqovvhojvxvuthkispeomxchczfjpgfduftsoifcvzxdrcayxyjowh"), 2289);
	this->hhgagqrnkbxpgzezzkhodxt(false, 15480, 38274, 37546, false, false, 1006, 21418, 6772);
	this->gkctmflzuwog(string("ncxvgochggjignjrpaebkvbfhsshssdfjtgofzafqdldcltjpzgzipcenokgrprsgloaikoltvaejbzuzmxlhjg"), string("sdxpllzuigzzgayekcjfwpjxbpkpcvnixkzjzqkuaorjwiizjcbwefahfqiv"), string("hnybmljdl"), 2512, 809);
	this->eqryuytkcmsonyuoxupxq(437, false, string("wizozbhxvmywoqisdaoyngcdxezkopoogonysdpfxmgtexbjqjqjbhldwrprkeuu"), 670, 1089, false);
	this->rsqfthseoxctbtqmujovr(3717, false, true);
	this->mxjrurehigkqvxe(2151, 1281, string("fizflrzsdmecdyirkowagqpfafhdrvqkkltrjvoectoekhfsbodjhhizvncierxsmqnqtxdywuwexwggjrum"), 2879, true);
	this->fyqmypgvogqteodazmf(string("rlisjjcddyeodamfveurbmgroszabgfeittkzggrfusomysaudegelkbknfgvukqyr"), 4184, 4096);
	this->nbqhcmblhjkkvjmhyduyioyu(string("hilhjuknyillegenxohtffpkkegizsibyrgwxttxdawcpacoxnzabrvhhbygxcliypfrrkbkchmt"), 43532, 7969, 15466, 48839, 3375, string("jrlfnahntaucsjvrgddtlmx"), 1929, 5841, 6880);
	this->xtegdsalfvf(40222, 21220, 8542, false, 1242);
	this->mtvmnykceopmt(28624, 42142, 68727, 54370, string("ocfbbfyryhofpsdzypojevhhlcxhssgtaszdfkabz"), 17765, string("owloanipdmpgbpuhycpcak"), string("osuhcjzenmspwkgk"));
	this->foptdpqgdpj(string("acsttdihuvmny"), 1718);
	this->sbnsnhdzowispfjcigxudcbum(string("wtuplyopdkpfbeivtsezscayamlvqmeoclfqruunsholmz"), 9189, true, 3295, 5871);
	this->kcwgnrqpyfd();
	this->atcnnqwhmalfszgjlfpxabfm();
	this->ifmhlriwajtf();
}
