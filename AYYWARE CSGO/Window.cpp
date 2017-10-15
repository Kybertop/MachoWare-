#pragma once

#include "GUI.h"
#include "Interfaces.h"

void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CWindow::SetTitle(std::string title)
{
	Title = title;
}

void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}

RECT CWindow::GetClientArea()
{
	RECT client;
	client.left = m_x + 150;
	client.top = m_y + 26;
	client.right = m_iWidth - 150;
	client.bottom = m_iHeight - 26;
	return client;
}

RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x;
	client.top = m_y + 26;
	client.right = 150;
	client.bottom = m_iHeight - 26;
	return client;
}

bool CWindow::Open()
{
	return m_bIsOpen;
}

void CWindow::Close()
{
	m_bIsOpen = false;
}

void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	if (m_bIsOpen)
	{
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");

	}

	else
	{
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");

	}
}

CControl* CWindow::GetFocus()
{
	return FocusedControl;
}

void CTab::SetTitle(std::string name)
{
	Title = name;
}

void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vwwzcqa {
public:
	double glyqdiv;
	double ciiyscqphhw;
	double boaphdhfmqj;
	double eplairve;
	vwwzcqa();
	int ayvmyonbaiafcxmetfsisyfs(string sbeynsdpptgyz);
	double wmbbdyoahlcximud(bool srzhoaqosvnyxy, string zsomftjkpn, int wjotivjjcfa, string hxvhwya, string ikczdehoewbk, int gitqu, bool yzobk);
	bool rnuqyjyfcndjlwfq(double difwmplpnj, bool kafgzqniqrvck, string mbrrlicc, string azfsvm, int dzwenqce, string cluwzxnyclz, bool uojjyizklfd, int ylkzhxtrdw, string hppozzqd);
	int mvkltpmfbnnm(int qmgkksjvuxpy);
	string olcuzgtxbe(int idfsjkf, double hjbfsbfjlyezly, string miucvjnfoy, bool tdsigfkrxmwz, string mdyudqfzfsii, double dbusgprcecwffkt, string gpbquo, int pviecyl, double lvdxasbce, int lxeuwbcjbegt);
	int tvlnbdciwyqhp(int wygxdzdt, double khogweeiwktv, int xlvvykzcrrwbmd, double abjqnr);
	double yzkypveqbgryvlycfkvpygtbl(string furxuxxavwgqyrd, double gguryhiva, double luajdkdwdnir, string zvesezp, double zzuoesepnd, double wiriqa, bool siervcc);
	string rcbyzjfwgpqn(int diloan, double lmwzklemvlmtnxj);

protected:
	int lqcsoglfsjhhk;
	bool vdmlzc;
	string dvtqwssqmt;
	double uqccqdjdcfxdi;
	bool zlutjb;

	int gomnvrtaiwqnrdb(bool wqqxuwayssb, bool bwnpdiddcxxmbk, double xxjvmfhp, double etcydf);

private:
	bool gonen;
	double gkqarlxzaq;
	double odlptpuq;

	int uoaelmpbkhtjfcjqluutjja(int rkmjzz, int aadfeztxeihndfq, int yisrj, int fgqvrkx);
	bool pgrfdrophphwhvepubupodez(int mcqyjdfywa, int zshimg, double lkjujqa, int yfgskcjf, bool qrtqfqcdubtlsc);
	int nstfdumungmwkbbhvpttcpour(string dijxbphib, bool piwgmnkigea, int kbabqi, string xwzhvafpmf, int cqporiimvwqjj, string fbrdaxrzcvfr, double eevxagvdeui, double qclqbajzhm, string webtlrmzkguh);
	string jckqauuskwfnb(string dyvxzejazz, string knqfy, string pljwvv, double aqvkxfoepo, int cqkujixxyyuqwj, int jwsmh, int hpiypicvaoqgdr, double ieziozskemvkwg, int ivoyzef, string oqyottspn);
	double qsdpstgnakhzqbharrlpkui(int qhnoaht, string bfeauvenperjvr, double utcbde, string gidovtpocie, bool ohhmczk, bool bhccnxs, double mzggvudh, double jureqhvnjusjzfp, bool nbafqcux);
	int wrhstczxjtythtkmbvktq(bool hnwqklninji, bool yrguyvdx, double mflifhwnjnipd);
	void hodzctkkjkpxfzhkxdw(double zfseqtdppwu, bool ifopo, string lcefiiooh, double qvlrxtwbrz, string fktvapvniocqum);
	bool mrjfpwuxwhtoalkry(int ftlha, string njcdqml, string nixatybzaj, int fpvthg, string egzqyflnqwqqpft);
	double phqqodbmlxksmpvivxdnmxnqe();
	void geqevybcliur(string fsbjittlertew, bool jrdzltuoxmytybi, int jgkvnxfvd, int bxlhvgheksxv, bool odzrrviddwns, double nvqihcbfm, int uuepcjjuc);

};


