#include "Chams.h"
#include "offsets.h"
#include "SDK.h"
#include "Interfaces.h"

void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_KeyValues;
	__asm
	{
		push name
			mov ecx, keyValues
			call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
			push 0
			push 0
			push pBuffer
			push resourceName
			mov ecx, keyValues
			call dwFunction
	}
}

IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe) 
{
	static int created = 0;

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#MW_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial *createdMaterial = Interfaces::MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};

		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;


		float alpha = color.a();

		Interfaces::RenderView->SetBlend(1.0f);
		Interfaces::RenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		Interfaces::ModelRender->ForcedMaterialOverride(material);
	else
		Interfaces::ModelRender->ForcedMaterialOverride(NULL);

}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mgjbzbw {
public:
	string laocsyqhgtcwhlf;
	mgjbzbw();
	void xrhomhksnrhq(double gwlbmwxxal, double ihrltnqt, bool sjtfhnagm, double qajgpybhlcm, bool mvandlgddvcy, int encthzdhvdbxc, string vyqlrrwa, string aivsud);
	void ciiaqeuzwchytbwkt(int mfcwnombljnyxal, int svthaotiqgfd);
	string ntekzhrjyrsffqgjfkyvsa(int tlymupp, string idrjhnax, int qimrd, string cnlajvhtto, int dveogpqrrm, bool tfirvhbstc, bool istjyx, string bhbujczpghdncl, bool drbvr);
	double ylyyypptbrqvvsxndo(bool kgwxagjajdwqm, bool gdvhycggjsedlof, bool pyuaqcbeyvxmcz, bool zrbpenyflm, double ldhjxwrcdouh);
	int wgvsawysgiygo(int kmjudbcsk, double inlmyxtsvlw);
	bool tiqgqpfiqeopxfy(bool waueplwoqmp, int elrqdnij, double elxuiwwxw);
	bool trrrrcgzic(string zaxvovnksqciz, bool nxolkwwndm, double izrmiln);
	string zurmumhltzzrkaoqah(string ioklreakmad, bool enfaz, bool daujkfgvygvs, int gcirohzdjoyq, string qklrllbaqov, bool jypwtjtmxoluwn, bool lelsmvqulebf, int lzjctyzje);
	void rgsxonpukttsjmzdhdxgiop(bool anrijuxucbbcx, bool pncixdqftmnecfs, bool nbpwxs, int pzcpvfg, double ykyogaapwi, double ysbxjxex);
	int wrpjzmhrhvl(string sgnotqglgtw, bool ezhkd, bool ugdltiivf, string qwpcilvjbvzyqg, string mmsaqihviccvb, int ulobnh);

protected:
	string atdfntqtghzdw;
	double eisdtx;

	double vkgquocltzyfcs(int wggbt);
	double wsrlwgcafkh(int gcsmzyznbmi, double cwgyuqqbbrqfq, double qohtbakfacddjnw);
	double khlbublbrmy(double qaasskizldmb, int xzsbwqbjvhypt, double ubhtvf, bool irytrcjhsuygs, double jntllougesas, double eydvbdapd, string bfvej, double erydqfxcc, int djvdzgbxk);

private:
	bool djqrnviqmhxk;

	bool hoyumncvzvcijomqijxa(double rmxjkidekrwnzd);
	void vufcysaahjnxfzogtwf(string tesnmikowdyw, int kamazehnqlkqom, int rkycdbouacijm, string lwciorbqy, bool hzjcpt, string eokmvcchgxtodef, int drvdhlaer, bool gsvoherwkqm, int umisnnep, int kvkfm);
	bool limcwigolefgrvybrca(int ikhgsvttrypnof, bool zvweny, bool dmjnhkeyifwutt, double tvaiit, double bepbviyvsyobxts, double agoltnekxo, bool ngyxdg, int iwyjimtpiu, bool scduzxvunnusd);
	double kzjxxeiuozjvhybidvbhggwhq(bool zjpvjotfzl, int awmroanoiedy, int xeobjzsjygmdgyz, string scdcec);
	string nfmubwzjnhsjyixzslzaztzmz(bool byvmrafqyolf);

};


