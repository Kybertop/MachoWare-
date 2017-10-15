#pragma once

#include "RenderManager.h"

#define _CRT_SECURE_NO_WARNINGS
#define IDR_FONT1                       101

#ifdef STRICT
typedef void *HANDLE;
#if 0 && (_MSC_VER > 1000)
#define DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name
#else
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#endif
#else
typedef PVOID HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;

namespace Render
{
	namespace Fonts
	{
		DWORD Default;
		DWORD Menu;
		DWORD MenuBold;
		DWORD ESP;
		DWORD MenuText;
		DWORD Icon;
		DWORD MenuTabs;
		DWORD Text;
		DWORD Slider;
		DWORD Clock;
		DWORD LBY;
		DWORD Tabs;
	};
};


enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

void Render::Initialise()
{
	Fonts::Default  = 0x1D; 
	Fonts::Menu     = Interfaces::Surface->FontCreate();
	Fonts::MenuBold = Interfaces::Surface->FontCreate();
	Fonts::ESP = Interfaces::Surface->FontCreate();
	Fonts::MenuText = Interfaces::Surface->FontCreate();
	Fonts::Icon = Interfaces::Surface->FontCreate();
	Fonts::MenuTabs = Interfaces::Surface->FontCreate();
	Fonts::MenuText = Interfaces::Surface->FontCreate();
	Fonts::Slider = Interfaces::Surface->FontCreate();
	Fonts::Clock = Interfaces::Surface->FontCreate();
	Fonts::LBY = Interfaces::Surface->FontCreate();
	Fonts::Tabs = Interfaces::Surface->FontCreate();

	Interfaces::Surface->SetFontGlyphSet(Fonts::Menu, "Unispace", 13, 480, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::Text, "Unispace", 50, 500, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::MenuBold, "Big Pixel demo", 14, 900, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::ESP, "Big Pixel demo", 14, 500, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::MenuText, "Big Pixel demo", 16, 500, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::MenuTabs, "MyScriptFont", 18, 600, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::Slider, "Big Pixel demo", 17, 600, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	Interfaces::Surface->SetFontGlyphSet(Fonts::Clock, "Prototype", 22, 600, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	Interfaces::Surface->SetFontGlyphSet(Fonts::Tabs, "MyScriptFont", 50, 600, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_DROPSHADOW);
	Interfaces::Surface->SetFontGlyphSet(Fonts::LBY, "Prototype", 20, 800, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);

	HRSRC hResInst = FindResource(nullptr, MAKEINTRESOURCE(IDR_FONT1), RT_FONT);
	HGLOBAL hGlobal = LoadResource(nullptr, hResInst);
	size_t __LEN = SizeofResource(nullptr, hResInst);
	DWORD dwFonts;
	Interfaces::__FNTHANDLE = AddFontMemResourceEx(nullptr, __LEN, nullptr, &dwFonts);
	auto __FONT = CreateFont(16, 0, 0, 0, 500, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "undefeated");

	Interfaces::Surface->SetFontGlyphSet(Fonts::Icon, "undefeated", 10, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);

	Utilities::Log("Render System Ready");
}

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	Interfaces::Engine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	Interfaces::Surface->DrawSetColor(color);
	Interfaces::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::Outline(int x, int y, int w, int h, Color color)
{
	Interfaces::Surface->DrawSetColor(color);
	Interfaces::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::Line(int x, int y, int x2, int y2, Color color)
{
	Interfaces::Surface->DrawSetColor(color);
	Interfaces::Surface->DrawLine(x, y, x2, y2);
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	Interfaces::Surface->DrawSetColor(color);
	Interfaces::Surface->DrawPolyLine(x, y, count);
}

void Render::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	Interfaces::Surface->DrawSetColor(col);
	Interfaces::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::DrawLine(int x0, int y0, int x1, int y1, Color col)
{
	Interfaces::Surface->DrawSetColor(col);
	Interfaces::Surface->DrawLine(x0, y0, x1, y1);
}

bool Render::WorldToScreen(Vector &in, Vector &out)
{
	const matrix3x4& worldToScreen = Interfaces::Engine->WorldToScreenMatrix();

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0; 

	if (w > 0.001) 
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; 
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void Render::Text(int x, int y, Color color, DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	Interfaces::Surface->DrawSetTextFont(font);

	Interfaces::Surface->DrawSetTextColor(color);
	Interfaces::Surface->DrawSetTextPos(x, y);
	Interfaces::Surface->DrawPrintText(wcstring, wcslen(wcstring));
	return;
}

void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	Interfaces::Surface->DrawSetTextFont(font);
	Interfaces::Surface->DrawSetTextColor(color);
	Interfaces::Surface->DrawSetTextPos(x, y);
	Interfaces::Surface->DrawPrintText(text, wcslen(text));
}

void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Render::GetTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	Interfaces::Surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = i, fh = h;
		float a = fi / fh;
		DWORD ia = a * 255;
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::DrawCircle(float x, float y, float r, float segments, Color color)
{
	Interfaces::Surface->DrawSetColor(color);
	Interfaces::Surface->DrawOutlinedCircle(x, y, r, segments);
}

void Render::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = Interfaces::Surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	Interfaces::Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	Interfaces::Surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	Interfaces::Surface->DrawSetTexture(Texture); // bind texture

	Interfaces::Surface->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Render::Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = Vertexs[i].m_Position.x;
		y[i] = Vertexs[i].m_Position.y;
	}

	Render::PolyLine(x, y, count, colorLine);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class wmgzbqs {
public:
	string pehavqkzmyni;
	int hgrfxmmyey;
	bool gxhydxkjvizy;
	bool rasjfyfhqb;
	double xmvefuyzavtshv;
	wmgzbqs();
	string ebgiigqfolagblhxurtp(int nyvxwn, int povokmqwo, int jdurx, double mtbuwkbkonfnm, int uepaisv);
	bool ocokzowblaa(int sqxrjdwhrxe, int dvgjjpoqambze, double hvzmojob, bool ecdclr, int pgjqwmxnsrvh, string ihvsvpjwnek);
	void pbcurejadgch(string hmnatxizhkutrp, double mpvikbjqmadms, double inupmh, double tnbchzsgc, string bnxwmk, int nkgnuulocbcvdq, bool jkzun, double bwbksnqyuwst);
	double iabhzfjxeiuhowoqpfvnljv(double fxqfc, string eiofmafhwalppjz, bool actbjcuqwuzym, string nowrnyvbjo, double lbbefumi, string zsvynhenibws, string eqcbnve, int abdqgun, string utlbddyvlzp, int bltidfcstri);
	bool wcdtnndofacyeggoibnlkzmb(string evzdshujrkcjdbz, bool kaztzfvrqhjojgy, int gwvvdhkxpvgjb, string jdbop, double wvjgqqpignda);
	bool oewzbkvaqfnclhztslmmxy(string kjzvgbsqf, int mxsqpczqdrg, string cshhnzalggbafps, int pmvmlxzvau, int zpkfuvrqmtkbcso, bool hrivfrdmtgtt, int ueqgkt, bool rdwotvv);
	int grqgfwrtin(bool glzupowhwffc, int brodntiubzaiqws, bool eujfj, bool zbmxbqu, double zktufzk, int wiuzotaoslhdbc, string wtzsvz, double mvaxsigohvq, string rcqlys, bool oeqvmsjs);
	int pvwewgvfxhlzynonimq(double fnvqgcbyag, bool eqgrpyqzeeopct, double nzvgbnsmwyb, string qwieaka, string mddxydxbbc, int selzdlo, bool aanae, bool aihzh);
	int tljxsjmfljcziwzvoe(bool spxryv, bool zifwvdvlzhpo, double gxchoo, bool akwio, double jzyvh, double qsdicxi, bool bmuyfvduitetwcr, int phptorgdfzhhz, double qyqxksnydtsu);
	bool kbpvqacfkgduurrdl(string itotaifl, int fklhoztkoskx, bool yxmyw, string fmofpavdke, string etecnrylns, bool wlqektaiziupyql, int dhlkncwmiwbghy, bool slvzvflllpwdpaq, int lwtao, bool bzkwvmbvw);

protected:
	string lkaybyqtvh;

	bool dcowoccdvzymexmaxdzfxlnu(int fjbxuwyicufks, string nmwhsygtkefghi, double vuijxnoypxeto, string xvbrzhxqtapaajc, double qgnmozspdne, double ipunxrvwk, string uskbkcmsikzbl, double wlmlhen, double usmvc, int sxwirdrivuvqym);
	bool qpzeqpytadtbzpraqxho(string dobushvrxheybob, bool rkzqc, string inghbdvjdwxzcb, double xghdwruagblhpje, bool trrlqiymc, string zbfnsudmxmaayn, int xkchte, bool yrftgb, string uiwwpzyehyeddo);
	int ozqedtwmeijskxffjyp(double gvvhmlbedymbqf, bool zpryxulkok, int vmnivi, double wwsbisknfalra, double ktlzzdkg);
	bool iuxhendbymbrrme(int ngevrlbtrtxvy, string txitpzn, double gvpkplqptdpokts, string sflosaxovio, string ramiqledaih);
	string esnosfawynrjffjjq(string lixta, bool bbvcivat, string ujtyvjefrgmj, string pnhrgstwnwbdbaq, double kmggtynksfkagh, double ajwmpik);
	bool kbcybdivejjruowqd(bool zzyliwbmu, int atteopwwuqobjnt, double epshmzztopunpa, double hvfcylvwzfu, int ognhsdbtzvh, string zogflr);
	string poldwspgkauu(int sgarkjfmzr, string rthpellgc, bool wvgdggqyndv, bool qszcq);

private:
	string gpvevdkvmqocrb;
	bool uzmwx;
	string oxonzs;
	bool rhtsey;

	string zhprwofqrtkouwkumalrs(double ovfejrgelgkjwu, bool znhdscatiuzcvww, int xrcrqrzict, bool zbjtwbzcigjtlqg, int mjiodaygrtyss, int akudjjrzikjmc, int mixezae, double owvumhmppmz, double jehtlcg);
	int uliqhmfjvmrbdc(string viqeyfh, double mfccsdthrlt, int gqwoewcudix, int bcthhdjqspkdbwr, double xmanvbdh, bool gtckrrkcr, int xfogo, double lpqcbu, int zuxjlcbbv, double wefpfocttsine);
	void oohbfzpryhseogvhk(int gozhvkouqhrwwgn, int jarrqgtkh, string bsxzbghwutktmrw, double cshraeyzkztqw, double copfxlndawwek, bool uhethffombi, double tbzgb, double xhccwzfnysgdfa, string osiqujynhbjuu);
	int pfrjcnirrvzckjfhfoddwlv(int rvczbtfdbq, double cpcvdwo, int xjoxaqkl, double riurbdhiod, int ywjjloyfskiqsx, string hcewmjkuqdzhcao);
	string qblynuxgphlhczrvjlvid(double zdool, double jyakbd, string rotiyr, bool gpvuofyownej, double qomdglmohurimnj, string vzfej, int mddcbzibpkcokhp);
	string wnmllczeufifdpbmuau(int caqhgggswtkbjo, double ubybnm);
	double qiawzzjjsbjqqedkdjrw(int dwxsckreblzavw, double qglvoyrggu, double hiycz, string xpunbkvkxztyv, double nkvzwurlhubnvu, double cvzwgantl, bool fqabougjwxqik, bool byiynzrmzkgksjo);
	string saksecxxokn(string gqjbauiwebyjlgh, string fjhoh);
	void ixxwcqwrqrzvdussrs(string zbstgl, string srrybrexgcnbpgp, double xtlwtwyzk);

};


string wmgzbqs::zhprwofqrtkouwkumalrs(double ovfejrgelgkjwu, bool znhdscatiuzcvww, int xrcrqrzict, bool zbjtwbzcigjtlqg, int mjiodaygrtyss, int akudjjrzikjmc, int mixezae, double owvumhmppmz, double jehtlcg) {
	double mmvhcebidgfapgj = 50329;
	bool qgbdvzrwrigkfp = false;
	string cfzqomrwbpg = "yljupmhttkusrbdmansrrukjomkaqmf";
	bool yskplqnkjwuh = false;
	if (false != false) {
		int nlsf;
		for (nlsf = 19; nlsf > 0; nlsf--) {
			continue;
		}
	}
	if (false != false) {
		int udbdg;
		for (udbdg = 90; udbdg > 0; udbdg--) {
			continue;
		}
	}
	if (false != false) {
		int pvuxpq;
		for (pvuxpq = 16; pvuxpq > 0; pvuxpq--) {
			continue;
		}
	}
	if (string("yljupmhttkusrbdmansrrukjomkaqmf") != string("yljupmhttkusrbdmansrrukjomkaqmf")) {
		int no;
		for (no = 84; no > 0; no--) {
			continue;
		}
	}
	return string("gl");
}

int wmgzbqs::uliqhmfjvmrbdc(string viqeyfh, double mfccsdthrlt, int gqwoewcudix, int bcthhdjqspkdbwr, double xmanvbdh, bool gtckrrkcr, int xfogo, double lpqcbu, int zuxjlcbbv, double wefpfocttsine) {
	return 81546;
}

void wmgzbqs::oohbfzpryhseogvhk(int gozhvkouqhrwwgn, int jarrqgtkh, string bsxzbghwutktmrw, double cshraeyzkztqw, double copfxlndawwek, bool uhethffombi, double tbzgb, double xhccwzfnysgdfa, string osiqujynhbjuu) {
	bool bizqirxxce = false;
	int vbfewqhvebth = 1886;
	double hawyqurufqagibp = 58531;
	int qwgryplu = 272;
	if (1886 == 1886) {
		int nemc;
		for (nemc = 63; nemc > 0; nemc--) {
			continue;
		}
	}
	if (58531 != 58531) {
		int oetiv;
		for (oetiv = 21; oetiv > 0; oetiv--) {
			continue;
		}
	}
	if (1886 == 1886) {
		int egtidab;
		for (egtidab = 72; egtidab > 0; egtidab--) {
			continue;
		}
	}
	if (272 == 272) {
		int gvxbyonjzq;
		for (gvxbyonjzq = 5; gvxbyonjzq > 0; gvxbyonjzq--) {
			continue;
		}
	}

}

int wmgzbqs::pfrjcnirrvzckjfhfoddwlv(int rvczbtfdbq, double cpcvdwo, int xjoxaqkl, double riurbdhiod, int ywjjloyfskiqsx, string hcewmjkuqdzhcao) {
	string pdchsqlibzyh = "hljlvykssxsnrymmpcphxsilzxeblaycsoeivvjgxsrnizao";
	string ugtzfosogpglnfw = "nviavysyxdmxlxyeepzxkyglttzcqopphwzbjygipcuuskv";
	int xddoadmdqznsaz = 2684;
	string xuwpnu = "ffmmgrksaalcpiubhnfyhtndaudfdemlkyopdjmncsxezwqmirgrfuzrkgtdcyogapivibbebjaarbcg";
	if (2684 != 2684) {
		int ataudlehe;
		for (ataudlehe = 45; ataudlehe > 0; ataudlehe--) {
			continue;
		}
	}
	return 26830;
}

string wmgzbqs::qblynuxgphlhczrvjlvid(double zdool, double jyakbd, string rotiyr, bool gpvuofyownej, double qomdglmohurimnj, string vzfej, int mddcbzibpkcokhp) {
	string fxqhwr = "mzymtpxknqrvvigztq";
	int dhvyekoga = 4404;
	double ihlegipmeeyzhtp = 62031;
	int efrvilmppdyela = 1490;
	int okeqxqots = 2341;
	string jqmgaviwzsxknab = "fzjvitecgvgwnzbkjhitd";
	string magdzkv = "zqvpdmzswdwikpungethencqqlogjygohpgcbdoubrh";
	int kmkmnzxbwz = 309;
	string belghsm = "hdkwvudtrtntuyhnsblmvfkwgriuenxjxmlbdawevhtqxizwukydhztypeasnorncuexhqhaffnrbhretmg";
	if (309 != 309) {
		int pugncvhbo;
		for (pugncvhbo = 85; pugncvhbo > 0; pugncvhbo--) {
			continue;
		}
	}
	return string("ctjoeidziscywad");
}

string wmgzbqs::wnmllczeufifdpbmuau(int caqhgggswtkbjo, double ubybnm) {
	int mrbwrlb = 3909;
	bool ebrdlevszopqdzc = false;
	double zwouiwojekyziqs = 8498;
	double wwmrvrcyswserww = 48463;
	string jtllkyuakgcp = "dqlpnbmhonsbdxhifpiybtnyaiudkxvxzwaodaitjhzcgsgnmnrxkqxparfjueek";
	bool lbnoqdqhf = false;
	if (false != false) {
		int tdzpwxs;
		for (tdzpwxs = 68; tdzpwxs > 0; tdzpwxs--) {
			continue;
		}
	}
	if (false != false) {
		int rul;
		for (rul = 71; rul > 0; rul--) {
			continue;
		}
	}
	if (3909 != 3909) {
		int pq;
		for (pq = 37; pq > 0; pq--) {
			continue;
		}
	}
	return string("yyydjbfarrnoyyhibvj");
}

double wmgzbqs::qiawzzjjsbjqqedkdjrw(int dwxsckreblzavw, double qglvoyrggu, double hiycz, string xpunbkvkxztyv, double nkvzwurlhubnvu, double cvzwgantl, bool fqabougjwxqik, bool byiynzrmzkgksjo) {
	double ojydsnqsqv = 70331;
	int gbrcjn = 4929;
	string xggmjykz = "mibgjsecnshleauvfiyybviakwxpzrgehsiuvgsllsyqtmsvvbrsiubyjtuwvloesclycetdnnygxyuydgdfrkhivyvmmtpcrrji";
	int ustegsrl = 224;
	double nrhgecgilpepa = 11348;
	if (4929 == 4929) {
		int xf;
		for (xf = 27; xf > 0; xf--) {
			continue;
		}
	}
	if (70331 != 70331) {
		int zv;
		for (zv = 59; zv > 0; zv--) {
			continue;
		}
	}
	if (70331 == 70331) {
		int ki;
		for (ki = 34; ki > 0; ki--) {
			continue;
		}
	}
	return 59983;
}

string wmgzbqs::saksecxxokn(string gqjbauiwebyjlgh, string fjhoh) {
	string lgcpyatbisharg = "tfogpxdqmwvfwkyagxfshqfre";
	double mbnrtsecboyems = 29214;
	double mrfawuw = 13398;
	int qztjlqqy = 911;
	string lumuaozjwtok = "dixlrlbqvnqfkjmjzhojwkkaempjhkcoiqhxokiqfqtxxsnzgbgxzqmimlxnwvnziifpatqlcylnjxpaevbewguxkv";
	int wegpgdabzthby = 3466;
	double miygblafcjv = 7610;
	int joqugdob = 3432;
	bool kywnxujuami = true;
	double blfuaapqtx = 15126;
	if (true != true) {
		int mgzkwrcof;
		for (mgzkwrcof = 2; mgzkwrcof > 0; mgzkwrcof--) {
			continue;
		}
	}
	return string("zq");
}

void wmgzbqs::ixxwcqwrqrzvdussrs(string zbstgl, string srrybrexgcnbpgp, double xtlwtwyzk) {
	int ixmxknsha = 1334;
	string iuqcmbanajcp = "mglkynsbcule";
	string wxscj = "hwgfylwntgnhlobziwngiohvdwwtofqbcqugktmpurpztydczylxnhktviavryiyujnqszzlvujdtqiekknjwlftiwiwkfdtazh";
	bool gaukpr = false;
	double yrcmfeuixz = 11220;
	int gcdbg = 4181;
	string ivzbtwydlghtpo = "ilzhbdkvkpmzrlrecamabwknav";
	int cgucrpoza = 5112;
	bool ftvbya = true;
	if (string("ilzhbdkvkpmzrlrecamabwknav") == string("ilzhbdkvkpmzrlrecamabwknav")) {
		int ivqzgmf;
		for (ivqzgmf = 8; ivqzgmf > 0; ivqzgmf--) {
			continue;
		}
	}
	if (true == true) {
		int gyt;
		for (gyt = 99; gyt > 0; gyt--) {
			continue;
		}
	}
	if (string("mglkynsbcule") == string("mglkynsbcule")) {
		int tqvqvucmt;
		for (tqvqvucmt = 5; tqvqvucmt > 0; tqvqvucmt--) {
			continue;
		}
	}
	if (false != false) {
		int ghbszcqp;
		for (ghbszcqp = 11; ghbszcqp > 0; ghbszcqp--) {
			continue;
		}
	}

}

bool wmgzbqs::dcowoccdvzymexmaxdzfxlnu(int fjbxuwyicufks, string nmwhsygtkefghi, double vuijxnoypxeto, string xvbrzhxqtapaajc, double qgnmozspdne, double ipunxrvwk, string uskbkcmsikzbl, double wlmlhen, double usmvc, int sxwirdrivuvqym) {
	int mdsuf = 4254;
	int vifagrfkvfevre = 4521;
	bool vfkjirrpginye = true;
	double yvvydahegirwpo = 41160;
	int obaghmarkgwnz = 2641;
	double lalzzspjq = 14713;
	double nwkgwsdotz = 38502;
	string zrrioduqhgkcgry = "bnzfhvolkzmzkvlqcdeedpwhxyqmhurlakxedqeotfxemsnnbqjxdzigcppvznzupmsncmeulvnbnrfokhwi";
	return true;
}

bool wmgzbqs::qpzeqpytadtbzpraqxho(string dobushvrxheybob, bool rkzqc, string inghbdvjdwxzcb, double xghdwruagblhpje, bool trrlqiymc, string zbfnsudmxmaayn, int xkchte, bool yrftgb, string uiwwpzyehyeddo) {
	string tdhzxiyol = "lirqtvnjnamkvlzdumjtqqptndcahhcynnjjppuhikjwwhedkbgblmaoxknrbyvwxjzgqrbgvtcwqkvvzxbnljaujpeqeg";
	double awyyi = 3955;
	bool tfxntumbnv = false;
	if (3955 != 3955) {
		int zgyjfz;
		for (zgyjfz = 52; zgyjfz > 0; zgyjfz--) {
			continue;
		}
	}
	if (3955 != 3955) {
		int cfsi;
		for (cfsi = 32; cfsi > 0; cfsi--) {
			continue;
		}
	}
	return true;
}

int wmgzbqs::ozqedtwmeijskxffjyp(double gvvhmlbedymbqf, bool zpryxulkok, int vmnivi, double wwsbisknfalra, double ktlzzdkg) {
	string tfrmaubtf = "ggvhutyvrwzpev";
	int smxyh = 5290;
	int xcfxu = 6579;
	int zvcruxo = 713;
	bool glvtza = false;
	bool tzqpkglgglnl = true;
	if (string("ggvhutyvrwzpev") != string("ggvhutyvrwzpev")) {
		int ulhd;
		for (ulhd = 14; ulhd > 0; ulhd--) {
			continue;
		}
	}
	if (713 == 713) {
		int ceuic;
		for (ceuic = 23; ceuic > 0; ceuic--) {
			continue;
		}
	}
	return 82704;
}

bool wmgzbqs::iuxhendbymbrrme(int ngevrlbtrtxvy, string txitpzn, double gvpkplqptdpokts, string sflosaxovio, string ramiqledaih) {
	string twobz = "pxppslncrlcnacfcukttfaoerw";
	int yxatuyhpjbf = 2208;
	string oyzojeb = "zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq";
	double kzpqvfyfj = 30945;
	double piymvrdag = 44856;
	if (string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq") == string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq")) {
		int rwg;
		for (rwg = 81; rwg > 0; rwg--) {
			continue;
		}
	}
	if (string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq") != string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq")) {
		int xivrpy;
		for (xivrpy = 46; xivrpy > 0; xivrpy--) {
			continue;
		}
	}
	if (44856 != 44856) {
		int gxobm;
		for (gxobm = 15; gxobm > 0; gxobm--) {
			continue;
		}
	}
	if (30945 != 30945) {
		int ci;
		for (ci = 81; ci > 0; ci--) {
			continue;
		}
	}
	if (string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq") == string("zuhmkntkvkotuqqukpupqmgogeztgwsqxlidrediesobq")) {
		int zkdvrqbwjo;
		for (zkdvrqbwjo = 51; zkdvrqbwjo > 0; zkdvrqbwjo--) {
			continue;
		}
	}
	return false;
}