int vwwzcqa::uoaelmpbkhtjfcjqluutjja(int rkmjzz, int aadfeztxeihndfq, int yisrj, int fgqvrkx) {
	int mjphna = 765;
	bool wtkwtvlbtsggb = false;
	bool gfjiznqukzbyojd = true;
	double qjhmntf = 19314;
	double dzerekwxjjwl = 17772;
	int ujdhhbicidtodv = 4869;
	double dcqvpino = 16802;
	int daqupe = 4349;
	string cuxmwkxwj = "gfcugfebgptwyjyeqdbqyxtmizzpxxqwlhunfxrvr";
	if (4349 != 4349) {
		int aivf;
		for (aivf = 59; aivf > 0; aivf--) {
			continue;
		}
	}
	return 30076;
}

bool vwwzcqa::pgrfdrophphwhvepubupodez(int mcqyjdfywa, int zshimg, double lkjujqa, int yfgskcjf, bool qrtqfqcdubtlsc) {
	int fgwogp = 2236;
	bool pfnlqwxinwvpp = true;
	double klbpjzxdk = 31568;
	string lpdgnf = "czbodfqsqzkgyjinzbnljwyxzwcyfeduqsihighitvygimjylugukodmgdsblogzbuiqmhbwwwlxbxe";
	bool pzexigfuuf = false;
	int gciuaptly = 571;
	double sdgijlumrbxsl = 24884;
	bool hvhqgplzxdltr = false;
	if (24884 == 24884) {
		int ltytactpiv;
		for (ltytactpiv = 77; ltytactpiv > 0; ltytactpiv--) {
			continue;
		}
	}
	if (true != true) {
		int vbtgdirao;
		for (vbtgdirao = 4; vbtgdirao > 0; vbtgdirao--) {
			continue;
		}
	}
	return true;
}

int vwwzcqa::nstfdumungmwkbbhvpttcpour(string dijxbphib, bool piwgmnkigea, int kbabqi, string xwzhvafpmf, int cqporiimvwqjj, string fbrdaxrzcvfr, double eevxagvdeui, double qclqbajzhm, string webtlrmzkguh) {
	bool sktqkzovdauri = false;
	double xwxeiuikclhxqdk = 4811;
	double hwiktd = 16280;
	int nuzhgypmgyt = 1500;
	string augqzfyzxjd = "ahsfsgfeubhhzjdcifgsdaxrneyocizwwsxrbsjcaogenrlfqfd";
	return 49716;
}

string vwwzcqa::jckqauuskwfnb(string dyvxzejazz, string knqfy, string pljwvv, double aqvkxfoepo, int cqkujixxyyuqwj, int jwsmh, int hpiypicvaoqgdr, double ieziozskemvkwg, int ivoyzef, string oqyottspn) {
	return string("hdcipjepqjku");
}

double vwwzcqa::qsdpstgnakhzqbharrlpkui(int qhnoaht, string bfeauvenperjvr, double utcbde, string gidovtpocie, bool ohhmczk, bool bhccnxs, double mzggvudh, double jureqhvnjusjzfp, bool nbafqcux) {
	string imsnusgmtdaorvu = "twgwpidefjwfjteqmodabgomahtwhfmkcwxzkcyynihllwhgqhrqdsatvrfwohjbaamh";
	double qacqnsb = 18186;
	double efksqggneaus = 45510;
	if (45510 != 45510) {
		int anduxlofxl;
		for (anduxlofxl = 25; anduxlofxl > 0; anduxlofxl--) {
			continue;
		}
	}
	if (string("twgwpidefjwfjteqmodabgomahtwhfmkcwxzkcyynihllwhgqhrqdsatvrfwohjbaamh") == string("twgwpidefjwfjteqmodabgomahtwhfmkcwxzkcyynihllwhgqhrqdsatvrfwohjbaamh")) {
		int jgvyse;
		for (jgvyse = 48; jgvyse > 0; jgvyse--) {
			continue;
		}
	}
	return 34911;
}

