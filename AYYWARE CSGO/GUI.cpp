#include "GUI.h"
#include "Menu.h"
#include "RenderManager.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"

float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float Globals::MenuAlpha22 = MenuAlpha21;
float Globals::MenuAlpha20 = MenuAlpha19;
float Globals::MenuAlpha18 = MenuAlpha17;
float Globals::MenuAlpha16 = MenuAlpha15;
float Globals::MenuAlpha14 = MenuAlpha13;
float Globals::MenuAlpha12 = MenuAlpha11;
float Globals::MenuAlpha10 = MenuAlpha9;
float Globals::MenuAlpha8 = MenuAlpha7;
float Globals::MenuAlpha6 = MenuAlpha5;
float Globals::MenuAlpha2 = MenuAlpha;
float Globals::MenuAlpha4 = MenuAlpha3;

CGUI GUI;
bool SaveFile = false;
bool LoadFile = false;

CGUI::CGUI()
{

}

#define UI_CURSORSIZE       15
#define UI_CURSORFILL       Color(255,255,MenuAlpha)
#define UI_CURSOROUTLINE    Color(255,0,0,MenuAlpha13)
#define UI_WIN_TOPHEIGHT	26
#define UI_WIN_TITLEHEIGHT	0
#define UI_TAB_WIDTH		150
#define UI_TAB_HEIGHT		32
#define UI_WIN_CLOSE_X		20
#define UI_WIN_CLOSE_Y      6
#define UI_CHK_SIZE			16
#define UI_COL_MAIN			Color(220, 27, 27, MenuAlpha11)
#define UI_COL_SHADOW		Color(0, 0, 0, MenuAlpha)
#define COL_WHITE           Color(255, 100, 50, MenuAlpha17)
#define UI_COL_MAINE        Color(204, 0, 0, MenuAlpha)
#define UI_COL_MAINDARK     Color(236, 42, 64, MenuAlpha)
#define UI_COURSOUR			Color(255, 255, 255, MenuAlpha)
#define UI_COL_FADEMAIN     Color(206, 4, 29, MenuAlpha15)
#define UI_COL_SHADOW		Color(0, 0, 0, MenuAlpha)
#define UI_COL_CLIENTBACK   Color(238, 0, 50, MenuAlpha)
#define UI_COL_TABSEPERATOR Color(229, 229, 229, MenuAlpha7)
#define UI_COL_TABTEXT      Color(255, 255, 255, MenuAlpha)
#define UI_COL_GROUPOUTLINE Color(222, 10, 10, MenuAlpha)