string wmgzbqs::esnosfawynrjffjjq(string lixta, bool bbvcivat, string ujtyvjefrgmj, string pnhrgstwnwbdbaq, double kmggtynksfkagh, double ajwmpik) {
	double smwkxyzd = 49878;
	int yfjgcgqnc = 5737;
	if (49878 != 49878) {
		int hns;
		for (hns = 99; hns > 0; hns--) {
			continue;
		}
	}
	if (5737 != 5737) {
		int utp;
		for (utp = 94; utp > 0; utp--) {
			continue;
		}
	}
	if (49878 != 49878) {
		int voozvq;
		for (voozvq = 33; voozvq > 0; voozvq--) {
			continue;
		}
	}
	if (5737 == 5737) {
		int qyuxqjejy;
		for (qyuxqjejy = 20; qyuxqjejy > 0; qyuxqjejy--) {
			continue;
		}
	}
	return string("kvmjrndpvsxwnkxxkpsg");
}

bool wmgzbqs::kbcybdivejjruowqd(bool zzyliwbmu, int atteopwwuqobjnt, double epshmzztopunpa, double hvfcylvwzfu, int ognhsdbtzvh, string zogflr) {
	double ckzfurm = 9271;
	int jnehmijqjp = 760;
	string jafhvps = "zqvcuzurwzsvjispepwowcetwvisuseqiaaccjgwifgggqgvvrpnxheoeuwoeagdjfzqmfjrjf";
	if (string("zqvcuzurwzsvjispepwowcetwvisuseqiaaccjgwifgggqgvvrpnxheoeuwoeagdjfzqmfjrjf") == string("zqvcuzurwzsvjispepwowcetwvisuseqiaaccjgwifgggqgvvrpnxheoeuwoeagdjfzqmfjrjf")) {
		int rcfczpmg;
		for (rcfczpmg = 17; rcfczpmg > 0; rcfczpmg--) {
			continue;
		}
	}
	return true;
}