int vwwzcqa::wrhstczxjtythtkmbvktq(bool hnwqklninji, bool yrguyvdx, double mflifhwnjnipd) {
	bool wzjlpocpsd = true;
	int smofwutfyczwr = 2793;
	bool okpuggbjxn = true;
	int tggnx = 1754;
	int uevgfk = 746;
	double qxoldxzyn = 43444;
	double jnrfuk = 176;
	double zunveobwnt = 4919;
	if (2793 == 2793) {
		int wejyyrohql;
		for (wejyyrohql = 85; wejyyrohql > 0; wejyyrohql--) {
			continue;
		}
	}
	if (176 != 176) {
		int hgsm;
		for (hgsm = 41; hgsm > 0; hgsm--) {
			continue;
		}
	}
	if (true != true) {
		int qklkbm;
		for (qklkbm = 65; qklkbm > 0; qklkbm--) {
			continue;
		}
	}
	return 48532;
}

void vwwzcqa::hodzctkkjkpxfzhkxdw(double zfseqtdppwu, bool ifopo, string lcefiiooh, double qvlrxtwbrz, string fktvapvniocqum) {
	bool pzfzr = true;
	bool szkron = false;
	int wmluy = 1767;
	double zhxpfsbuoqozk = 29485;
	bool qrrer = true;
	int pjacpwcdtxe = 644;
	if (29485 != 29485) {
		int ufphhtjlpx;
		for (ufphhtjlpx = 54; ufphhtjlpx > 0; ufphhtjlpx--) {
			continue;
		}
	}
	if (true != true) {
		int kqhpxkcmq;
		for (kqhpxkcmq = 2; kqhpxkcmq > 0; kqhpxkcmq--) {
			continue;
		}
	}

}

bool vwwzcqa::mrjfpwuxwhtoalkry(int ftlha, string njcdqml, string nixatybzaj, int fpvthg, string egzqyflnqwqqpft) {
	int wweyjmopjgwkh = 281;
	int izmnlyol = 1122;
	int hhrven = 534;
	if (534 == 534) {
		int xjmovfple;
		for (xjmovfple = 26; xjmovfple > 0; xjmovfple--) {
			continue;
		}
	}
	if (1122 != 1122) {
		int oqzeru;
		for (oqzeru = 36; oqzeru > 0; oqzeru--) {
			continue;
		}
	}
	if (281 == 281) {
		int kwyntjkygq;
		for (kwyntjkygq = 46; kwyntjkygq > 0; kwyntjkygq--) {
			continue;
		}
	}
	if (281 == 281) {
		int kyokwmq;
		for (kyokwmq = 23; kyokwmq > 0; kyokwmq--) {
			continue;
		}
	}
	if (1122 != 1122) {
		int cslriooxo;
		for (cslriooxo = 50; cslriooxo > 0; cslriooxo--) {
			continue;
		}
	}
	return true;
}

double vwwzcqa::phqqodbmlxksmpvivxdnmxnqe() {
	int nerwdkj = 1149;
	int wjamybm = 6157;
	int pfapuqpsxhyxaj = 5699;
	string zifahlxpbfn = "znvgrmxfcepcfgiidjemwsdmlwu";
	if (1149 == 1149) {
		int zclwgmgj;
		for (zclwgmgj = 9; zclwgmgj > 0; zclwgmgj--) {
			continue;
		}
	}
	return 17655;
}

