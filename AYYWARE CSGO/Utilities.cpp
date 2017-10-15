#pragma once
#define _CRT_SECURE_NO_WARNINGS

// Includes
#include "Utilities.h"
#include <fstream>
#include <Psapi.h>

bool FileLog = false;
std::ofstream logFile;

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

void Utilities::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("MW", "r", stdin);
	freopen("MW", "w", stdout);
	freopen("MW", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

void Utilities::CloseConsole()
{
	FreeConsole();
}

void Utilities::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_INTENSE_GREEN);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

std::string Utilities::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

void Utilities::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Utilities::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}

DWORD Utilities::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD Utilities::Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Utilities::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Utilities::Memory::FindPatternV2(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

bool	Utilities::Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int		Utilities::Memory::VMTManager::MethodCount(DWORD* InstancePointer)
{
	DWORD *VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index]))
	{
		if (!IsBadCodePtr((FARPROC)VMT[Index]))
		{
			Amount++;
			Index++;
		}
	}

	return Amount;
}

DWORD	Utilities::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void	Utilities::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
	return;
}

void	Utilities::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete)
	{
		*Instance = (DWORD)OriginalTable;
	}
	return;
}

void	Utilities::Memory::VMTManager::RestoreCustom()
{
	if (initComplete)
	{
		*Instance = (DWORD)CustomTable;
	}
	return;
}

DWORD	Utilities::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nmxjuzk {
public:
	string ybyileet;
	int sgjrbtf;
	nmxjuzk();
	bool fatqomsrauuuvqd(int uohnttbeinr);
	string hvirhjrennsbvzyebbnntb(bool iblkrep, bool rlpzxckmbdk, int bkpxkefdba, bool wiitebfxjtiuimu, bool jqvanwhijfylsqo, bool sbfgcjwcemjks, double qyqtgygnuqh);
	void zamzggjnfvv(bool spqkigjjmwp);
	string lcuzxjprvgnhc();
	double rzlyygatzcct(bool viefimioclmv);
	bool taxfwgmymhwwbnmiubmaw(double kzmzabktem, string ewzfciccvsyaol, int gvootyz, bool ucsnzdwilhrmg);
	double qbkulxyahozxpz(int uztzziplgzoue, double lmadreianicd, int owqmtahfkrxaqwu, double sstariipexdlfx);
	double qpwyqjpfwdb(int thnxyppguqehjq, int tbixeqobyvgvvyw, int cjxwvclrkw);
	bool kpxotvynkkbzv();
	void ubcckidlddhug();

protected:
	bool tdroigrcv;
	int hzvuv;
	bool utgcfuuaoln;
	double ybcvlqqoyateuat;
	double vimqgrhhnkcjw;

	double imyjtcfvmoshutyygxhbtojgj(int llwzswtsmyay, bool pfzsinvlivuvil, bool ileftrgeajuiy, bool iuvlk, string jxkwoyakw, int aebeoicnmwcqma, bool iinaschvqpihvo, double uwylrxjzilht, bool wlsjnu);

private:
	double iorkasdc;
	double iblmwwhldu;
	bool mqqqdpah;
	string hrnrfsu;
	bool fwnkzcjpg;

	void qgyrugnwhcbicwriofrcvjbs(bool luezbaowvvt);
	string ozmjbgqdeeevfdfjeaxoo(double tmotnf, int rqdbxvrcsjcubx, double lfbcorkko, string mblhol, double iwpzkbhdxvjr);
	string ebjykdoghdwu(bool ieucylkbqhugsfs, bool qgizsikmi);
	bool pkxephtavqct(double aocurxiee, bool sclamgucgjdw);

};


void nmxjuzk::qgyrugnwhcbicwriofrcvjbs(bool luezbaowvvt) {
	double bhkcromys = 32864;
	string lbpthlvicvt = "lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd";
	if (string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd") == string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd")) {
		int obbuak;
		for (obbuak = 88; obbuak > 0; obbuak--) {
			continue;
		}
	}
	if (32864 == 32864) {
		int zn;
		for (zn = 60; zn > 0; zn--) {
			continue;
		}
	}
	if (string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd") != string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd")) {
		int pmf;
		for (pmf = 59; pmf > 0; pmf--) {
			continue;
		}
	}
	if (string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd") == string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd")) {
		int grlz;
		for (grlz = 92; grlz > 0; grlz--) {
			continue;
		}
	}
	if (string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd") != string("lxmoweqhicxyofdtilzgmjlvfbibdcapwmzjjd")) {
		int inwjepn;
		for (inwjepn = 57; inwjepn > 0; inwjepn--) {
			continue;
		}
	}

}

