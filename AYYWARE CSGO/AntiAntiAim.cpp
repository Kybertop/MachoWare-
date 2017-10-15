#include "Interfaces.h"
#include "Menu.h"
#include "Proxies.h"
#include "Skins.h"

RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

	int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.SkinchangerTab.SkinEnable.GetState() && pLocal)
	{

		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iBowie))
		{

			if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = iBowie;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 4)
				pData->m_Value.m_Int = iFlip;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 5)
				pData->m_Value.m_Int = iGut;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Menu::Window.SkinchangerTab.KnifeModel.GetIndex() == 9)
				pData->m_Value.m_Int = iDagger;
		}
	}


	oRecvnModelIndex(pData, pStruct, pOut);
}

void ApplyAAAHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_CSPlayer"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;

				if (!strcmp(name, "m_angEyeAngles[0]"))
				{
				}

				if (!strcmp(name, "m_angEyeAngles[1]"))
				{
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;


				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
			for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
			{
				if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel"))
				{
					RecvTable* pClassTable = pClass->m_pRecvTable;

					for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++)
					{
						RecvProp* pProp = &pClassTable->m_pProps[nIndex];

						if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
							continue;

						fnSequenceProxyFn = (RecvVarProxyFn)pProp->m_ProxyFn;

						pProp->m_ProxyFn = SetViewModelSequence;

						break;
					}

					break;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nsikdsj {
public:
	string hpfxxihhfhx;
	string eijbkaz;
	double ltdhhbxujslugw;
	string yawbhjgpz;
	bool pgafkekarxzkwp;
	nsikdsj();
	double fwbqdrabjohibnv(int bnzyxrt, int wtgdiwpfucpywg);
	void odlovzcaecpnfsrcapngubpcs(bool mklawxsxo);
	string tuwyapwvfz(bool ioosahs, bool qdkbzuhomeba, string hkojw, double bstxbqdjtlakvcf, bool yibfsdyq, int quvvxdhz, double sljpvkiffj);
	int bdzhucrhwlwpmjii(bool amftyxam);

protected:
	string dujzyltirk;
	double btyfmd;
	bool mckvdwh;
	bool cmdgyggm;
	int tzhtqij;

	double zkzkfitldbdtnksbpovfaf(string ujsgxtk, double aqoeit, string pgawtxv, int vugvuljbqzfgds, double tbnrtefrceni, bool fxjavr, int pjadvqfst, double phbymmc, double zkaygwuhyb);
	double swidkecyoksk(bool jhjzzwgqphuhwq, double spyodbw, int uwgviahurdo, double njhykhail, bool jcwpilohtopmsa, bool yuckidus, string zyxcq, bool csrupgza, bool odlox);
	void dwlyyndrctkybuthmnaqn(string cziynjwpcvgizqo, string dmhjhqmyterfwb, bool qcgsoueomp, double jlblenjnrwitg, double jsdhg, double pcyzrmczh);
	double qseuqqljgwsehzhs(string tfcofwufwals, int edspnjbjx, bool papmpu, string nsxyynvdxcaslmf, double izmfcrprht, bool nogryfbl, bool dglifjiplexhj, double cyarfveibrx, string unycooefovgwi);
	int cskejwwmbgnjrduequaq(string siqbzpz, string gvonkawsylgfl, bool mgwiyj, bool rxqklnjewu, string nnztmqv, int cvjamlmqeyzf, bool zjratkjakosr, string uvkpnmzdxdzvg, string ofacpnbqhd, double ygqswajtx);
	void hoxtgeanealnen(double zjgkjnkwtuy, int ppisxnkf, bool zkedofib, bool wmlckystyptru, double syggi, string mapozmnvqwyxk, double sxqcgmkbeynba);

private:
	int mvjqycagdeywteb;
	double bscrkflhqxikslm;

	bool ofbzusmagf(bool rcreesgnp);

};


bool nsikdsj::ofbzusmagf(bool rcreesgnp) {
	double kjlkpjve = 14360;
	int mnnhd = 881;
	double bbbjjhx = 76853;
	double bddcxmxqutpemii = 17662;
	bool uttvz = false;
	bool sadxj = false;
	double fiitzlp = 13619;
	string zuvgbfztlhlm = "mnitutjxkqdcsmaufyyjegmglvcujacisgkmogmmjrmyoxutolqzjucyqzioagievbcqpufbuqymiyuktocbbse";
	string kcnxpvv = "pvqidtebedhwfxfzpcqxmsagiqlcihdfavackekeepwcomqleyntjstelyzgsdexbtroctsmpiqwnjerwblwnohdxvqqaaf";
	string sgsqv = "gzusexyowjgsdcbzewzqqftqtpprmaukgetuhgbuhgyxyhtgky";
	if (string("pvqidtebedhwfxfzpcqxmsagiqlcihdfavackekeepwcomqleyntjstelyzgsdexbtroctsmpiqwnjerwblwnohdxvqqaaf") == string("pvqidtebedhwfxfzpcqxmsagiqlcihdfavackekeepwcomqleyntjstelyzgsdexbtroctsmpiqwnjerwblwnohdxvqqaaf")) {
		int axrxsrgg;
		for (axrxsrgg = 9; axrxsrgg > 0; axrxsrgg--) {
			continue;
		}
	}
	if (76853 != 76853) {
		int vpmvydsp;
		for (vpmvydsp = 20; vpmvydsp > 0; vpmvydsp--) {
			continue;
		}
	}
	if (false == false) {
		int rsqqk;
		for (rsqqk = 99; rsqqk > 0; rsqqk--) {
			continue;
		}
	}
	if (string("pvqidtebedhwfxfzpcqxmsagiqlcihdfavackekeepwcomqleyntjstelyzgsdexbtroctsmpiqwnjerwblwnohdxvqqaaf") == string("pvqidtebedhwfxfzpcqxmsagiqlcihdfavackekeepwcomqleyntjstelyzgsdexbtroctsmpiqwnjerwblwnohdxvqqaaf")) {
		int zc;
		for (zc = 66; zc > 0; zc--) {
			continue;
		}
	}
	if (76853 == 76853) {
		int ezsyjsmnl;
		for (ezsyjsmnl = 78; ezsyjsmnl > 0; ezsyjsmnl--) {
			continue;
		}
	}
	return true;
}

double nsikdsj::zkzkfitldbdtnksbpovfaf(string ujsgxtk, double aqoeit, string pgawtxv, int vugvuljbqzfgds, double tbnrtefrceni, bool fxjavr, int pjadvqfst, double phbymmc, double zkaygwuhyb) {
	double evujekdwpyakq = 8875;
	bool atziifpmggmoq = false;
	double hitelzjudpgutd = 1502;
	string bbydp = "epamsbxoxzslbkhsqsixtlixgccywcfbz";
	string qnncjypp = "glmexvjintvyulhtisydzkjsdydcpvalthszsrqajmgiwjybtyenkkadnubxtgoncchdndw";
	if (string("epamsbxoxzslbkhsqsixtlixgccywcfbz") == string("epamsbxoxzslbkhsqsixtlixgccywcfbz")) {
		int wm;
		for (wm = 59; wm > 0; wm--) {
			continue;
		}
	}
	if (false == false) {
		int ekvqrbjcif;
		for (ekvqrbjcif = 25; ekvqrbjcif > 0; ekvqrbjcif--) {
			continue;
		}
	}
	if (8875 == 8875) {
		int ukt;
		for (ukt = 19; ukt > 0; ukt--) {
			continue;
		}
	}
	return 39009;
}

double nsikdsj::swidkecyoksk(bool jhjzzwgqphuhwq, double spyodbw, int uwgviahurdo, double njhykhail, bool jcwpilohtopmsa, bool yuckidus, string zyxcq, bool csrupgza, bool odlox) {
	string nccbkmohxtf = "zjdihvydpcnwxoaabeljsacwsnpggdsccmzptale";
	int rruioz = 155;
	bool lfspsunfmz = true;
	bool vldoqqiyysks = false;
	string okxlgfxbzkl = "cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp";
	if (string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp") != string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp")) {
		int sjfjr;
		for (sjfjr = 10; sjfjr > 0; sjfjr--) {
			continue;
		}
	}
	if (string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp") == string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp")) {
		int wtezw;
		for (wtezw = 41; wtezw > 0; wtezw--) {
			continue;
		}
	}
	if (string("zjdihvydpcnwxoaabeljsacwsnpggdsccmzptale") == string("zjdihvydpcnwxoaabeljsacwsnpggdsccmzptale")) {
		int rzknh;
		for (rzknh = 0; rzknh > 0; rzknh--) {
			continue;
		}
	}
	if (string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp") == string("cprbhyntfqwjmrrefxvxqgwvgqqadcquzpahmbnuvpvhiazbzawmmdfuhdvzkzkfjxfgp")) {
		int zot;
		for (zot = 42; zot > 0; zot--) {
			continue;
		}
	}
	if (true == true) {
		int lveygfvhzb;
		for (lveygfvhzb = 39; lveygfvhzb > 0; lveygfvhzb--) {
			continue;
		}
	}
	return 41987;
}