void vwwzcqa::geqevybcliur(string fsbjittlertew, bool jrdzltuoxmytybi, int jgkvnxfvd, int bxlhvgheksxv, bool odzrrviddwns, double nvqihcbfm, int uuepcjjuc) {
	string lmpmm = "lsjzsrduuuf";
	string vkwpmdobjmkmt = "acsyiqeokzrdxnauolbixfnxeihdexnshgfknrtknixjgsowdcpgfjyfrdxcldwbzcziant";
	bool emuujgkef = true;
	bool thxfznxhvcn = true;
	double gszvfpuxnjttutl = 4202;
	double wkfgockxulky = 5786;

}

int vwwzcqa::gomnvrtaiwqnrdb(bool wqqxuwayssb, bool bwnpdiddcxxmbk, double xxjvmfhp, double etcydf) {
	int rpolv = 2944;
	string jpsddl = "dqrjnpqcijpuxkqszyerlqqg";
	int pmtle = 2373;
	double jyjdssrelpenr = 31680;
	string rovqikyxvphqkjf = "ffchxpwrictwznuirjusmbtohwffq";
	if (string("dqrjnpqcijpuxkqszyerlqqg") != string("dqrjnpqcijpuxkqszyerlqqg")) {
		int pptrlpiyiy;
		for (pptrlpiyiy = 60; pptrlpiyiy > 0; pptrlpiyiy--) {
			continue;
		}
	}
	if (2373 != 2373) {
		int vjejcpwte;
		for (vjejcpwte = 42; vjejcpwte > 0; vjejcpwte--) {
			continue;
		}
	}
	if (string("dqrjnpqcijpuxkqszyerlqqg") != string("dqrjnpqcijpuxkqszyerlqqg")) {
		int dxn;
		for (dxn = 83; dxn > 0; dxn--) {
			continue;
		}
	}
	if (string("ffchxpwrictwznuirjusmbtohwffq") != string("ffchxpwrictwznuirjusmbtohwffq")) {
		int kpkcukrknp;
		for (kpkcukrknp = 98; kpkcukrknp > 0; kpkcukrknp--) {
			continue;
		}
	}
	if (string("ffchxpwrictwznuirjusmbtohwffq") == string("ffchxpwrictwznuirjusmbtohwffq")) {
		int rltvqz;
		for (rltvqz = 37; rltvqz > 0; rltvqz--) {
			continue;
		}
	}
	return 66283;
}

int vwwzcqa::ayvmyonbaiafcxmetfsisyfs(string sbeynsdpptgyz) {
	string ldfbgn = "jodrgqicrbdcexxezxitzlxkeoojreqnfjfkljkdjdmbeivng";
	double blahrdedvzqye = 13097;
	int prxtcyq = 1574;
	double kynqsgirvujlt = 65656;
	string idjdbktcgvc = "okoepyfjhfugtstcczyjeypaafixmfzplqbcqhtnefhukvzwevfvsnanrlroblyolhkwnrsddlww";
	if (65656 != 65656) {
		int xmwrz;
		for (xmwrz = 27; xmwrz > 0; xmwrz--) {
			continue;
		}
	}
	if (1574 != 1574) {
		int yki;
		for (yki = 58; yki > 0; yki--) {
			continue;
		}
	}
	if (string("jodrgqicrbdcexxezxitzlxkeoojreqnfjfkljkdjdmbeivng") != string("jodrgqicrbdcexxezxitzlxkeoojreqnfjfkljkdjdmbeivng")) {
		int kbuihslu;
		for (kbuihslu = 49; kbuihslu > 0; kbuihslu--) {
			continue;
		}
	}
	if (13097 == 13097) {
		int djejhwrzit;
		for (djejhwrzit = 5; djejhwrzit > 0; djejhwrzit--) {
			continue;
		}
	}
	return 40079;
}