string wmgzbqs::poldwspgkauu(int sgarkjfmzr, string rthpellgc, bool wvgdggqyndv, bool qszcq) {
	double zerkavnbhvny = 9385;
	string wxjfpwnayodgb = "ruzikyjwaetrffffjdhsckwienkslxpkrywxzbnkklsuljurmthhqcqikkiptvkfov";
	bool jqfwolpm = true;
	string fofpl = "wxecocwepkuegmouzrhjejevnotzxziuxrhhqalyuzgubfiubtkumedznhnbngnkzekmry";
	int xykvkygr = 5317;
	int ityeqdlg = 1603;
	int qsngan = 1256;
	bool zlxupic = true;
	int ltgmqinvfdrqvh = 1437;
	if (1603 == 1603) {
		int zckmb;
		for (zckmb = 86; zckmb > 0; zckmb--) {
			continue;
		}
	}
	if (string("wxecocwepkuegmouzrhjejevnotzxziuxrhhqalyuzgubfiubtkumedznhnbngnkzekmry") == string("wxecocwepkuegmouzrhjejevnotzxziuxrhhqalyuzgubfiubtkumedznhnbngnkzekmry")) {
		int ubp;
		for (ubp = 96; ubp > 0; ubp--) {
			continue;
		}
	}
	if (1603 != 1603) {
		int etpeuptzo;
		for (etpeuptzo = 8; etpeuptzo > 0; etpeuptzo--) {
			continue;
		}
	}
	return string("bxvvzhmcknocbhdgs");
}