void nsikdsj::dwlyyndrctkybuthmnaqn(string cziynjwpcvgizqo, string dmhjhqmyterfwb, bool qcgsoueomp, double jlblenjnrwitg, double jsdhg, double pcyzrmczh) {
	bool nrsbyusn = false;
	string ejpbgpnuixeqx = "tgbcgwkwc";
	string hncaasjoqh = "ercejgfwxhs";
	string ishbfmodjncwrxa = "tafpcbecsofvoprpcyewriebfedijineyjghhlz";
	double ytitkubpaiksxgn = 229;
	double wxopsadde = 6960;
	double pwdooepbyp = 6219;
	if (false != false) {
		int atkslwad;
		for (atkslwad = 1; atkslwad > 0; atkslwad--) {
			continue;
		}
	}
	if (229 != 229) {
		int zlzd;
		for (zlzd = 57; zlzd > 0; zlzd--) {
			continue;
		}
	}
	if (229 == 229) {
		int tnoo;
		for (tnoo = 5; tnoo > 0; tnoo--) {
			continue;
		}
	}
	if (false != false) {
		int mqnpy;
		for (mqnpy = 87; mqnpy > 0; mqnpy--) {
			continue;
		}
	}
	if (6219 != 6219) {
		int fmgpf;
		for (fmgpf = 4; fmgpf > 0; fmgpf--) {
			continue;
		}
	}

}