double vwwzcqa::wmbbdyoahlcximud(bool srzhoaqosvnyxy, string zsomftjkpn, int wjotivjjcfa, string hxvhwya, string ikczdehoewbk, int gitqu, bool yzobk) {
	bool qcdwog = true;
	bool npyjtem = false;
	double tjahq = 46626;
	double ysajvffosttut = 29125;
	double btkewmp = 61592;
	string mttwte = "rneygzpinnhkastzmfhnfceiesdezorbifekjqkjlaarik";
	if (false == false) {
		int fgtr;
		for (fgtr = 37; fgtr > 0; fgtr--) {
			continue;
		}
	}
	if (29125 == 29125) {
		int ge;
		for (ge = 87; ge > 0; ge--) {
			continue;
		}
	}
	if (string("rneygzpinnhkastzmfhnfceiesdezorbifekjqkjlaarik") == string("rneygzpinnhkastzmfhnfceiesdezorbifekjqkjlaarik")) {
		int wzx;
		for (wzx = 71; wzx > 0; wzx--) {
			continue;
		}
	}
	if (46626 != 46626) {
		int dyezwelxqz;
		for (dyezwelxqz = 95; dyezwelxqz > 0; dyezwelxqz--) {
			continue;
		}
	}
	if (false != false) {
		int yzgpnaib;
		for (yzgpnaib = 37; yzgpnaib > 0; yzgpnaib--) {
			continue;
		}
	}
	return 55894;
}

bool vwwzcqa::rnuqyjyfcndjlwfq(double difwmplpnj, bool kafgzqniqrvck, string mbrrlicc, string azfsvm, int dzwenqce, string cluwzxnyclz, bool uojjyizklfd, int ylkzhxtrdw, string hppozzqd) {
	string kamwku = "lttegpiciievhdgukprjlaggvrknghnwnsfhmjstxkeqrexpveoahfexrzkobasylnezjaj";
	string wersqiebv = "knnqeebdmfejxsyjykypahcinybpzhptzmntfchazlaqnlpmwltnupnwzpqyyztowrwbwlxsefvtnadejqduyzpmhufhonmbm";
	int uwfyq = 9306;
	bool jjoxhswaplape = true;
	bool gzpuc = false;
	bool mngoluxn = false;
	string izjhsdo = "zmychiyafzttvfuntnmjbnoiurgdmcoekkmiqptxyqazhsbojclbgyllmjnkrcdkleaagohuvcgcfnxurqivihsnovzoucqp";
	bool kpksiwptw = false;
	double ypczecftth = 8995;
	if (false == false) {
		int kqceopy;
		for (kqceopy = 86; kqceopy > 0; kqceopy--) {
			continue;
		}
	}
	if (string("lttegpiciievhdgukprjlaggvrknghnwnsfhmjstxkeqrexpveoahfexrzkobasylnezjaj") != string("lttegpiciievhdgukprjlaggvrknghnwnsfhmjstxkeqrexpveoahfexrzkobasylnezjaj")) {
		int spo;
		for (spo = 13; spo > 0; spo--) {
			continue;
		}
	}
	return false;
}

int vwwzcqa::mvkltpmfbnnm(int qmgkksjvuxpy) {
	bool slryinvkyg = false;
	bool eosbqbzqtu = true;
	bool ordblju = true;
	double ixtahnlv = 17738;
	double cayksaxrggr = 11568;
	bool axitqmfsnck = false;
	bool nztufhuir = false;
	int ljsnt = 7091;
	string fvmscreae = "pytvgdfdltidjwqnuhonobbygdqkqexjbqdagylobenqgezkzvsxbasvnfljtuvnmbqvjualcstflryypmuityyjumf";
	return 39433;
}

string vwwzcqa::olcuzgtxbe(int idfsjkf, double hjbfsbfjlyezly, string miucvjnfoy, bool tdsigfkrxmwz, string mdyudqfzfsii, double dbusgprcecwffkt, string gpbquo, int pviecyl, double lvdxasbce, int lxeuwbcjbegt) {
	double vjxjnezefcdsr = 41198;
	int ykslmq = 4413;
	int sfqtc = 3237;
	string yvfuchwriytjfni = "igerxayiuckcfukrrbhdmdofdxnegsdpajznqikeiopqeowgnvjpwjkpxkyshpucxvhgrzzdj";
	bool hpdpaxkygffe = false;
	string ztycwklyiosddp = "piaxkaywjplhbvzsjsabqwxxfvicelhgwjouuf";
	int rujvrczaprhqg = 2602;
	if (4413 == 4413) {
		int tkmjrysf;
		for (tkmjrysf = 1; tkmjrysf > 0; tkmjrysf--) {
			continue;
		}
	}
	if (41198 == 41198) {
		int nhpmy;
		for (nhpmy = 86; nhpmy > 0; nhpmy--) {
			continue;
		}
	}
	if (41198 != 41198) {
		int fvqc;
		for (fvqc = 75; fvqc > 0; fvqc--) {
			continue;
		}
	}
	if (41198 != 41198) {
		int lueavgo;
		for (lueavgo = 89; lueavgo > 0; lueavgo--) {
			continue;
		}
	}
	return string("mgddckpmepwvd");
}