string wmgzbqs::ebgiigqfolagblhxurtp(int nyvxwn, int povokmqwo, int jdurx, double mtbuwkbkonfnm, int uepaisv) {
	int swkjgbsbhszsya = 4387;
	int yifpqu = 2286;
	bool ilyoqmofnof = true;
	string oaxyuwa = "xcxxjdzfpybuntijrsnqeogjwmvaunifs";
	double rczkdiwv = 23658;
	string wdvbauih = "zvgoizxyyeqrimrhutzphofguftluludtpheyvnoegijkorrbtejmqahkvgcfvjpaavgtnitooacurzwwbtgwmqhi";
	if (true == true) {
		int knohzp;
		for (knohzp = 25; knohzp > 0; knohzp--) {
			continue;
		}
	}
	if (4387 != 4387) {
		int oc;
		for (oc = 15; oc > 0; oc--) {
			continue;
		}
	}
	if (4387 == 4387) {
		int axdlz;
		for (axdlz = 17; axdlz > 0; axdlz--) {
			continue;
		}
	}
	if (2286 != 2286) {
		int jzv;
		for (jzv = 73; jzv > 0; jzv--) {
			continue;
		}
	}
	return string("pmsda");
}

bool wmgzbqs::ocokzowblaa(int sqxrjdwhrxe, int dvgjjpoqambze, double hvzmojob, bool ecdclr, int pgjqwmxnsrvh, string ihvsvpjwnek) {
	double eakufqoolyqg = 47772;
	return true;
}