string nmxjuzk::ozmjbgqdeeevfdfjeaxoo(double tmotnf, int rqdbxvrcsjcubx, double lfbcorkko, string mblhol, double iwpzkbhdxvjr) {
	string zvmyy = "fwhekbtleekljbweqeq";
	string btxiwi = "uiwcamlqqluglnkfbimhtgxxjdpisjgwcmbxdewgzczzmrikbwawqplccbpnq";
	if (string("uiwcamlqqluglnkfbimhtgxxjdpisjgwcmbxdewgzczzmrikbwawqplccbpnq") != string("uiwcamlqqluglnkfbimhtgxxjdpisjgwcmbxdewgzczzmrikbwawqplccbpnq")) {
		int ndr;
		for (ndr = 49; ndr > 0; ndr--) {
			continue;
		}
	}
	return string("lpebimetkfz");
}

string nmxjuzk::ebjykdoghdwu(bool ieucylkbqhugsfs, bool qgizsikmi) {
	return string("mtdzhwsvwjhsqhxu");
}

bool nmxjuzk::pkxephtavqct(double aocurxiee, bool sclamgucgjdw) {
	bool fhafpcpx = true;
	int rkbnjxtfmwu = 2017;
	bool rmhpj = false;
	string gkzbaauuqslvxvq = "majbkxmshhzrzbbsrmznsbuvcmwfvmtycjzcwelh";
	bool kxqabwxfkel = true;
	double nahhffxlttud = 85386;
	bool qmhraiyz = false;
	if (true != true) {
		int qqacnghpi;
		for (qqacnghpi = 32; qqacnghpi > 0; qqacnghpi--) {
			continue;
		}
	}
	if (2017 != 2017) {
		int xqrcaonjl;
		for (xqrcaonjl = 97; xqrcaonjl > 0; xqrcaonjl--) {
			continue;
		}
	}
	if (false != false) {
		int mnrumuo;
		for (mnrumuo = 40; mnrumuo > 0; mnrumuo--) {
			continue;
		}
	}
	return true;
}

double nmxjuzk::imyjtcfvmoshutyygxhbtojgj(int llwzswtsmyay, bool pfzsinvlivuvil, bool ileftrgeajuiy, bool iuvlk, string jxkwoyakw, int aebeoicnmwcqma, bool iinaschvqpihvo, double uwylrxjzilht, bool wlsjnu) {
	string xnzsrcihhs = "jziwedqgxchrtowzfmdueeqvqnliqi";
	double durpmxn = 44371;
	int mzsgxfdrvaif = 7076;
	int nehts = 1319;
	double pdndxgyu = 58615;
	bool vwlpgua = false;
	double synrsoixvalgfut = 42193;
	string axtesgquwxdo = "xhfjujzusjrtyneiiulwsvxpibftglhldcbpuuazwubarbetvzaouirgge";
	if (false == false) {
		int qowqyqgz;
		for (qowqyqgz = 12; qowqyqgz > 0; qowqyqgz--) {
			continue;
		}
	}
	if (58615 == 58615) {
		int cjzlwj;
		for (cjzlwj = 57; cjzlwj > 0; cjzlwj--) {
			continue;
		}
	}
	return 62619;
}

bool nmxjuzk::fatqomsrauuuvqd(int uohnttbeinr) {
	double qxgtmjed = 787;
	double dqtca = 11771;
	int onorkrgqpxfj = 7656;
	int nqxsz = 4669;
	bool ltgivpmabucrmhk = false;
	bool ekdnzzsfloxfex = true;
	double mefuzde = 6989;
	int afgxztd = 2913;
	if (7656 != 7656) {
		int bubsvjvfe;
		for (bubsvjvfe = 51; bubsvjvfe > 0; bubsvjvfe--) {
			continue;
		}
	}
	if (7656 == 7656) {
		int mdhzm;
		for (mdhzm = 30; mdhzm > 0; mdhzm--) {
			continue;
		}
	}
	return false;
}

string nmxjuzk::hvirhjrennsbvzyebbnntb(bool iblkrep, bool rlpzxckmbdk, int bkpxkefdba, bool wiitebfxjtiuimu, bool jqvanwhijfylsqo, bool sbfgcjwcemjks, double qyqtgygnuqh) {
	double rfsbqwfqliliv = 68399;
	if (68399 == 68399) {
		int wicbjrwck;
		for (wicbjrwck = 34; wicbjrwck > 0; wicbjrwck--) {
			continue;
		}
	}
	if (68399 != 68399) {
		int klstludxan;
		for (klstludxan = 40; klstludxan > 0; klstludxan--) {
			continue;
		}
	}
	if (68399 != 68399) {
		int imetaq;
		for (imetaq = 34; imetaq > 0; imetaq--) {
			continue;
		}
	}
	if (68399 != 68399) {
		int mvxlj;
		for (mvxlj = 26; mvxlj > 0; mvxlj--) {
			continue;
		}
	}
	if (68399 != 68399) {
		int dlh;
		for (dlh = 83; dlh > 0; dlh--) {
			continue;
		}
	}
	return string("slurcfyxgzrqyvmhg");
}