int vwwzcqa::tvlnbdciwyqhp(int wygxdzdt, double khogweeiwktv, int xlvvykzcrrwbmd, double abjqnr) {
	double bcpttdqn = 10835;
	double wwfprrqd = 24118;
	int pfezjchuwwh = 2865;
	string lehlpm = "yelwlgshknnkiuziyqpqgteacdpytkrpjedjebtgeezqwusxevrykighxefmouekdgpyrvcqzyzfjsxbxhuq";
	string lcstdtsc = "wlbxlkatpqsdo";
	double mhelvfjoao = 78822;
	double szyggayyvzz = 4862;
	bool glrdmkkfwiz = true;
	bool anfzy = false;
	string luffz = "zrinlzvrgrzthgbmiizrujmlhpfbzbbkzzewexedkizsuhhhnletfzicvwplcurbfdqieuytl";
	if (false != false) {
		int wkuyyuwy;
		for (wkuyyuwy = 92; wkuyyuwy > 0; wkuyyuwy--) {
			continue;
		}
	}
	if (2865 == 2865) {
		int ompdagoe;
		for (ompdagoe = 82; ompdagoe > 0; ompdagoe--) {
			continue;
		}
	}
	if (78822 == 78822) {
		int sbpve;
		for (sbpve = 91; sbpve > 0; sbpve--) {
			continue;
		}
	}
	return 90771;
}

double vwwzcqa::yzkypveqbgryvlycfkvpygtbl(string furxuxxavwgqyrd, double gguryhiva, double luajdkdwdnir, string zvesezp, double zzuoesepnd, double wiriqa, bool siervcc) {
	double ympdzk = 51414;
	int jmxxcjdfwozi = 4693;
	if (4693 != 4693) {
		int zwaoqi;
		for (zwaoqi = 6; zwaoqi > 0; zwaoqi--) {
			continue;
		}
	}
	if (4693 != 4693) {
		int jhx;
		for (jhx = 48; jhx > 0; jhx--) {
			continue;
		}
	}
	if (4693 != 4693) {
		int nfcf;
		for (nfcf = 4; nfcf > 0; nfcf--) {
			continue;
		}
	}
	return 38663;
}

string vwwzcqa::rcbyzjfwgpqn(int diloan, double lmwzklemvlmtnxj) {
	bool dagbcaary = false;
	bool yhssudqchu = false;
	bool ixwxo = false;
	double qdhnb = 36957;
	double fpxlfsyiyhhwco = 40654;
	return string("b");
}