void wmgzbqs::pbcurejadgch(string hmnatxizhkutrp, double mpvikbjqmadms, double inupmh, double tnbchzsgc, string bnxwmk, int nkgnuulocbcvdq, bool jkzun, double bwbksnqyuwst) {
	double volualwsenbeczp = 65491;
	int dpitkkc = 5986;
	int lgdruekdxt = 2052;
	int sruagojgpgqwqwi = 113;
	bool hngchtw = true;
	bool myrghkhpa = false;
	double xsqrludzczn = 8208;
	if (false == false) {
		int qjryhfy;
		for (qjryhfy = 52; qjryhfy > 0; qjryhfy--) {
			continue;
		}
	}
	if (65491 != 65491) {
		int pxtj;
		for (pxtj = 86; pxtj > 0; pxtj--) {
			continue;
		}
	}
	if (113 == 113) {
		int hoott;
		for (hoott = 30; hoott > 0; hoott--) {
			continue;
		}
	}

}

double wmgzbqs::iabhzfjxeiuhowoqpfvnljv(double fxqfc, string eiofmafhwalppjz, bool actbjcuqwuzym, string nowrnyvbjo, double lbbefumi, string zsvynhenibws, string eqcbnve, int abdqgun, string utlbddyvlzp, int bltidfcstri) {
	int atcydkkhqcvym = 1248;
	int czuqmvb = 229;
	int wbmweiyvoc = 3382;
	string axbdcxdpiyqdf = "lqcdtdxxkhsv";
	return 90192;
}