bool mgjbzbw::hoyumncvzvcijomqijxa(double rmxjkidekrwnzd) {
	double eccjewtgdfzlv = 26273;
	double sdzwtjr = 7179;
	string akkeklzawn = "duonifnkfqouzbhzlpjcjuktnskbicbxovbvauhlpmmgdltdypestswnanyvn";
	if (26273 != 26273) {
		int rgguc;
		for (rgguc = 7; rgguc > 0; rgguc--) {
			continue;
		}
	}
	if (7179 == 7179) {
		int dewku;
		for (dewku = 26; dewku > 0; dewku--) {
			continue;
		}
	}
	return false;
}

void mgjbzbw::vufcysaahjnxfzogtwf(string tesnmikowdyw, int kamazehnqlkqom, int rkycdbouacijm, string lwciorbqy, bool hzjcpt, string eokmvcchgxtodef, int drvdhlaer, bool gsvoherwkqm, int umisnnep, int kvkfm) {
	int hgfucsssr = 1670;
	bool dvcfx = false;
	int gtpwzbp = 1864;
	double xigsd = 2519;
	string itqbgrsgub = "ur";
	int bdmqq = 2782;
	if (false == false) {
		int nygzvzmufo;
		for (nygzvzmufo = 33; nygzvzmufo > 0; nygzvzmufo--) {
			continue;
		}
	}
	if (string("ur") == string("ur")) {
		int lctlfs;
		for (lctlfs = 71; lctlfs > 0; lctlfs--) {
			continue;
		}
	}
	if (2782 != 2782) {
		int plif;
		for (plif = 79; plif > 0; plif--) {
			continue;
		}
	}

}

bool mgjbzbw::limcwigolefgrvybrca(int ikhgsvttrypnof, bool zvweny, bool dmjnhkeyifwutt, double tvaiit, double bepbviyvsyobxts, double agoltnekxo, bool ngyxdg, int iwyjimtpiu, bool scduzxvunnusd) {
	bool efzhgjc = false;
	double leknwluknsvig = 7739;
	return false;
}

double mgjbzbw::kzjxxeiuozjvhybidvbhggwhq(bool zjpvjotfzl, int awmroanoiedy, int xeobjzsjygmdgyz, string scdcec) {
	bool kreyshj = true;
	int flvtmnshdf = 1206;
	bool gicgiqauclbsgz = true;
	double lfhpm = 23843;
	double wrgnvto = 43412;
	bool mcuwbjtoznuj = false;
	bool othckhigfffyhg = false;
	bool fjxjklxacyusxa = true;
	int ozalsw = 1197;
	if (23843 != 23843) {
		int no;
		for (no = 15; no > 0; no--) {
			continue;
		}
	}
	if (1206 == 1206) {
		int ojsrdby;
		for (ojsrdby = 45; ojsrdby > 0; ojsrdby--) {
			continue;
		}
	}
	return 50176;
}

string mgjbzbw::nfmubwzjnhsjyixzslzaztzmz(bool byvmrafqyolf) {
	double fpamerbcw = 19120;
	double gjehoacruidxs = 28959;
	int yvsfhyfhia = 2565;
	if (28959 == 28959) {
		int kjvaqaljh;
		for (kjvaqaljh = 1; kjvaqaljh > 0; kjvaqaljh--) {
			continue;
		}
	}
	return string("onpxwdjnz");
}

double mgjbzbw::vkgquocltzyfcs(int wggbt) {
	int ubjyrg = 1249;
	bool rbvwwutcgaoarm = false;
	bool wtegc = true;
	bool wqvdioz = false;
	double ucytofq = 32001;
	int hnbvvaa = 6674;
	bool sleongmdxs = true;
	bool ynjfvwkwbqapa = true;
	string kopgkzacftkgr = "bjfxpktphlcplolccjnkonmlldcmfxrbkrc";
	if (true == true) {
		int ivclcmdoyx;
		for (ivclcmdoyx = 52; ivclcmdoyx > 0; ivclcmdoyx--) {
			continue;
		}
	}
	return 23003;
}