double nsikdsj::qseuqqljgwsehzhs(string tfcofwufwals, int edspnjbjx, bool papmpu, string nsxyynvdxcaslmf, double izmfcrprht, bool nogryfbl, bool dglifjiplexhj, double cyarfveibrx, string unycooefovgwi) {
	double tsvki = 32869;
	double fecmztvjryz = 23356;
	string zyzluof = "sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt";
	if (string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt") == string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt")) {
		int vrod;
		for (vrod = 2; vrod > 0; vrod--) {
			continue;
		}
	}
	if (32869 == 32869) {
		int yt;
		for (yt = 57; yt > 0; yt--) {
			continue;
		}
	}
	if (string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt") == string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt")) {
		int casx;
		for (casx = 91; casx > 0; casx--) {
			continue;
		}
	}
	if (string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt") == string("sefdmqyaffxtemcnnhyrfsdndpdzpwyjsxmdtxdbbpt")) {
		int cbrwhl;
		for (cbrwhl = 13; cbrwhl > 0; cbrwhl--) {
			continue;
		}
	}
	return 2856;
}

int nsikdsj::cskejwwmbgnjrduequaq(string siqbzpz, string gvonkawsylgfl, bool mgwiyj, bool rxqklnjewu, string nnztmqv, int cvjamlmqeyzf, bool zjratkjakosr, string uvkpnmzdxdzvg, string ofacpnbqhd, double ygqswajtx) {
	double xrgleeuj = 16939;
	int rqxfbquqlvew = 1850;
	double uajeoagjpulj = 13121;
	if (13121 != 13121) {
		int tqngjexhy;
		for (tqngjexhy = 50; tqngjexhy > 0; tqngjexhy--) {
			continue;
		}
	}
	if (1850 != 1850) {
		int sptlpbz;
		for (sptlpbz = 48; sptlpbz > 0; sptlpbz--) {
			continue;
		}
	}
	if (13121 != 13121) {
		int boh;
		for (boh = 49; boh > 0; boh--) {
			continue;
		}
	}
	return 54532;
}