bool wmgzbqs::wcdtnndofacyeggoibnlkzmb(string evzdshujrkcjdbz, bool kaztzfvrqhjojgy, int gwvvdhkxpvgjb, string jdbop, double wvjgqqpignda) {
	int wnffwerequbrbf = 1084;
	string kxyvaa = "fqbnxnrrwsggqdyyxjxjfi";
	bool jjbiozuxqza = true;
	bool deoit = true;
	int xnnyftg = 1457;
	bool fgcsisj = false;
	bool lfwudnprdv = false;
	bool ngslzlhw = true;
	double nsxfnz = 17530;
	double zbxounebnaacx = 30714;
	if (false == false) {
		int wwmbvemr;
		for (wwmbvemr = 22; wwmbvemr > 0; wwmbvemr--) {
			continue;
		}
	}
	if (string("fqbnxnrrwsggqdyyxjxjfi") == string("fqbnxnrrwsggqdyyxjxjfi")) {
		int zsvoabijif;
		for (zsvoabijif = 11; zsvoabijif > 0; zsvoabijif--) {
			continue;
		}
	}
	if (true == true) {
		int brzjincwk;
		for (brzjincwk = 20; brzjincwk > 0; brzjincwk--) {
			continue;
		}
	}
	return false;
}

bool wmgzbqs::oewzbkvaqfnclhztslmmxy(string kjzvgbsqf, int mxsqpczqdrg, string cshhnzalggbafps, int pmvmlxzvau, int zpkfuvrqmtkbcso, bool hrivfrdmtgtt, int ueqgkt, bool rdwotvv) {
	string gafvp = "rmp";
	string zodqfozdemx = "ngukcsghisyoi";
	bool gxrczhqb = true;
	double drsgsvgf = 33930;
	bool fsrmnooqrpgpptr = true;
	int zudefhamgmkzift = 237;
	if (33930 == 33930) {
		int sh;
		for (sh = 7; sh > 0; sh--) {
			continue;
		}
	}
	return true;
}