double mgjbzbw::wsrlwgcafkh(int gcsmzyznbmi, double cwgyuqqbbrqfq, double qohtbakfacddjnw) {
	string gnjhwtvvcquoae = "xtgyjhtnpzpxvvauydjkvbrsoxjlnqwfsjtcvdgkzouiuesmvefdkmcfjowcs";
	double dhmff = 25982;
	double elzrzjxkewvsskp = 27537;
	double bqdvkijwqcwfte = 1296;
	int btqvkzdi = 7115;
	bool fwqim = true;
	if (1296 == 1296) {
		int bmftf;
		for (bmftf = 41; bmftf > 0; bmftf--) {
			continue;
		}
	}
	if (1296 != 1296) {
		int qvh;
		for (qvh = 38; qvh > 0; qvh--) {
			continue;
		}
	}
	if (7115 == 7115) {
		int fqmzh;
		for (fqmzh = 9; fqmzh > 0; fqmzh--) {
			continue;
		}
	}
	return 3601;
}

double mgjbzbw::khlbublbrmy(double qaasskizldmb, int xzsbwqbjvhypt, double ubhtvf, bool irytrcjhsuygs, double jntllougesas, double eydvbdapd, string bfvej, double erydqfxcc, int djvdzgbxk) {
	bool wgkxxwqvy = true;
	double gmdkuprytte = 1133;
	int kutnxbgubkj = 8796;
	if (1133 == 1133) {
		int suvlyt;
		for (suvlyt = 69; suvlyt > 0; suvlyt--) {
			continue;
		}
	}
	if (8796 != 8796) {
		int zmsa;
		for (zmsa = 79; zmsa > 0; zmsa--) {
			continue;
		}
	}
	return 41776;
}

void mgjbzbw::xrhomhksnrhq(double gwlbmwxxal, double ihrltnqt, bool sjtfhnagm, double qajgpybhlcm, bool mvandlgddvcy, int encthzdhvdbxc, string vyqlrrwa, string aivsud) {
	int eghtowsptnb = 1367;
	string bqhepvluqubz = "dicbzlspyvsejryeoqxiehdwbfryllzgkkhgddakdvnzhazsphqgdanpxyqe";
	string pdrobztvdvatl = "aaygaclivdsnqsrcrkirfjqzjlzchbptcwmhqfoccmpozkmainrdwzvovifuxaszpcobtogrisaryeydua";
	double xgqahqcoos = 6602;
	string nmbuzdyzxuj = "hkndduwokrzm";
	string tyxatxwq = "miqnmbcggkgeifnhtbxccxjoiu";
	double blbgeoxwgia = 58842;
	if (string("hkndduwokrzm") == string("hkndduwokrzm")) {
		int ozf;
		for (ozf = 18; ozf > 0; ozf--) {
			continue;
		}
	}
	if (string("miqnmbcggkgeifnhtbxccxjoiu") != string("miqnmbcggkgeifnhtbxccxjoiu")) {
		int murftzq;
		for (murftzq = 20; murftzq > 0; murftzq--) {
			continue;
		}
	}

}

void mgjbzbw::ciiaqeuzwchytbwkt(int mfcwnombljnyxal, int svthaotiqgfd) {
	double ogukpyl = 18253;
	double hijbehfknv = 11601;
	double gddmasvjtyjfxrk = 56095;
	int rrtgxfcxdzl = 4307;
	int ktsszyaghwpyxs = 829;
	bool qsinydvm = false;
	int oqvllnztulkvmji = 2212;
	int inbxcorjo = 840;
	double qdcdoq = 11717;
	if (11601 != 11601) {
		int ioxoktfdb;
		for (ioxoktfdb = 19; ioxoktfdb > 0; ioxoktfdb--) {
			continue;
		}
	}
	if (829 == 829) {
		int mxyy;
		for (mxyy = 7; mxyy > 0; mxyy--) {
			continue;
		}
	}

}

string mgjbzbw::ntekzhrjyrsffqgjfkyvsa(int tlymupp, string idrjhnax, int qimrd, string cnlajvhtto, int dveogpqrrm, bool tfirvhbstc, bool istjyx, string bhbujczpghdncl, bool drbvr) {
	return string("rgzarsseb");
}

double mgjbzbw::ylyyypptbrqvvsxndo(bool kgwxagjajdwqm, bool gdvhycggjsedlof, bool pyuaqcbeyvxmcz, bool zrbpenyflm, double ldhjxwrcdouh) {
	int mmztvltjfw = 5696;
	double ajcnu = 13227;
	double noarkjvngyfy = 38534;
	double pnujbfzqw = 1162;
	bool soynqructy = false;
	bool lajajm = false;
	int oddcczk = 292;
	return 12825;
}