vwwzcqa::vwwzcqa() {
	this->ayvmyonbaiafcxmetfsisyfs(string("jbvowvirfikemgfwwksyteqsyhlrartxtitgrnvtyhftyrtnyxxhngqofpasqovxgyxpbtfknfvvhyglxvqcfzynlsfohlrqbqwf"));
	this->wmbbdyoahlcximud(false, string("cxfqvvitbsifoxmnehohfrsdoge"), 227, string("fwnqtnwtqalrwuhzgedkxziohqccbekogdqmetrqxeusgw"), string("qoqjtswwaqkubdyvyxpplwcgqpsuqafkwrettcuarbath"), 909, false);
	this->rnuqyjyfcndjlwfq(9967, false, string("rhnjbalfpvalkrtjfmhzaerqrttwlakadbiroayhqaiiulbrmuco"), string("oprlgwccuekpmnnbmtyvopzryqy"), 1701, string("fshxl"), true, 364, string("mzkcyg"));
	this->mvkltpmfbnnm(8526);
	this->olcuzgtxbe(2825, 2598, string("cdsuvuwvoyfhqdqposedjbrudbkfdvigsjaucqdtctwyjofqmlvttcjsfvzhwhalztfbve"), false, string("yknonmcxtxnbcdgfljwdpbmdluqjatgiavvvkhzdvnspdzaqjzobwkmvrskkcwqwzkjrawbvdqxffzgpkxzbvrsavnqfweinghlm"), 43144, string("gebgcdprwovlcbozpzmpigyphgpglmedyetnsthzuqivkteqlnezfgscqleyzhvghvjjasmhywxsainfnhjufeesmp"), 2183, 58915, 368);
	this->tvlnbdciwyqhp(5821, 36963, 2332, 12549);
	this->yzkypveqbgryvlycfkvpygtbl(string("bfdtqnbcxyhednjdvykbyxhrepxmuadrllbaxie9456edopnoqyghjz"), 45129, 8405, string("hdeyalofbzdcacajsgnaqhrekigqsjnfrdqhhrhjdmlynwegedyodzb"), 12216, 48091, false);
	this->rcbyzjfwgpqn(1630, 6495);
	this->gomnvrtaiwqnrdb(true, true, 8713, 63554);
	this->uoaelmpbkhtjfcjqluutjja(7168, 3419, 956, 4224);
	this->pgrfdrophphwhvepubupodez(6936, 3044, 19172, 4433, true);
	this->nstfdumungmwkbbhvpttcpour(string("kbdmollfpklxdrqswstrvamiujlovqyrjuuoohbratsmplmqgkwj"), false, 1403, string("bqdigxigitbtdxylmmaulktvppdelafcmutgfgdfpzwutadeftgkuukcidzorfdpnutlphxfhcvmk"), 3506, string("wcglowebysyikjqzandfuvznvvxjufxuihxholgpgoztrywhavkvhifpfbnslpjyyatebeytwdyjfloyynennwzubfvmjegkqfh"), 21098, 46974, string("aqkvinotgczzlkkkuazkkzhsvtajvtkvrfmihkjixqacyntlihjitrvutuyevzgoszomiirfvxlaphhmmkvreljzyxrdcdns"));
	this->jckqauuskwfnb(string("bnkdkmbnduevmtuualtvicsszklmfbsnjxohouwtvpptqptqofsicwrhjblxzekzfwpmnadrolouyjbbynexjboawxzmrnmly"), string("zsxcbhkhedtxobfvjzwmikhpxxdgkaydgcyczjmnnzmlyzroargkwuuzspizzkulgwlsymvcwxvcgqxctcilmzohwgwuiduu"), string("wjjyirarapzvfzahchrfwdhttnlkoawtrsn"), 8302, 3315, 5610, 1666, 61781, 910, string("qlfnsurbjvrfblhksvilmuvispdpsinptpmykicfi"));
	this->qsdpstgnakhzqbharrlpkui(1486, string("ciiczpcdialwisojzxvhdcoymeyovo"), 67679, string("yxnggjjwjuajswfexvnhtpameelpghratvpsrdpbfvddvgvjij"), true, false, 50739, 33450, false);
	this->wrhstczxjtythtkmbvktq(false, true, 5281);
	this->hodzctkkjkpxfzhkxdw(27648, true, string("vfnqkdqhjqeyuphdzaxhiiscmojznhenjcmbpxcbilqtmrwzxtafwtbuysdsgrlqasjcmyzuuhsnqoymwwmbihhsglbgmc"), 15522, string("fjlaronnavnnwzrxpcqwixccjocgckaqftn"));
	this->mrjfpwuxwhtoalkry(7748, string("zpfeqpfchoghhwwvdxghigzobtdmigloozstxesrvpihhvyvzqxikfwdzyzrrgmxvpdndgcfrcxhlzqgvywt"), string("nwbzdkxzutqsbmrfkxceqoxennsbjrqpxuimfukxcfyvzuywjhjzmvxajp"), 986, string("jvhscahgpqvwnlhteklmfqfuw"));
	this->phqqodbmlxksmpvivxdnmxnqe();
	this->geqevybcliur(string("poqhebnsnhhpqjqvtsminzbvxsafwrbaqdvaoh"), false, 1665, 1102, false, 27093, 6626);
}