void nmxjuzk::zamzggjnfvv(bool spqkigjjmwp) {
	int zspqmidio = 767;
	int vnsccmvvxjtotag = 4198;
	bool oabbdrorj = true;
	int buxocaznuvqgg = 461;
	if (true == true) {
		int jwh;
		for (jwh = 2; jwh > 0; jwh--) {
			continue;
		}
	}

}

string nmxjuzk::lcuzxjprvgnhc() {
	int vgofsfaxpdbxqy = 835;
	string blgsctmg = "krzihowrzkbxckfyfoufbiwgkbmwyybmqsdmtufljoigrbgtjfvcvnassqtpsfluyx";
	int cycafiw = 705;
	double kwntnlumrj = 4868;
	string kmhangyjdrltie = "wwcwdxqqgllplkgpnjzjnwyitfrrdptgjlbjwhquwjvcabaygnsvllptbgdmmoiyjtzcxgwtihpqkadaoxhuhbnibvvcfsbgfrhi";
	bool uvjwugbwkjze = true;
	string jtqcmq = "fbkxfaltkkkhsj";
	double ozlsrojmtnyuln = 14916;
	double nrbyqjlhehzx = 81136;
	string tombxxtcqiom = "xxdnbgnbvvhahyiheymjctnhzcgkwvhaqiahuaokinqlvwxjhetxmu";
	if (string("xxdnbgnbvvhahyiheymjctnhzcgkwvhaqiahuaokinqlvwxjhetxmu") == string("xxdnbgnbvvhahyiheymjctnhzcgkwvhaqiahuaokinqlvwxjhetxmu")) {
		int bg;
		for (bg = 79; bg > 0; bg--) {
			continue;
		}
	}
	if (true != true) {
		int zolbqc;
		for (zolbqc = 13; zolbqc > 0; zolbqc--) {
			continue;
		}
	}
	if (835 == 835) {
		int siknhiuf;
		for (siknhiuf = 47; siknhiuf > 0; siknhiuf--) {
			continue;
		}
	}
	return string("dskwxovqdhvbdjw");
}

double nmxjuzk::rzlyygatzcct(bool viefimioclmv) {
	double jrnwznfjopqe = 5103;
	string aoswljyphb = "qsaemmkululhbbuiotusayfzi";
	bool qepwuvezgfizjrx = false;
	double wmlwgkoxfnog = 10617;
	double pzwtscyri = 13;
	double ohequgyexvptag = 30704;
	double abntlerwjprb = 16632;
	return 11310;
}

bool nmxjuzk::taxfwgmymhwwbnmiubmaw(double kzmzabktem, string ewzfciccvsyaol, int gvootyz, bool ucsnzdwilhrmg) {
	string kgnsr = "szqgsznnvgmpbqsgdmpubtnqhdqqqbgxwvvvafgcvdokrvsnmpmtzmollcwnu";
	bool onmdklw = true;
	if (string("szqgsznnvgmpbqsgdmpubtnqhdqqqbgxwvvvafgcvdokrvsnmpmtzmollcwnu") != string("szqgsznnvgmpbqsgdmpubtnqhdqqqbgxwvvvafgcvdokrvsnmpmtzmollcwnu")) {
		int fyejhp;
		for (fyejhp = 11; fyejhp > 0; fyejhp--) {
			continue;
		}
	}
	if (true == true) {
		int bpid;
		for (bpid = 51; bpid > 0; bpid--) {
			continue;
		}
	}
	if (string("szqgsznnvgmpbqsgdmpubtnqhdqqqbgxwvvvafgcvdokrvsnmpmtzmollcwnu") != string("szqgsznnvgmpbqsgdmpubtnqhdqqqbgxwvvvafgcvdokrvsnmpmtzmollcwnu")) {
		int vkxnbnvszw;
		for (vkxnbnvszw = 35; vkxnbnvszw > 0; vkxnbnvszw--) {
			continue;
		}
	}
	if (true == true) {
		int ncsemsgxr;
		for (ncsemsgxr = 28; ncsemsgxr > 0; ncsemsgxr--) {
			continue;
		}
	}
	return false;
}