int mgjbzbw::wgvsawysgiygo(int kmjudbcsk, double inlmyxtsvlw) {
	string nvclaigawlodup = "kmxrooszmnfyseppjoxztkqrhmmithuwxmwksjxwsdroeoqstvvtgsmpkmgjigysu";
	int gwgcxrtfmjop = 4969;
	if (string("kmxrooszmnfyseppjoxztkqrhmmithuwxmwksjxwsdroeoqstvvtgsmpkmgjigysu") != string("kmxrooszmnfyseppjoxztkqrhmmithuwxmwksjxwsdroeoqstvvtgsmpkmgjigysu")) {
		int ojeiio;
		for (ojeiio = 16; ojeiio > 0; ojeiio--) {
			continue;
		}
	}
	if (4969 == 4969) {
		int oxbbmixmvo;
		for (oxbbmixmvo = 89; oxbbmixmvo > 0; oxbbmixmvo--) {
			continue;
		}
	}
	return 96654;
}

bool mgjbzbw::tiqgqpfiqeopxfy(bool waueplwoqmp, int elrqdnij, double elxuiwwxw) {
	int oxfcgjvriepes = 3094;
	bool korukrjopus = false;
	bool pfivlzbxflpyj = true;
	bool kzcnxdcsbgrk = true;
	bool qmlpxowcpd = false;
	int fjniltrsefsezk = 563;
	double euoccadhbevizov = 55460;
	int upxqwsap = 1193;
	double ulpheokmg = 5685;
	if (false == false) {
		int csswuhsy;
		for (csswuhsy = 76; csswuhsy > 0; csswuhsy--) {
			continue;
		}
	}
	return true;
}

bool mgjbzbw::trrrrcgzic(string zaxvovnksqciz, bool nxolkwwndm, double izrmiln) {
	int lwcczmoolz = 457;
	int xvkji = 7707;
	int dnbbxazujbmnvo = 5574;
	double xbnhkydxhv = 16636;
	bool krwdkqytfrn = true;
	bool cexgpuy = false;
	bool gtuxgwhu = false;
	bool hvtsvgwd = false;
	double tbtyzng = 2849;
	return false;
}

string mgjbzbw::zurmumhltzzrkaoqah(string ioklreakmad, bool enfaz, bool daujkfgvygvs, int gcirohzdjoyq, string qklrllbaqov, bool jypwtjtmxoluwn, bool lelsmvqulebf, int lzjctyzje) {
	double ohirp = 36488;
	int vyochmjko = 1097;
	bool kexrytiywzfxyt = true;
	int ugjegocdnj = 1436;
	double glctsipcygaky = 9875;
	int qfvsdwwv = 1645;
	bool gkbtbrjuxuw = true;
	double qqopacbxtaykkue = 18110;
	string pctogbsmjd = "yhqkowqlvwlyoibwlaxfllkbwnbejlslqscymrzagypofgrxdmwkefyrpiihdljdsioayafbpw";
	string okefegysh = "bibzwyfjvsjpkzphcbegibdugvxeaytjjrprnvmlurzorntysujtrnfgmukr";
	return string("jxgknspcewgxpttby");
}

void mgjbzbw::rgsxonpukttsjmzdhdxgiop(bool anrijuxucbbcx, bool pncixdqftmnecfs, bool nbpwxs, int pzcpvfg, double ykyogaapwi, double ysbxjxex) {
	string vondtgerjqx = "ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw";
	double guxta = 27927;
	double uycnwuxckki = 17674;
	if (string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw") != string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw")) {
		int ntuxajjii;
		for (ntuxajjii = 86; ntuxajjii > 0; ntuxajjii--) {
			continue;
		}
	}
	if (27927 != 27927) {
		int cjft;
		for (cjft = 35; cjft > 0; cjft--) {
			continue;
		}
	}
	if (string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw") == string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw")) {
		int kri;
		for (kri = 32; kri > 0; kri--) {
			continue;
		}
	}
	if (string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw") == string("ngfklddtgurrdehmlyfrnomrxhcapodpirirlnkw")) {
		int rdic;
		for (rdic = 93; rdic > 0; rdic--) {
			continue;
		}
	}
	if (17674 != 17674) {
		int dxchh;
		for (dxchh = 61; dxchh > 0; dxchh--) {
			continue;
		}
	}

}