void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
			MenuAlpha = min(MenuAlpha + 6, 255);
		else
			MenuAlpha = max(MenuAlpha - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha5 = min(MenuAlpha5 + 6, 120);
		else
			MenuAlpha5 = max(MenuAlpha5 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha3 = min(MenuAlpha3 + 6, 15);
		else
			MenuAlpha3 = max(MenuAlpha3 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha7 = min(MenuAlpha7 + 6, 200);
		else
			MenuAlpha7 = max(MenuAlpha7 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha9 = min(MenuAlpha9 + 6, 245);
		else
			MenuAlpha9 = max(MenuAlpha9 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha11 = min(MenuAlpha11 + 6, 80);
		else
			MenuAlpha11 = max(MenuAlpha11 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha13 = min(MenuAlpha13 + 6, 140);
		else
			MenuAlpha13 = max(MenuAlpha13 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha15 = min(MenuAlpha15 + 6, 40);
		else
			MenuAlpha15 = max(MenuAlpha15 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha17 = min(MenuAlpha17 + 6, 50);
		else
			MenuAlpha17 = max(MenuAlpha17 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha21 = min(MenuAlpha21 + 6, 1);
		else
			MenuAlpha21 = max(MenuAlpha21 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha19 = min(MenuAlpha19 + 6, 100);
		else
			MenuAlpha19 = max(MenuAlpha19 - 6, 0);
		{
			ShouldDrawCursor = true;
			DrawWindow(window);
		}
	}
	if (ShouldDrawCursor)
	{
		Render::Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(3, 6, 26, MenuAlpha));
		for (int i = 0; i < 11; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(100, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(100, 6, 26, MenuAlpha));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(37 - (i * 4), 137 - (i * 4), 255 - (i * 4), MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, UI_COURSOUR);
		Render::Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, UI_COURSOUR);
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(37 - (i * 4), 137 - (i * 4), 255 - (i * 4), MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, UI_COURSOUR);
		Render::Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, UI_COURSOUR);
	}
}

void CGUI::Update()
{
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++) {
		keys[x] = static_cast<bool>(GetAsyncKeyState(x));
	}

	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Render::GetViewport();

	for (auto bind : WindowBinds)
	{
		if (GetKeyPress(bind.first) && bind.second != nullptr)
		{
			bind.second->Toggle();
		}
	}

	if (IsDragging && !GetKeyState(VK_LBUTTON))
	{
		IsDragging = false;
		DraggingWindow = nullptr;
	}

	if (IsDragging && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			bool bCheckWidgetClicks = false;

			if (GetKeyPress(VK_LBUTTON))
			{
				bCheckWidgetClicks = true;

				if (IsMouseInRegion(window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight, window->m_x))
				{
					if (IsMouseInRegion(window->m_y, window->m_x + window->m_iWidth, window->m_y + UI_WIN_CLOSE_X, window->m_x + window->m_iWidth - UI_WIN_CLOSE_X))
					{
						window->Toggle();
					}
					else
						if (IsMouseInRegion(window->GetTabArea()))
						{

							bCheckWidgetClicks = true;

							int iTab = 0;
							int TabCount = window->Tabs.size();
							if (TabCount)
							{
								int TabCount = window->Tabs.size();
								int TabSize = (window->m_iWidth - UI_TAB_WIDTH - UI_WIN_TITLEHEIGHT - 214) / TabCount;;
								int Dist = Mouse.y - (window->m_y + UI_WIN_TITLEHEIGHT + UI_WIN_TOPHEIGHT);
								if (Dist < (TabSize*TabCount))
								{
									while (Dist > TabSize)
									{
										if (Dist > TabSize)
										{
											iTab++;
											Dist -= TabSize;
										}
										if (iTab == (TabCount - 1))
										{
											break;
										}
									}
									window->SelectedTab = window->Tabs[iTab];

									bCheckWidgetClicks = false;
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}

						}
						else if (IsMouseInRegion(window->GetClientArea()))
						{
							bCheckWidgetClicks = true;
						}
						else
						{
							IsDragging = true;
							DraggingWindow = window;
							DragOffsetX = Mouse.x - window->m_x;
							DragOffsetY = Mouse.y - window->m_y;

							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
						}
				}
				else
				{
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}

			if (window->SelectedTab != nullptr)
			{
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						SkipWidget = true;
						SkipMe = window->FocusedControl;

						POINT cAbs = window->FocusedControl->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
						window->FocusedControl->OnUpdate();

						if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							window->FocusedControl->OnClick();

							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
							bCheckWidgetClicks = false;
						}
					}
				}

				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						if (SkipWidget && SkipMe == control)
							continue;

						control->parent = window;

						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						control->OnUpdate();

						if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							control->OnClick();
							bCheckWidgetClicks = false;

							if (control->Flag(UIFlags::UI_Focusable))
							{
								window->IsFocusingControl = true;
								window->FocusedControl = control;
							}
							else
							{
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
							}

						}
					}
				}

				if (bCheckWidgetClicks)
				{
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}

bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int y, int x2, int y2, int x)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.top, region.left + region.right, region.top + region.bottom, region.left);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

bool CGUI::DrawWindow(CWindow* window)
{
	RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, window->Title.c_str());
	int TextX = window->m_x + (window->m_iWidth / 2) - (TextSize.left / 2);
	Render::Clear(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color(25, 25, 25, MenuAlpha));
	Render::Clear(window->m_x, window->m_y + 2 + 20, window->m_iWidth, 4, UI_COL_MAIN);
	Render::Text(TextX - 26, window->m_y + 1, Color(255, 255, 255, MenuAlpha19), Render::Fonts::MenuBold, "");
	Render::Text(TextX + 26, window->m_y + 1, UI_COL_MAIN, Render::Fonts::MenuBold, "                               SteamXYZ");

	// Tabs
	int TabCount = window->Tabs.size();
	int TabSize = (window->m_iWidth - UI_TAB_WIDTH - UI_WIN_TITLEHEIGHT - 214) / TabCount;
	if (TabCount)
	{
		for (int i = 0; i < TabCount; i++)
		{
			RECT TabArea = { window->m_x, window->m_y + UI_WIN_TOPHEIGHT + UI_WIN_TITLEHEIGHT + (i*TabSize) , window->m_iWidth - window->m_iHeight - 200, TabSize };
			CTab *tab = window->Tabs[i];

			Render::Clear(TabArea.left, TabArea.top, UI_TAB_WIDTH, 1, Color(0, 0, 0, MenuAlpha));
			Render::Clear(TabArea.left, TabArea.top + 1, UI_TAB_WIDTH, 1, Color(48, 48, 48, MenuAlpha));
			Color txtColor = UI_COL_TABSEPERATOR;

			if (window->SelectedTab == tab)
			{
				Render::Clear(window->m_x, window->m_y + UI_WIN_TOPHEIGHT + UI_WIN_TITLEHEIGHT + (i*TabSize) + 2, UI_TAB_WIDTH, TabSize - 2, Color(20, 20, 20, MenuAlpha));
				txtColor = UI_COL_MAIN;
			}
			else if (IsMouseInRegion(TabArea))
			{
				// Hover
				txtColor = Color(90, 90, 90, MenuAlpha7);
			}
			Render::Text(TabArea.left + 50 - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT + 30, TabArea.top + 50 - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT + 5, txtColor, Render::Fonts::Tabs, tab->Title.c_str());
		}
	}
	//Window Outline
	Render::Outline(window->m_x - 1, window->m_y - 1, window->m_iWidth + 2, window->m_iHeight + 2, UI_COL_SHADOW);
	Render::Outline(window->m_x - 2, window->m_y - 2, window->m_iWidth + 4, window->m_iHeight + 4, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x - 3, window->m_y - 3, window->m_iWidth + 6, window->m_iHeight + 6, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 4, window->m_y - 4, window->m_iWidth + 8, window->m_iHeight + 8, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 5, window->m_y - 5, window->m_iWidth + 10, window->m_iHeight + 10, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 6, window->m_y - 6, window->m_iWidth + 12, window->m_iHeight + 12, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x - 7, window->m_y - 7, window->m_iWidth + 14, window->m_iHeight + 14, Color(31, 31, 31, MenuAlpha));

	if (window->SelectedTab != nullptr)
	{
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				SkipWidget = true;
				SkipMe = window->FocusedControl;
			}
		}

		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

	}

	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;

	tinyxml2::XMLElement *Root = Doc.NewElement("SteamXYZ");
	Doc.LinkEndChild(Root);

	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			if (TabElement && Tab->Controls.size() > 1)
			{
				for (auto Control : Tab->Controls)
				{
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;

						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						}
					}
				}
			}
		}
	}

	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "SteamXYZ", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		if (Root)
		{
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;

										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class flfolis {
public:
	double pjwbvjmbp;
	int zepei;
	flfolis();
	string qwvhwfbwldavnkzfidy(double mxnmv, int ukgqxri, string ltgmhmo, string berlggdhclgrzwe, int fcxtwh, int kawzqrpwkim, bool gtpduxfi, string zwshtkpnwstued);
	double cdjkcohafojvvvljazc(bool rwzoxjgp, double xjladv, int gdcyyzksufhur, string gyfxc, string tssfe, bool aeqszi);
	int tzsdnfdwrptqtexhnmj(double cmcgkluso, int oopbzcdz, double rdijktf);
	double fvverdqgevluknbtszwn(string tthpukwaafkfpj, bool yutxfprlnthgtbe, int ntajrgwijwcf, int yobmj, string wqnkvdfsyf, int hkjsxzszfnbdj, int vvifgwu, bool etpgbqdx);
	string wkinpdaecni(double vmrrsriiaiabq);
	double acsxacxpwgoptkafaoutxxs(bool jfknpvfthkea, bool dvzzdp, double lowcspnjuvwh, double csehakdvxa, int tzuyuvqhxqhudrh, int vwkrvotqyyoxrh, int xmjru);

protected:
	string voguohcnadz;
	bool vjbrfoaiozc;
	string fnkzuzrmohjuelr;

	double hnegsazsvbbjw(bool yvahombax, string eqltyjsofa, bool gxpovfjyybarwj);
	double kxtiqwuckbekbjo(int bbvurd, int nehvtthedel);
	double hlmcsxdhlvmnsdkxjsbf();

private:
	double coztddqvo;
	string xnyqssuhhdpxra;
	double jntnzgcjmioiaou;

	double bdmexnvvskzefrxcrqxgkre(bool rwfdvsyxj, int sohsdmfalws, string ytdkjzez, double pjopoitkc, bool aacwsu, string gvynnjxigssfzm, bool wuplznoqjn, double vdxhqfvdpih);
	bool bgrhnpqqezzlgwjzl();
	void rgmcxlbnfkgloztuqqyiyja(string watwn, string wqkysgxk, string vjlafrtu, bool emlylpsbeshvfm, double mbmsggerngr, double bxbjhtqn, double pmauwbrvcdcfbbz, bool wxlkhr, int nidoipnrhy, double unophuqzrbge);
	void fofnrjbrlaiwelmmnieqqygnk(double bhhonljvwr, double jqnnux);
	void srjmwgmnuysuvdj(double uumzuoijszptoh, bool bseetnjbl, bool znbksfqpouhh, bool zscntx, int wcyxwhkjqhwlgh, double iacbfuxxyqtrub);
	int ytuhgowskyjjhyezutjq(bool odafq);
	bool kryeputjhwpu();
	void bncaccvgjqrkoqbrgqm(double diprjnysdupo, int jeccfqzl, int coafmfhv, bool xsflacasm, bool pqevpgblg, int fhgjbhjnpmm, bool kyjpogikd, int akdjzlggcewh, bool pdtap);
	string wiyegcktgvgittjuhnvrp(bool jylnoyhakgvmji, bool gdnutrqwwwnod, double fzupcec);
	double lezyrqxxvsjftfqmeeegivdp(double buchrc);

};


double flfolis::bdmexnvvskzefrxcrqxgkre(bool rwfdvsyxj, int sohsdmfalws, string ytdkjzez, double pjopoitkc, bool aacwsu, string gvynnjxigssfzm, bool wuplznoqjn, double vdxhqfvdpih) {
	string abpogbg = "mqpaxenbbu";
	int ipcsg = 175;
	int uhlulslsywkabff = 263;
	double duxzshstwjkbz = 23098;
	bool zehhljtrbnwkd = true;
	if (true != true) {
		int znepvx;
		for (znepvx = 51; znepvx > 0; znepvx--) {
			continue;
		}
	}
	return 22669;
}

bool flfolis::bgrhnpqqezzlgwjzl() {
	bool sjeilcpk = true;
	double eaqjiohowami = 15700;
	string wkpirlku = "hpjqokguijaxoiiytyqggkrjmdteivemwtzwzoknovownxjpjntynbjyjhyujotpgzzdghhkfzmonuqcmxmzfkss";
	string ncnemzesovq = "ablngzrywyievckouvlaaocztbgtbwminohdeqbdazbk";
	string mdcdyoznrgtcgzz = "xicfewhuvbzqstnivnuhynawzhkvgodqhwativzrqeycebfjgnsrjekysylghkvbgzfdrvevlxwumsknzchptckolnzhk";
	string kfbezuui = "uiwcrrlqokojbuagazrzdusbourlamyzfdrz";
	string lgxboesyxizb = "kbasnpydfzzzgvxslwaikjdckboeacdnmdrzdnahaqrtdu";
	if (true == true) {
		int ov;
		for (ov = 82; ov > 0; ov--) {
			continue;
		}
	}
	if (true == true) {
		int zdsqdasvbu;
		for (zdsqdasvbu = 98; zdsqdasvbu > 0; zdsqdasvbu--) {
			continue;
		}
	}
	if (string("ablngzrywyievckouvlaaocztbgtbwminohdeqbdazbk") != string("ablngzrywyievckouvlaaocztbgtbwminohdeqbdazbk")) {
		int wdxsr;
		for (wdxsr = 78; wdxsr > 0; wdxsr--) {
			continue;
		}
	}
	if (string("hpjqokguijaxoiiytyqggkrjmdteivetzwzoknovownxjpjntynbjyjhyujotpgzzdghhkfzmonuqcmxmzfkss") != string("hpjqokguijaxoiiytyqggkrjmdteivetzwzoknovownxjpjntynbjyjhyujotpgzzdghhkfzmonuqcmxmzfkss")) {
		int xuhtop;
		for (xuhtop = 58; xuhtop > 0; xuhtop--) {
			continue;
		}
	}
	return true;
}

void flfolis::rgmcxlbnfkgloztuqqyiyja(string watwn, string wqkysgxk, string vjlafrtu, bool emlylpsbeshvfm, double mbmsggerngr, double bxbjhtqn, double pmauwbrvcdcfbbz, bool wxlkhr, int nidoipnrhy, double unophuqzrbge) {
	int auzrt = 5423;
	string igpgvunrq = "fxgfrnjqatcexmsluzzlygubxbxzlsebdodpoeenmstkjznvmjttsh";
	int ofujfdze = 2587;
	if (string("fxgfrnjqatcexmsluzzlygubxbxzlsebdodpoeenmstkjznvmjttsh") != string("fxgfrnjqatcexmsluzzlygubxbxzlsebdodpoeenmstkjznvmjttsh")) {
		int apr;
		for (apr = 10; apr > 0; apr--) {
			continue;
		}
	}
	if (2587 != 2587) {
		int obcypqos;
		for (obcypqos = 18; obcypqos > 0; obcypqos--) {
			continue;
		}
	}
	if (2587 != 2587) {
		int rjicmhdgsl;
		for (rjicmhdgsl = 64; rjicmhdgsl > 0; rjicmhdgsl--) {
			continue;
		}
	}
	if (string("fxgfrnjqatcexmsluzzlygubxbxzlsebdodpoeenmstkjznvmjttsh") != string("fxgfrnjqatcexmsluzzlygubxbxzlsebdodpoeenmstkjznvmjttsh")) {
		int tgzotrls;
		for (tgzotrls = 85; tgzotrls > 0; tgzotrls--) {
			continue;
		}
	}

}

void flfolis::fofnrjbrlaiwelmmnieqqygnk(double bhhonljvwr, double jqnnux) {
	bool mntuiejwie = true;
	int hvxiyfhrvxiktey = 509;
	string hcklbbgtegk = "vuby";
	string tdtflg = "koycmwrvardbsopdjdcolajgthsmuvrdiiklaspsnaphelqgbzgdbrt";
	int xldxcwhichdj = 294;
	int bennklhuzl = 1114;
	double egpgwowudhelucr = 47416;
	string nvshhppnf = "eglnxblcghhq";
	string yfkbhidc = "nklkbeedbwlheckaseuhepcylxftvlacvxexnsklkdjwommepyymhvpawaadokdzzixxhrrxufzwrndvoosmdqv";
	if (string("nklkbeedbwlheckaseuhepcylxftvlacvxexnsklkdjwommepyymhvpawaadokdzzixxhrrxufzwrndvoosmdqv") == string("nklkbeedbwlheckaseuhepcylxftvlacvxexnsklkdjwommepyymhvpawaadokdzzixxhrrxufzwrndvoosmdqv")) {
		int rhqjhnkvr;
		for (rhqjhnkvr = 76; rhqjhnkvr > 0; rhqjhnkvr--) {
			continue;
		}
	}

}

void flfolis::srjmwgmnuysuvdj(double uumzuoijszptoh, bool bseetnjbl, bool znbksfqpouhh, bool zscntx, int wcyxwhkjqhwlgh, double iacbfuxxyqtrub) {
	string hrrmhuxgv = "pfxztnvhuiborvzpmtihe";
	string efsdwszvxsp = "gradqldekdxaleamaaofnomtdhkbsfnjhpcdtuhtcnvpadqrgqluqrhwxyyazqhtvzjocuojxodpwwhotutzxulmmawqgxy";
	bool wqhemja = true;
	int cotzr = 2085;
	int wegvfmpbw = 1318;
	int porokzxpnqfikz = 3521;
	double ycbmy = 11335;
	int hwlknxibzkfrsdf = 2626;
	string uaigysrzxd = "fanhxbtkjwwhglekmzslebwanqqfhztsshwjmoqlesfefcloccfcbjfhrxbpmsjgoxaewhtvvtytqsacpshzwxewtesmijb";
	int cgjknvffw = 1961;
	if (string("pfxztnvhuiborvzpmtihe") != string("pfxztnvhuiborvzpmtihe")) {
		int fkfic;
		for (fkfic = 27; fkfic > 0; fkfic--) {
			continue;
		}
	}
	if (string("pfxztnvhuiborvzpmtihe") != string("pfxztnvhuiborvzpmtihe")) {
		int petebrml;
		for (petebrml = 22; petebrml > 0; petebrml--) {
			continue;
		}
	}
	if (string("pfxztnvhuiborvzpmtihe") == string("pfxztnvhuiborvzpmtihe")) {
		int kx;
		for (kx = 60; kx > 0; kx--) {
			continue;
		}
	}
	if (string("fanhxbtkjwwhglekmzslebwanqqfhztsshwjmoqlesfefcloccfcbjfhrxbpmsjgoxaewhtvvtytqsacpshzwxewtesmijb") != string("fanhxbtkjwwhglekmzslebwanqqfhztsshwjmoqlesfefcloccfcbjfhrxbpmsjgoxaewhtvvtytqsacpshzwxewtesmijb")) {
		int jnvnzeiewd;
		for (jnvnzeiewd = 91; jnvnzeiewd > 0; jnvnzeiewd--) {
			continue;
		}
	}
	if (1961 == 1961) {
		int xkej;
		for (xkej = 81; xkej > 0; xkej--) {
			continue;
		}
	}

}

int flfolis::ytuhgowskyjjhyezutjq(bool odafq) {
	bool lfdyfbszg = false;
	int jnqhsxzosxip = 2706;
	string vqhuvbfk = "clrkrueeyovwnwvyhptzvybklhxvzhykefzdyrytlladk";
	string drmuojbeaswrgh = "ejivxxiopuaaknagpilxlnattstjniantbyxmqzkllnjoaruf";
	if (false == false) {
		int xiaosg;
		for (xiaosg = 33; xiaosg > 0; xiaosg--) {
			continue;
		}
	}
	return 11908;
}

bool flfolis::kryeputjhwpu() {
	double eqtegac = 12314;
	bool hvvwbbdlzdyuqwu = true;
	int pxkhacctaxdo = 918;
	double qxirtdss = 26317;
	int oudhliimqyy = 2693;
	bool alnogawb = false;
	bool xekqhkixwyjccq = false;
	int kbndo = 1812;
	bool gkazetenlfxzuq = true;
	string tzdqzkmog = "yxlyyufsphsuhucvmolfagwekjdlepskfxna";
	if (1812 != 1812) {
		int cbheymln;
		for (cbheymln = 8; cbheymln > 0; cbheymln--) {
			continue;
		}
	}
	if (false == false) {
		int kormfr;
		for (kormfr = 31; kormfr > 0; kormfr--) {
			continue;
		}
	}
	if (false == false) {
		int silplfg;
		for (silplfg = 82; silplfg > 0; silplfg--) {
			continue;
		}
	}
	return true;
}

void flfolis::bncaccvgjqrkoqbrgqm(double diprjnysdupo, int jeccfqzl, int coafmfhv, bool xsflacasm, bool pqevpgblg, int fhgjbhjnpmm, bool kyjpogikd, int akdjzlggcewh, bool pdtap) {
	string qszxgf = "bgdiidzwlifitirlfhacdqjxrqcwulwesfrquhgjb";
	bool bdqlqmbmrbbzt = false;
	double itmdmjtkqjz = 32975;
	bool vmozxot = true;
	int cfkcbzem = 2493;
	int anttkwshzuur = 6611;
	string hbznggkej = "ndpnun";
	bool kaqmqiasvfxfqb = true;
	double fsmrg = 5479;
	int ihabqcs = 1425;
	if (6611 != 6611) {
		int hy;
		for (hy = 63; hy > 0; hy--) {
			continue;
		}
	}
	if (2493 != 2493) {
		int fjo;
		for (fjo = 39; fjo > 0; fjo--) {
			continue;
		}
	}
	if (32975 == 32975) {
		int wr;
		for (wr = 66; wr > 0; wr--) {
			continue;
		}
	}
	if (5479 == 5479) {
		int hjlbdspz;
		for (hjlbdspz = 100; hjlbdspz > 0; hjlbdspz--) {
			continue;
		}
	}

}

string flfolis::wiyegcktgvgittjuhnvrp(bool jylnoyhakgvmji, bool gdnutrqwwwnod, double fzupcec) {
	string gudqeucgzsbi = "zyeozjlrzmmihdukdaeie";
	bool zdnzfgn = false;
	string dgrucfu = "hywhoodsiujqqtiuedwieztluqsdwyydigqujwnwpfzl";
	double icspuwgqncyfb = 56699;
	double gjkyzut = 6233;
	int zwcghcep = 2180;
	if (56699 != 56699) {
		int mjxr;
		for (mjxr = 16; mjxr > 0; mjxr--) {
			continue;
		}
	}
	return string("wsrhqghgtg");
}

double flfolis::lezyrqxxvsjftfqmeeegivdp(double buchrc) {
	bool erahqnsqpd = true;
	double vjpxluv = 13399;
	if (13399 != 13399) {
		int ydmybqjkqp;
		for (ydmybqjkqp = 34; ydmybqjkqp > 0; ydmybqjkqp--) {
			continue;
		}
	}
	return 95626;
}

double flfolis::hnegsazsvbbjw(bool yvahombax, string eqltyjsofa, bool gxpovfjyybarwj) {
	int xlanofzrul = 1601;
	string fgmoeldydlho = "tbqxczdlpmbwrlihflntmmhtzbhedilzybiprgs";
	if (string("tbqxczdlpmbwrlihflntmmhtzbhedilzybiprgs") != string("tbqxczdlpmbwrlihflntmmhtzbhedilzybiprgs")) {
		int gsk;
		for (gsk = 71; gsk > 0; gsk--) {
			continue;
		}
	}
	if (1601 == 1601) {
		int jsu;
		for (jsu = 65; jsu > 0; jsu--) {
			continue;
		}
	}
	if (1601 == 1601) {
		int qj;
		for (qj = 87; qj > 0; qj--) {
			continue;
		}
	}
	return 94149;
}

double flfolis::kxtiqwuckbekbjo(int bbvurd, int nehvtthedel) {
	int gwpfal = 2;
	int rjadmgvfqb = 43;
	return 36380;
}

double flfolis::hlmcsxdhlvmnsdkxjsbf() {
	double vdjxxhfc = 36236;
	bool hflkffpcbvv = false;
	double tlzmigsnjaw = 37077;
	int uuvlbi = 3994;
	string cqfwy = "ewcvofnljtlzztsgjhryphuleyfilrjtyacznsioyuuqkjduklmmwdmusquahwldgk";
	double xxyvmnrujn = 44250;
	int yxxvnseiktyv = 1301;
	int luicrtx = 1536;
	bool qophkmfrd = true;
	bool amnodnisq = true;
	if (36236 != 36236) {
		int madxemtad;
		for (madxemtad = 27; madxemtad > 0; madxemtad--) {
			continue;
		}
	}
	if (string("ewcvofnljtlzztsgjhryphuleyfilrjtyacznsioyuuqkjduklmmwdmusquahwldgk") == string("ewcvofnljtlzztsgjhryphuleyfilrjtyacznsioyuuqkjduklmmwdmusquahwldgk")) {
		int skqwikoj;
		for (skqwikoj = 85; skqwikoj > 0; skqwikoj--) {
			continue;
		}
	}
	if (true != true) {
		int uas;
		for (uas = 81; uas > 0; uas--) {
			continue;
		}
	}
	return 84312;
}

string flfolis::qwvhwfbwldavnkzfidy(double mxnmv, int ukgqxri, string ltgmhmo, string berlggdhclgrzwe, int fcxtwh, int kawzqrpwkim, bool gtpduxfi, string zwshtkpnwstued) {
	bool vqmbfud = true;
	bool eptpeorlzggvnl = true;
	double hqfngt = 31759;
	bool fqoogjuza = false;
	int wdqdyjetzakna = 483;
	return string("nzxzjwlruiptnudhforb");
}

double flfolis::cdjkcohafojvvvljazc(bool rwzoxjgp, double xjladv, int gdcyyzksufhur, string gyfxc, string tssfe, bool aeqszi) {
	double sfhsvqzampznxvp = 1936;
	int hobigf = 89;
	string kvrdfcprypzutw = "lqqnwveprorpltqyjrqbajbylgbltesl";
	int bfypwqmhyrfql = 43;
	if (43 != 43) {
		int fxwjhjypel;
		for (fxwjhjypel = 99; fxwjhjypel > 0; fxwjhjypel--) {
			continue;
		}
	}
	if (1936 != 1936) {
		int lfhsgbad;
		for (lfhsgbad = 65; lfhsgbad > 0; lfhsgbad--) {
			continue;
		}
	}
	if (43 != 43) {
		int krz;
		for (krz = 58; krz > 0; krz--) {
			continue;
		}
	}
	if (1936 != 1936) {
		int xuaj;
		for (xuaj = 21; xuaj > 0; xuaj--) {
			continue;
		}
	}
	if (89 == 89) {
		int ssdmxdptfq;
		for (ssdmxdptfq = 25; ssdmxdptfq > 0; ssdmxdptfq--) {
			continue;
		}
	}
	return 37377;
}

int flfolis::tzsdnfdwrptqtexhnmj(double cmcgkluso, int oopbzcdz, double rdijktf) {
	double lmknjivtr = 48822;
	bool xwdaglqihv = false;
	string fwyxkgzgkbu = "hdbxnlrygtfkzpqmscvmmhdzptuaifndxrbrbaqxiijgdiqstecovhaayhhxvspismsbqnpfaoznidcfprzbaa";
	double hrvouqzg = 15731;
	string bqeumue = "ugrkmhphfmmnntxwknbbafranqcehauwmrwrhotgxouunbisdvqaimcqpioiqzyebtlidbhaly";
	double fcnnjfzeqk = 1301;
	if (15731 == 15731) {
		int ok;
		for (ok = 70; ok > 0; ok--) {
			continue;
		}
	}
	if (15731 == 15731) {
		int kvia;
		for (kvia = 62; kvia > 0; kvia--) {
			continue;
		}
	}
	return 10150;
}

double flfolis::fvverdqgevluknbtszwn(string tthpukwaafkfpj, bool yutxfprlnthgtbe, int ntajrgwijwcf, int yobmj, string wqnkvdfsyf, int hkjsxzszfnbdj, int vvifgwu, bool etpgbqdx) {
	string cgoturtwmvqrf = "gqrublmiyjvqmrzbawndpomwje";
	double xdzgwnes = 81698;
	string uezvanvvahyaq = "fxtlywhmogjqamkrolifjvchojjexaaxjwoxns";
	double nagawd = 5033;
	string gojpcvo = "yswdqedzuomtbtmnfxsrqcmegobfahqwyangikfwmglcua";
	string vxwlosylqsnxq = "hertgnjjshzolgzsespookefdezgxel";
	int uxeikeycwhiiff = 735;
	double zynevrb = 6072;
	if (string("hertgnjjshzolgzsespookefdezgxel") == string("hertgnjjshzolgzsespookefdezgxel")) {
		int dcefc;
		for (dcefc = 44; dcefc > 0; dcefc--) {
			continue;
		}
	}
	if (81698 == 81698) {
		int piloptva;
		for (piloptva = 30; piloptva > 0; piloptva--) {
			continue;
		}
	}
	if (string("yswdqedzuomtbtmnfxsrqcmegobfahqwyangikfwmglcua") == string("yswdqedzuomtbtmnfxsrqcmegobfahqwyangikfwmglcua")) {
		int sflwvd;
		for (sflwvd = 54; sflwvd > 0; sflwvd--) {
			continue;
		}
	}
	return 92008;
}

string flfolis::wkinpdaecni(double vmrrsriiaiabq) {
	bool olzelkjwu = true;
	double xnvjctthahtnqt = 35682;
	bool bscvwwmxuozc = true;
	double fbxtvujjkkexu = 18939;
	double yskteqqvjrcwl = 15075;
	if (15075 != 15075) {
		int kydf;
		for (kydf = 73; kydf > 0; kydf--) {
			continue;
		}
	}
	if (true == true) {
		int vqnkauarsi;
		for (vqnkauarsi = 9; vqnkauarsi > 0; vqnkauarsi--) {
			continue;
		}
	}
	if (18939 == 18939) {
		int dcfkcatstm;
		for (dcfkcatstm = 36; dcfkcatstm > 0; dcfkcatstm--) {
			continue;
		}
	}
	if (18939 != 18939) {
		int yjoubsiona;
		for (yjoubsiona = 43; yjoubsiona > 0; yjoubsiona--) {
			continue;
		}
	}
	return string("vvjkcjpaop");
}

double flfolis::acsxacxpwgoptkafaoutxxs(bool jfknpvfthkea, bool dvzzdp, double lowcspnjuvwh, double csehakdvxa, int tzuyuvqhxqhudrh, int vwkrvotqyyoxrh, int xmjru) {
	double fewnk = 42617;
	bool dkmypclyoqmzfl = true;
	if (42617 != 42617) {
		int moxahyf;
		for (moxahyf = 84; moxahyf > 0; moxahyf--) {
			continue;
		}
	}
	if (42617 == 42617) {
		int mmtkrsabd;
		for (mmtkrsabd = 30; mmtkrsabd > 0; mmtkrsabd--) {
			continue;
		}
	}
	if (42617 != 42617) {
		int wcvydkod;
		for (wcvydkod = 58; wcvydkod > 0; wcvydkod--) {
			continue;
		}
	}
	if (42617 == 42617) {
		int adp;
		for (adp = 76; adp > 0; adp--) {
			continue;
		}
	}
	return 23341;
}

flfolis::flfolis() {
	this->qwvhwfbwldavnkzfidy(55436, 1416, string("usnsuxuocof"), string("wzxbxbmfnbuuetcmgxkntlyugxqbxsssdwdswjiznnisxekdaenqkgmzbnjocoteqniidimt"), 5831, 3372, false, string("ceuypqsehxmhkuuxabkgtjwwfcxgxuzrspbdupgacqexloauwpsjeanjqgqeruw"));
	this->cdjkcohafojvvvljazc(false, 37084, 1892, string("yxalswovubpibhoixbplkcvkqpxsrvwuhjpyeurovrkhfntkoavoxnafkvxbeiruxobndwywvtjrauehhwlmhdbfvgugqi"), string("ndjdcfdfogbodoawueyngrbxwvk"), true);
	this->tzsdnfdwrptqtexhnmj(42339, 4221, 38124);
	this->fvverdqgevluknbtszwn(string("fvimvjmbplabyeaxgxrjqgqpgbriiwkptylsiduskunyf"), true, 3371, 2836, string("qffjporeqvzwdmexbslgusufyycbvumxbcthncebwftxmmomznf"), 3555, 1210, true);
	this->wkinpdaecni(19899);
	this->acsxacxpwgoptkafaoutxxs(true, true, 21076, 36089, 1615, 7280, 2590);
	this->hnegsazsvbbjw(false, string("zrauaisbkhrxalyhbaeepegnuvwcqefhmpwcsrfecuapfqnrqqghvaszhcb"), true);
	this->kxtiqwuckbekbjo(2682, 3058);
	this->hlmcsxdhlvmnsdkxjsbf();
	this->bdmexnvvskzefrxcrqxgkre(true, 1255, string("kqwlavdnmwysquqixrgdrpavlwrxhtdujraafzvgzutgyeljinycwednjj"), 11637, false, string("zmwexrrsjycwtvwkfipttaajewpyrlndiqhumfjgelpwogxcbgzetrnkdzosrirbxedxjdcrbvqf"), true, 7707);
	this->bgrhnpqqezzlgwjzl();
	this->rgmcxlbnfkgloztuqqyiyja(string("ketwpfzmcyxaktfkepyikokqjxkteximrgsskvexaxdlawbnwlekayfcnyz"), string("rqzyqjjieaadtqutdxkawlkocrlxzggbomlgmsvxku"), string("oxthbtfmxnqpudtxkjmbwqtzmrxfwmarbdrptxzokwuydqzxcphqnhfjkqwljhyyifmvpnnudtrzzkjlfmaazyje"), true, 36270, 18887, 65675, false, 4319, 13508);
	this->fofnrjbrlaiwelmmnieqqygnk(22360, 23178);
	this->srjmwgmnuysuvdj(873, false, true, true, 737, 10);
	this->ytuhgowskyjjhyezutjq(false);
	this->kryeputjhwpu();
	this->bncaccvgjqrkoqbrgqm(26445, 2845, 6730, true, true, 1670, true, 3077, false);
	this->wiyegcktgvgittjuhnvrp(true, true, 14600);
	this->lezyrqxxvsjftfqmeeegivdp(18969);
}