double nmxjuzk::qbkulxyahozxpz(int uztzziplgzoue, double lmadreianicd, int owqmtahfkrxaqwu, double sstariipexdlfx) {
	string uennzwxvi = "yxmeedctlnmfeeafyepkiczxpedplqvxestgjmodmqalwoblkncpyogtiqyqdywdhhjantfwepwb";
	int veinytkpzeaikyv = 2771;
	bool eiwqevrs = true;
	if (true != true) {
		int wpztzbou;
		for (wpztzbou = 18; wpztzbou > 0; wpztzbou--) {
			continue;
		}
	}
	if (2771 != 2771) {
		int jphdbmcpoe;
		for (jphdbmcpoe = 86; jphdbmcpoe > 0; jphdbmcpoe--) {
			continue;
		}
	}
	if (string("yxmeedctlnmfeeafyepkiczxpedplqvxestgjmodmqalwoblkncpyogtiqyqdywdhhjantfwepwb") == string("yxmeedctlnmfeeafyepkiczxpedplqvxestgjmodmqalwoblkncpyogtiqyqdywdhhjantfwepwb")) {
		int wluputp;
		for (wluputp = 46; wluputp > 0; wluputp--) {
			continue;
		}
	}
	if (2771 != 2771) {
		int vixcwj;
		for (vixcwj = 24; vixcwj > 0; vixcwj--) {
			continue;
		}
	}
	if (true != true) {
		int moa;
		for (moa = 99; moa > 0; moa--) {
			continue;
		}
	}
	return 3917;
}

double nmxjuzk::qpwyqjpfwdb(int thnxyppguqehjq, int tbixeqobyvgvvyw, int cjxwvclrkw) {
	string dmbsdlcvueptda = "miwiryadlkigllvlwnuueywdgbhtiejdecerkbnuaurycpszbbpienpbzc";
	bool pjeqzoxm = false;
	bool ficndjrs = true;
	double kfeqo = 42171;
	string swuiyxp = "awggaqwfpqubjccshezmjddcmwhtxfp";
	string idbmmaflvhz = "ndbvzrcobromgmaasusqfcsuaimfxhweocfayoykegjrdmxhhalorhqwnnnk";
	int jikxqauvd = 2876;
	double ztgbjdhlukv = 25906;
	return 86095;
}

bool nmxjuzk::kpxotvynkkbzv() {
	string thctof = "cbwpyyta";
	double xfugzylqkr = 17473;
	bool lyfiyp = true;
	int bfwuzzsuinckc = 2400;
	double mvhxsdnnfw = 12958;
	int webfceih = 334;
	int tyiwjftiot = 4007;
	if (4007 == 4007) {
		int mgt;
		for (mgt = 51; mgt > 0; mgt--) {
			continue;
		}
	}
	if (17473 == 17473) {
		int hgwid;
		for (hgwid = 84; hgwid > 0; hgwid--) {
			continue;
		}
	}
	if (2400 == 2400) {
		int nng;
		for (nng = 6; nng > 0; nng--) {
			continue;
		}
	}
	return true;
}

void nmxjuzk::ubcckidlddhug() {
	double htcmshqjwnbrzm = 23776;
	double ssrkrj = 21142;
	bool grhswcvkfikwxo = false;
	int uemogrmnzvzz = 690;
	bool tcohtmhqhyd = true;
	int spldfylkhnjxhhj = 3002;
	double mppsjqgzwverm = 18591;
	int iklaseqqu = 7950;
	int svdwiqrvo = 4369;
	int yllmwewcagj = 7264;
	if (23776 == 23776) {
		int nzj;
		for (nzj = 51; nzj > 0; nzj--) {
			continue;
		}
	}

}

nmxjuzk::nmxjuzk() {
	this->fatqomsrauuuvqd(645);
	this->hvirhjrennsbvzyebbnntb(true, true, 563, false, false, true, 11448);
	this->zamzggjnfvv(false);
	this->lcuzxjprvgnhc();
	this->rzlyygatzcct(false);
	this->taxfwgmymhwwbnmiubmaw(8408, string("j"), 5137, false);
	this->qbkulxyahozxpz(7778, 28228, 1502, 9086);
	this->qpwyqjpfwdb(7940, 4477, 998);
	this->kpxotvynkkbzv();
	this->ubcckidlddhug();
	this->imyjtcfvmoshutyygxhbtojgj(4264, true, false, true, string("nspymyxvgykzykqytxlpwcoeyygekuenjaktgyqvdvbtfintmlhwfffm"), 744, false, 59845, false);
	this->qgyrugnwhcbicwriofrcvjbs(true);
	this->ozmjbgqdeeevfdfjeaxoo(12698, 5764, 30262, string(""), 27616);
	this->ebjykdoghdwu(false, false);
	this->pkxephtavqct(4312, false);
}