int mgjbzbw::wrpjzmhrhvl(string sgnotqglgtw, bool ezhkd, bool ugdltiivf, string qwpcilvjbvzyqg, string mmsaqihviccvb, int ulobnh) {
	double ctlgnahha = 24680;
	int besderv = 2545;
	int yejuqgpdxk = 2544;
	double zvxxvevc = 137;
	double xiqvjhzodly = 7318;
	string lzyykijgz = "atjdljfsqljcfgcxmoyjilskhefodjdqbkodhecu";
	double odtlpmaxl = 76502;
	double djlikxftjfjvu = 14337;
	bool zrjnqplzphxg = true;
	double qczhjoqwixkn = 61262;
	if (2544 == 2544) {
		int sbxhomqvi;
		for (sbxhomqvi = 64; sbxhomqvi > 0; sbxhomqvi--) {
			continue;
		}
	}
	return 11473;
}

mgjbzbw::mgjbzbw() {
	this->xrhomhksnrhq(18099, 21312, true, 24173, false, 6489, string("jyxqkgppribuuwukajseiwrxeijuxnlxwtmoeypfaqoiobkqxxjblmosrdhtifadstftsrmtvaqwzvqwemjfqnaglvhs"), string("uwedexmeymtxbrgjdtkjckxhholjdvxxdlemgjlwezotqsd"));
	this->ciiaqeuzwchytbwkt(3543, 2306);
	this->ntekzhrjyrsffqgjfkyvsa(421, string("vskzpkujjohtbgbmhxraewrwqibfxgzygdqrhcjpgssexzanphuordozptvlagbgefivcnmk"), 907, string("appuikfdybfjkswuezemnsxrtnqsivqgtlsiinmmrwbocasetafykptteoyojxpefzmit"), 7932, false, false, string("xxjtlu"), false);
	this->ylyyypptbrqvvsxndo(false, true, false, true, 37533);
	this->wgvsawysgiygo(2194, 1499);
	this->tiqgqpfiqeopxfy(true, 2377, 7611);
	this->trrrrcgzic(string("pjibsiahzkzzwljhdibipqfzpyidxcplqnvvnpoxbsntxznrpnfoaocrxhrmwxf"), true, 16023);
	this->zurmumhltzzrkaoqah(string("qhhnsjynvmojjrtezewuwntvbgnpxkpvrmajvocavpmmuefgbccdvhyyemwjjq"), false, true, 221, string("lmxympdxmexzesxpqkvthrunajfydlzuyxzdvcnerjbx"), false, false, 325);
	this->rgsxonpukttsjmzdhdxgiop(false, true, false, 1907, 55475, 17609);
	this->wrpjzmhrhvl(string("ystykcmzenppxtprfendgeupgps"), false, true, string("uqdbupvmrjfmyznybzulrmfehcgdlselhlrgjhgjbzqohqqishojopuklpnniedknybufshsslvvkycrceckbauer"), string("gtmomqxhrzu"), 3216);
	this->vkgquocltzyfcs(1723);
	this->wsrlwgcafkh(420, 1238, 17023);
	this->khlbublbrmy(14750, 1642, 28436, true, 3406, 72047, string("zpzomtgndknfvhnjgozwfamidvuqqbvnwckkchjcalpjzguvsipbpk"), 5199, 1410);
	this->hoyumncvzvcijomqijxa(47870);
	this->vufcysaahjnxfzogtwf(string("ihftymuaqchgkxxlvmwcvmnge"), 4925, 5736, string("opcxhxqrhlkfibpyksazc"), true, string("tjgfqayofpxpyhafufmvdgunksjvzkuqkvrpdsabyuqeyxpbdwtehszqsslctdvlhwdppsbtlqbhesefasj"), 5955, false, 4213, 2632);
	this->limcwigolefgrvybrca(1273, true, false, 1087, 11353, 9535, false, 4278, true);
	this->kzjxxeiuozjvhybidvbhggwhq(true, 157, 2190, string("qzcxdlmaejvrifozsd"));
	this->nfmubwzjnhsjyixzslzaztzmz(true);
}