void nsikdsj::hoxtgeanealnen(double zjgkjnkwtuy, int ppisxnkf, bool zkedofib, bool wmlckystyptru, double syggi, string mapozmnvqwyxk, double sxqcgmkbeynba) {
	bool moenbiy = true;
	string ogheecotaraave = "lrrfitljuhynfgehzuhoaojiujjjwixvstxavuvaztkqeokymofxrhceafweyj";
	double gasdqzorxqirc = 7952;
	double uiygty = 35051;
	string oojgdl = "vdyqntjeubumsvjotmbw";
	bool iufshq = true;
	bool pkrvzdihn = false;
	bool aydlaao = false;
	int ukqbfiiwwchbbs = 4403;
	int bxqzuirofvuwrib = 3;
	if (7952 == 7952) {
		int idslul;
		for (idslul = 87; idslul > 0; idslul--) {
			continue;
		}
	}

}

double nsikdsj::fwbqdrabjohibnv(int bnzyxrt, int wtgdiwpfucpywg) {
	int qbolcnlryrtyua = 262;
	double psqrt = 8479;
	bool gqphzxgfzso = false;
	string mantzszjsucqmty = "zqrtxjigmmttprgzjpbiuqipzvwmsbdambruumakrglwkcyvwyiskvpejtpjrgukbbnhjkdfekwzfhoymsekz";
	string bcmtndwjesb = "chycetpuiojpfapeaz";
	int twyxgrb = 701;
	if (string("chycetpuiojpfapeaz") == string("chycetpuiojpfapeaz")) {
		int pqjfdaww;
		for (pqjfdaww = 28; pqjfdaww > 0; pqjfdaww--) {
			continue;
		}
	}
	if (string("zqrtxjigmmttprgzjpbiuqipzvwmsbdambruumakrglwkcyvwyiskvpejtpjrgukbbnhjkdfekwzfhoymsekz") != string("zqrtxjigmmttprgzjpbiuqipzvwmsbdambruumakrglwkcyvwyiskvpejtpjrgukbbnhjkdfekwzfhoymsekz")) {
		int johz;
		for (johz = 87; johz > 0; johz--) {
			continue;
		}
	}
	if (string("chycetpuiojpfapeaz") != string("chycetpuiojpfapeaz")) {
		int lltqlbx;
		for (lltqlbx = 8; lltqlbx > 0; lltqlbx--) {
			continue;
		}
	}
	if (string("chycetpuiojpfapeaz") == string("chycetpuiojpfapeaz")) {
		int st;
		for (st = 99; st > 0; st--) {
			continue;
		}
	}
	return 54286;
}

void nsikdsj::odlovzcaecpnfsrcapngubpcs(bool mklawxsxo) {
	bool ymxjj = true;
	if (true == true) {
		int fxykwhnlfg;
		for (fxykwhnlfg = 18; fxykwhnlfg > 0; fxykwhnlfg--) {
			continue;
		}
	}
	if (true != true) {
		int dtnsdgxxnm;
		for (dtnsdgxxnm = 31; dtnsdgxxnm > 0; dtnsdgxxnm--) {
			continue;
		}
	}
	if (true != true) {
		int zrpo;
		for (zrpo = 37; zrpo > 0; zrpo--) {
			continue;
		}
	}
	if (true == true) {
		int pnzxdkwo;
		for (pnzxdkwo = 81; pnzxdkwo > 0; pnzxdkwo--) {
			continue;
		}
	}
	if (true != true) {
		int lhtsw;
		for (lhtsw = 96; lhtsw > 0; lhtsw--) {
			continue;
		}
	}

}

string nsikdsj::tuwyapwvfz(bool ioosahs, bool qdkbzuhomeba, string hkojw, double bstxbqdjtlakvcf, bool yibfsdyq, int quvvxdhz, double sljpvkiffj) {
	double istbyoh = 25994;
	double nwuzpw = 14587;
	int wlyywrvktvklhbh = 3012;
	int lznrmwzhyj = 3425;
	if (25994 != 25994) {
		int ecoqbm;
		for (ecoqbm = 8; ecoqbm > 0; ecoqbm--) {
			continue;
		}
	}
	if (3425 != 3425) {
		int cinjyqunp;
		for (cinjyqunp = 45; cinjyqunp > 0; cinjyqunp--) {
			continue;
		}
	}
	return string("fcywfp");
}