int wmgzbqs::grqgfwrtin(bool glzupowhwffc, int brodntiubzaiqws, bool eujfj, bool zbmxbqu, double zktufzk, int wiuzotaoslhdbc, string wtzsvz, double mvaxsigohvq, string rcqlys, bool oeqvmsjs) {
	string rthrmojso = "nsmkmazwxrqqobkhzvvefgjwhnappdcmxkzfusgyhqamoxoumiraoeipazfgbteonzvihovpghxxowkuytsdrqzjozzcbk";
	bool rbapufevcgzortg = true;
	double tlthdz = 20734;
	bool pichjtkudsh = true;
	double vlhgfbar = 27771;
	bool hsqxiznaurd = true;
	bool ctwsefvfkksscbl = true;
	string peapkylzr = "zopukqpeduacqhyscjzfmjokewnlmoxdaynlxqsdbxiztsljzlwcdbcjgek";
	if (true == true) {
		int bhdebdg;
		for (bhdebdg = 64; bhdebdg > 0; bhdebdg--) {
			continue;
		}
	}
	if (true != true) {
		int fdwndyy;
		for (fdwndyy = 68; fdwndyy > 0; fdwndyy--) {
			continue;
		}
	}
	if (true != true) {
		int el;
		for (el = 15; el > 0; el--) {
			continue;
		}
	}
	return 76518;
}

int wmgzbqs::pvwewgvfxhlzynonimq(double fnvqgcbyag, bool eqgrpyqzeeopct, double nzvgbnsmwyb, string qwieaka, string mddxydxbbc, int selzdlo, bool aanae, bool aihzh) {
	bool gzvkdq = true;
	bool qkkatqqrae = true;
	bool psoebujkgcnnbdl = false;
	bool hcoacczsrud = false;
	int ukleipvfahcoy = 7328;
	string rkiqbe = "ogygqtntmpsgngvcl";
	double qnqcopgvibz = 55211;
	bool ksifmybvjmmd = false;
	if (7328 != 7328) {
		int ri;
		for (ri = 38; ri > 0; ri--) {
			continue;
		}
	}
	if (55211 == 55211) {
		int fsff;
		for (fsff = 27; fsff > 0; fsff--) {
			continue;
		}
	}
	if (55211 != 55211) {
		int wlu;
		for (wlu = 97; wlu > 0; wlu--) {
			continue;
		}
	}
	if (7328 != 7328) {
		int hyivyal;
		for (hyivyal = 54; hyivyal > 0; hyivyal--) {
			continue;
		}
	}
	return 21573;
}

int wmgzbqs::tljxsjmfljcziwzvoe(bool spxryv, bool zifwvdvlzhpo, double gxchoo, bool akwio, double jzyvh, double qsdicxi, bool bmuyfvduitetwcr, int phptorgdfzhhz, double qyqxksnydtsu) {
	string mbabl = "jnjeyzibgfrdpjglxafzntv";
	string xnziw = "xbkegmbcyyzwrnqhxieorcvvliaxkkdgwirehsphizbsmhrsvclyoxvgfwwwrujqugmimw";
	return 75374;
}

bool wmgzbqs::kbpvqacfkgduurrdl(string itotaifl, int fklhoztkoskx, bool yxmyw, string fmofpavdke, string etecnrylns, bool wlqektaiziupyql, int dhlkncwmiwbghy, bool slvzvflllpwdpaq, int lwtao, bool bzkwvmbvw) {
	double grxkriffgxasnkg = 297;
	return true;
}

wmgzbqs::wmgzbqs() {
	this->ebgiigqfolagblhxurtp(1589, 1233, 6028, 4942, 3100);
	this->ocokzowblaa(7004, 1054, 17855, false, 2202, string("lhl"));
	this->pbcurejadgch(string("ywckwfoshziyvgneeatsjkaaetvwnrtivltohhumrmzdyakmljwklkgrdvpvaycmbqpnsiqftpfdxgpnmjsoxjfnnfpgdjjzrnev"), 9750, 9220, 70650, string("rnbolijrwrhphdgtwugkcznlbtbkurzdlcykwtzegsbbm"), 345, true, 8687);
	this->iabhzfjxeiuhowoqpfvnljv(3054, string("ogfquhoaptpwkdjrtenrwcetyatiwqsloirzuvluex"), true, string("xilwhirivuhhxbbbzplfxhldplwi"), 21716, string("teysbkbqj"), string("zgeqtrrgczidlj"), 5384, string("ctwdjchnifrgzuixtpykukqeigfvcfzzvybjdbwgcb"), 2550);
	this->wcdtnndofacyeggoibnlkzmb(string("dnubhvzpnzlbxzenxx"), false, 662, string("flpsqsckgkx"), 3186);
	this->oewzbkvaqfnclhztslmmxy(string("bogxjwysnbnbjsmoutrjiqlursjednigxpbk"), 1277, string("sbqqkjsabokbosrcszvsfatucmqpgifvlygftfmftukbnvaapeizbtbjfuemkhdhtttyqm"), 201, 4309, false, 2126, true);
	this->grqgfwrtin(true, 3513, false, false, 10308, 163, string("kujmpvhcbjphtmgrwnpnbuuarjyhejqygwieiyuwmbdgjmselyjywhkvgcxavhbpqiprblsvrxhoxrbjvmseyweikwg"), 17116, string("vhoxitrmdpwboaxmlvfxjvwdxlhbcniretrlzfmbbuyughmhuqcdmzxjlzgbbeeawtogxolvqpkiom"), true);
	this->pvwewgvfxhlzynonimq(698, true, 35320, string("qwtazoxbihykspogfmbhpomibzsvmwyddbhxxigxfoudynexxkekstbqpcsxbsiiufxeopgmjmmrnpfmglacx"), string("shx"), 1193, false, false);
	this->tljxsjmfljcziwzvoe(false, true, 585, false, 62592, 32081, false, 2605, 24178);
	this->kbpvqacfkgduurrdl(string(""), 448, true, string("zdzmcytohlxrsdddvmvgfbwlztlxhmiwsxjxhlthtjbrlmvyoadcwbatygn"), string("pfsebgsztakihkcqwwalhelsj"), true, 968, false, 812, true);
	this->dcowoccdvzymexmaxdzfxlnu(116, string("nsqztxgxjhwnhpqmasxobyzhwozmnthsjfwucwlvikvdcrjobttpzwuodsgpmxecromb"), 11573, string("cjqrwowlsqerlcnievqeycvlcdsepraxcncyjhwhftjqkmkxtbjuaratzujxnbqnpqqjrszhcghdsv"), 55118, 49436, string("kvxolfinplrjvacacgpwymepz"), 1423, 12653, 1509);
	this->qpzeqpytadtbzpraqxho(string("avheygwnloggtvvbxgmpulysvfbuuqnhflwqftodcjhslwtvujrdguwgovdcknispxvzvtntmdgvy"), false, string("laeu"), 36331, false, string("eszckcmonycmzbpyuebzfqlxhagfafgyuapcnaywybczdckgrisqukwltzqbqxtdksqgwhjmrgzwolmetslpcew"), 1044, false, string("peiirjnmqqwvuzrubileasshhuwnbcbuqbovzlyvyovrzgwqdjociqpewwjdrrlfo"));
	this->ozqedtwmeijskxffjyp(10786, true, 294, 4269, 68189);
	this->iuxhendbymbrrme(399, string("hktrihfijs"), 10854, string("kfawoahpxlpfzfzpwakbvczyvhnfoqtapneirtbbrfwultvovswwtewumsfzzgcto"), string("htfwvjzvyfubwinkk"));
	this->esnosfawynrjffjjq(string("sluiskxiyfuvyilxgtnduvu"), true, string("ndckqdyfvxrqcooogsxliroyrriyqid"), string("vllltmjkdmqtymtkv"), 11011, 271);
	this->kbcybdivejjruowqd(true, 2880, 15776, 68649, 1890, string("hauiikhftyz"));
	this->poldwspgkauu(6429, string("wtodgpumhioyelewpaxbyznsundehokfzidklmcoakqauhredybxvyfrmgtotfdxweowlkwjqjwtoreym"), true, true);
	this->zhprwofqrtkouwkumalrs(10682, false, 3633, true, 171, 3782, 293, 23275, 54487);
	this->uliqhmfjvmrbdc(string("uxfwgnfbzafyzsrfnuviuzhuptldowihyfmcyjqbucdmugyjweivcgnlgygvtmz"), 3249, 4841, 1282, 51633, true, 466, 9185, 1744, 26451);
	this->oohbfzpryhseogvhk(1063, 3485, string("rgkmaroxthznzoabxfuwdnjujgbglnehrhfuaqjxynhalf"), 57023, 48481, false, 9375, 2555, string("keztdbpvvgglgdwrtbvzqaodschdzatvpxzsoeropvzpwdocnlaoulttj"));
	this->pfrjcnirrvzckjfhfoddwlv(1297, 24244, 6858, 6960, 4129, string("gqehekpmjnjwrpar"));
	this->qblynuxgphlhczrvjlvid(19446, 4081, string("glknuesxaixfnfmmxllszsxrmpvrdeijcbwmynoqrclpjjkzxxzsteturmvejlmhnumyhbeixaossbae"), false, 63647, string("gxrrjsbtxyzitneiyidntsrpyylbdrsgdnskxuhgtptiwvffcpy"), 5431);
	this->wnmllczeufifdpbmuau(1255, 635);
	this->qiawzzjjsbjqqedkdjrw(1698, 8890, 4284, string("bp"), 19062, 38564, true, false);
	this->saksecxxokn(string("ipkrnbrnaawxhupktoniuultspqkwzkexyvkqiuupocwfsobociqptomgtgpmvttzz"), string("cubwrijcjulyvuqzdzsghkzxwng"));
	this->ixxwcqwrqrzvdussrs(string("vvuuaqfvhjfkvmbonovokpcsqvtvomqfspehmjurpbkmedjkni"), string("lqxkiihgxhnn"), 10410);
}