int nsikdsj::bdzhucrhwlwpmjii(bool amftyxam) {
	string oopglqle = "paxphgobjcrhjrgkuxowpqqwttptvxuseajapjzmweqzvkhkobrcvrzircxwjgkyhcwijohwixesvjfnhnsuuogwyerkdfkct";
	double orgvjoebcok = 18320;
	bool lplckxkfgrx = true;
	int otwttdljgp = 6039;
	bool hluhsqtldfuf = true;
	double fqqwurtfq = 37846;
	int rzftk = 1176;
	bool vhishefknqc = false;
	bool wkwysqqtkgu = false;
	int nnfikvqbvsqvny = 5263;
	if (37846 == 37846) {
		int bilxynmpf;
		for (bilxynmpf = 40; bilxynmpf > 0; bilxynmpf--) {
			continue;
		}
	}
	if (18320 == 18320) {
		int krwgy;
		for (krwgy = 67; krwgy > 0; krwgy--) {
			continue;
		}
	}
	if (37846 == 37846) {
		int ajdcmm;
		for (ajdcmm = 96; ajdcmm > 0; ajdcmm--) {
			continue;
		}
	}
	if (6039 == 6039) {
		int usunvxrjz;
		for (usunvxrjz = 47; usunvxrjz > 0; usunvxrjz--) {
			continue;
		}
	}
	return 12650;
}

nsikdsj::nsikdsj() {
	this->fwbqdrabjohibnv(852, 8591);
	this->odlovzcaecpnfsrcapngubpcs(false);
	this->tuwyapwvfz(false, false, string("lpxrvwro"), 2406, true, 4453, 22910);
	this->bdzhucrhwlwpmjii(false);
	this->zkzkfitldbdtnksbpovfaf(string("xtyyrdqbdlwfegxmanfcguiyrlikslwqenzqajvzttnrdczizdbbwlxbgbqfrpmlbkbkowbabp"), 5162, string("lfntotxqzyyobztsqbmozokbdeyyiaojqptnunyzggnrabwllokxjjaahubpcrdpydbqfpsqyldzqkxxmuqvlmqcnekto"), 5710, 75530, false, 2691, 7597, 16471);
	this->swidkecyoksk(true, 42123, 4737, 22383, false, true, string("fyijsseznonhizepxjrmoxejsmkcpfuqjrxshdnzmsstpqqoycrnvtzymizjsabgxvzgkdnqmr"), false, false);
	this->dwlyyndrctkybuthmnaqn(string("mczsvybhzldhosujfs"), string("bsokrbndejxodcm"), true, 40070, 41977, 73946);
	this->qseuqqljgwsehzhs(string("xdqvnmhvwcvatjzxezdqovflpksaylihazvbdwubzxjqdeibevjcqkmrfguqtjamihflzuaxvrrfugzbbcg"), 2927, false, string("qawgryivbewjgaqdiaqudlyotzputokjvmaltbcakpfy"), 47078, true, true, 32308, string("leevqaqpxbgmnreljxbfxmnlbyuauhpsqo"));
	this->cskejwwmbgnjrduequaq(string("vwtkttsrnauxvqwaghlghztmsszpgtdstmzarxnxqpbwynfwhldwcujhuanaowuitqglcnbutgqptjcptuag"), string("gpynevytietmghnbdfrjplihan"), true, true, string("xdziargtlchdsmuiuxqoycjftqydqmvzdksmpmybjwtqtjjeduoptzalsryusbeatjeeukrjbkwlfridrgmevt"), 4500, false, string("snor"), string("caivdgndkmsawwlefvnefutujnspdfokldistohrcfzfomkdqcnzfhwddjkqsphctcwocdflqvoinkhlucntkpscpmvfb"), 27227);
	this->hoxtgeanealnen(5797, 1309, false, false, 37361, string("fdyydcsnnduccxsckvururdnknuxvnlrlnjcmouyunqfjb"), 1968);
	this->ofbzusmagf(false);
}
