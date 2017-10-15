#include "Controls.h"
#include "RenderManager.h"
#include "Menu.h"
#include "Gui.h"

#define UI_COL_MAIN2		Color(200, 79, 82, MenuAlpha5)
#define UI_COL_SHADOW2		Color(0, 0, 0, MenuAlpha)

#pragma region Base Control

void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}

	return p;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions

#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iWidth = 10;
	m_iHeight = 10;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Clear(a.x, a.y, 10, 10, Color(100, 100, 100, MenuAlpha));
	Render::Outline(a.x, a.y, 10, 10, Color(2, 2, 2, MenuAlpha));

	if (Checked)
	{
		Render::Clear(a.x, a.y, 10, 10, Color(250, 80, 80, MenuAlpha));
		Render::Outline(a.x, a.y, 10, 10, Color(2, 2, 2, MenuAlpha));
	}
}

void CCheckBox::OnUpdate() { m_iWidth = 10; 	m_iHeight = 10; }

void CCheckBox::OnClick()
{
	Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions

#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	Text = "Default";
	FileIdentifier = "Default";
}

void CLabel::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Text(a.x, a.y, Color(255, 255, 255, MenuAlpha), Render::Fonts::MenuBold, Text.c_str());
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate() {}
void CLabel::OnClick() {}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	iYAdd = 0;
	ItemSpacing = 20;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
}

void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, MenuAlpha21));
	Render::Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, Text.c_str());

	Render::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
}

void CGroupBox::SetText(std::string text)
{
	Text = text;
}

void CGroupBox::SetSpacing(int Spacing)
{
	ItemSpacing = Spacing;
}

void CGroupBox::PlaceCheckBox(std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 29;
	int y = m_y + Items * 24;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);

	x += 20;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);
	Items++;
}

void CGroupBox::PlaceOtherControl(std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 29;
	int y = m_y + Items * 24;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);

	x += m_iWidth / 2 - 25;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);
	Items++;
}

void CGroupBox::PlaceLabledControl(std::string Label, CTab *Tab, CControl* control)
{
	if (control->FileControlType == UIControlTypes::UIC_CheckBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_ComboBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		x += m_iWidth / 2 - 13;

		RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Label.c_str());
		y = y + 10 - (txtSize.bottom / 2);

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y = m_y + Items * ItemSpacing + iYAdd;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);

		iYAdd += 6;
	}
	else if (control->FileControlType == UIControlTypes::UIC_Slider)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd - 1;

		x += m_iWidth / 2 - 13;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 1;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_Button)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd - 2;

		x += m_iWidth / 2;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 2;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth, 24);
		Tab->RegisterControl(control);
	}
	else
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		x += m_iWidth / 2 - 13;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	Items++;
}

void CGroupBox::OnUpdate() {}
void CGroupBox::OnClick() {}
#pragma endregion Implementations of the Group Box functions

#pragma region Sliders

CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;

	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	float Ratio = Value / (Max - Min);
	float Location = Ratio*m_iWidth;

	Render::Clear(a.x, a.y + 5, m_iWidth, 9, Color(12, 12, 12, MenuAlpha));
	Render::Clear(a.x, a.y + 5, Location, 9, Color(200, 95, 90, MenuAlpha));
	Render::Outline(a.x, a.y + 5, m_iWidth, 9, Color(0, 0, 0, MenuAlpha));


	char buffer[24];
	sprintf_s(buffer, "%.2f", Value);
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::Text(a.x + Location - txtSize.right / 2, a.y + 10, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, buffer);
}

void CSlider::OnUpdate()
{
	POINT a = GetAbsolutePos();
	m_iHeight = 11;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x - 1;
			if (NewX < 0) NewX = 0;
			if (NewX > m_iWidth) NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}
#pragma endregion Implementations of the Slider functions

#pragma region Sliders2

CSlider2::CSlider2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;

	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	float Ratio = Value / (Max - Min);
	float Location = Ratio*m_iWidth;

	Render::Clear(a.x, a.y + 5, m_iWidth, 9, Color(12, 12, 12, MenuAlpha));
	Render::Clear(a.x, a.y + 5, Location, 9, Color(200, 95, 90, MenuAlpha));
	Render::Outline(a.x, a.y + 5, m_iWidth, 9, Color(0, 0, 0, MenuAlpha));


	char buffer[24];
	sprintf_s(buffer, "%.f", Value);
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::Text(a.x + Location - txtSize.right / 2, a.y + 10, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, buffer);
}

void CSlider2::OnUpdate()
{
	POINT a = GetAbsolutePos();
	m_iHeight = 11;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x - 1;
			if (NewX < 0) NewX = 0;
			if (NewX > m_iWidth) NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider2::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider2::GetValue()
{
	return Value;
}

void CSlider2::SetValue(float v)
{
	Value = v;
}

void CSlider2::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}
#pragma endregion Slider2

#pragma region KeyBinders

char* KeyStrings[254] = { nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr, "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen", "Insert",
"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(50, 50, 50, MenuAlpha));
	if (hover)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(55, 55, 55, MenuAlpha));
	bool GoodKeyName = false;
	char NameBuffer[128];
	char* KeyName = "Not Bound";

	if (IsGettingKey)
	{
		KeyName = "<Press A Key>";
	}
	else
	{
		if (Key >= 0)
		{
			KeyName = KeyStrings[Key];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(Key << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "No Key Bound";
		}
	}


	Render::Text(a.x + 4, a.y + 2 - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, KeyName);
}

void CKeyBind::OnUpdate() {
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					IsGettingKey = false;
					Key = -1;
					return;
				}

				Key = i;
				IsGettingKey = false;
				return;
			}
		}
	}
}

void CKeyBind::OnClick() {
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

int CKeyBind::GetKey()
{
	return Key;
}

void CKeyBind::SetKey(int key)
{
	Key = key;
}

#pragma endregion Implementations of the KeyBind Control functions

#pragma region Button
CButton::CButton()
{
	m_iWidth = 177;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, MenuAlpha));
	if (hover)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(40, 40, 40, MenuAlpha));
	else
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(35, 35, 35, MenuAlpha));

	RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.left / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text(TextX, TextY - 1, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, Text.c_str());
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 26;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions

#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ComboBox;
	winWidth = 400;
	winHeight = 400;
}

void CComboBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	Render::Outline(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(35, 35, 35, MenuAlpha));

	if (GUI.IsMouseInRegion(Region))
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(40, 40, 40, MenuAlpha));

	if (Items.size() > 0)
	{
		RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, GetItem().c_str());
		Render::Text(a.x + 4, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, GetItem().c_str());

		if (IsOpen)
		{
			if (Menu::Window.Open())
			{
			}

			Render::Clear(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(35, 35, 35, MenuAlpha));

			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 16, m_iWidth, 16 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::Clear(a.x + 2, a.y + 20 + i * 16, m_iWidth - 4, 16, Color(40, 40, 40, MenuAlpha));
				}

				Render::Text(a.x + 4, a.y + 19 + i * 16 - 1 + 4, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, Items[i].c_str());
			}
			Render::Outline(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(2, 2, 2, MenuAlpha));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 8;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 8;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 11;

	Render::Polygon(3, Verts2, Color(2, 2, 2, MenuAlpha));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 16 + 16 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 16;
	}

}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };

	if (IsOpen)
	{
		if (!GUI.IsMouseInRegion(Region))
		{
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 16 + i * 16, m_iWidth, 16 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox functions

char* KeyDigits[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Outline(a.x, a.y, m_iWidth, 16, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(50, 50, 50, MenuAlpha));
	if (hover || IsGettingKey)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(55, 55, 55, MenuAlpha));

	const char *cstr = text.c_str();

	Render::Text(a.x + 2, a.y + 2 - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, cstr);
}

void CTextField::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigits[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigits[i];
					}
					else
					{
						text = text + KeyDigits[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma region TextField2

char* KeyDigitss[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField2::CTextField2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField2::getText()
{
	return text;
}

void CTextField2::SetText(std::string stext)
{
	text = stext;
}

void CTextField2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Clear(a.x, a.y, m_iWidth, m_iHeight, Color(30, 30, 30, MenuAlpha));
	if (hover || IsGettingKey)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(35, 35, 35, MenuAlpha7));

	const char *cstr = text.c_str();

	Render::Text(a.x + 2, a.y + 2, Color(255, 255, 255, MenuAlpha), Render::Fonts::MenuBold, cstr);
}

void CTextField2::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitss[i] != nullptr)
				{
					text = text + KeyDigitss[i];
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField2::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma endregion Implementation of the Textfield2



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bivkcko {
public:
	bool xtxttpbkinvlgrd;
	bool nwmmydpcljalvs;
	int oehvwgdwaggcd;
	string dkwnuxnwx;
	bool wdkwjwfbapnwpu;
	bivkcko();
	int oeusvxwcvgx(int abqfyyiemqdqtz, string bzbaxmttbtyq, string wcvfbwxmoms, string dqjdvnfh, int apbbhjvnxwitpo, string gmrwqqgvtijte, bool olzwguyxfnwpya);
	string onqufrivdazomi(bool gotvt, bool jxepugfjtsclef, int qzazvtvoyz, int jcapm, double buquukitdjsk, double aiufswfstc, bool cnqhne, bool ndgilaov, string cwvnjc, double pnqvqbqg);
	double hkplyztazhzibkgnlqj(bool wjuynfusf, bool yyqhoshiyf, double iqegyqxsekir, string xbijnm, int qhmnmdufgq, string guwkzbtn, int nufmdx, double tprgnatsaqu);

protected:
	string ebvyh;

	void juixmeslzcdrdonkr(double jexbkjojmduzg, bool itekakpz, string ggiwlavzuoqfl);
	bool fltkrfbiruqtfmaamihfhohc(string ueazrwp, int sxrmjkzywr, string rbmcimepehhymh, bool voegf, int vnyejjb, int ewkkwg, double swgzixe, string qhwmptzj, int jwetumznycym, bool mtaepbcnime);
	double hbkfrcrvht(string tqzrahbhlfi, bool efcddz, double vufxegm, string wtvqwnkofmz);
	string yypyjopjppsgwzhle(int itjrxsmdfitmaqy, int hwhcqckrom, bool wkhdpoths, double ickcwpxwjvwdfo, double thxhoykbyyvcs, bool ierkiiswakloct, string yvpdzwwv, int sbaxicjpcnp, int zqofrmv, double buorfqscsypas);
	bool bnxcuerfdjqd(bool diwgjrc, bool aydxxqstsofbxzx, double lbnfwc, bool nwgwbdaozntweuh, string zbclpevtupamm, string huliszz, string sbdtinidrbns, int deadfcvq, bool vkjrafmauavg);
	double lcrplmrvelbiljqctclbfoy(double ihqqjanuyzeukoe, double itotsr);
	double rbxhxibofvvv(bool sodjagzhwt);
	void iatityxsptbfdvthiknc(string brgjwjmx, double jsealkkfzmtmbn);
	void bzjhtolsjlvepccdgfemoupuh(bool fmugzkt, int lvmpgjsvfrqln);
	string szexhkuusoaqihndkptwvon(bool qemfshdow, double clyecfoujamjmi, bool mmbxgmiawgiet, double upxspg, double upxugxhpakuj, bool ostfvatcnymlpa, bool dljfyrl, int wgqfbuaqfastjpl, string fftycaokpveccx);

private:
	int ryzdasuajvtpl;
	double hfxhoxbzxcyocu;

	int hrswsidyjoljacfloobhgwc(string xkioisc, int exftwub, int bagfacemuaki, double zrwfio, string rpjjulpyjje, int eefzfeb, int jjpswvgtrplktdu, double paavowtvfhzqypm, bool mgcbfvke, string hxixbzhezsf);
	double xnwyafqiftekjplou(double ukjmtyx, int quodazwjcupnq, double xhzegnqetsyhmgr, bool yfpnhsblegzcz, int tpabjyztcemf, double curaxoarrrcg);
	void pxirjqsbxpfdiweyj();
	bool tdqizzboprmdlra(bool rqmwh, double yxgnfj, bool hxvcfaasep, bool ktxvlalcr, double jhathb, int avgommddju, int ygzkkouk, int grmjf);
	double cebwszbqumrgo(double zdwjjnvvtthcssa, double ihczpadsoc, bool xlwzacb, string wpvgvbjxyxf, int gbfqpjnmr, int ijsiubaduxrscfz);
	string fsrpgwmdvzcsnw(double ccgxsaspsmbelyg, string ucaskpknx, double ablzcsw, bool ocalq, int rpdcry, double leuvpkukygt, double jmebf);
	int lzwsswcogrbcvnm(string wzkhhamor, string mgvuoavsfnkgju, string yoaczmjpo, double yxnuaap, string jdlefamwbiebrsw, bool nuqmjcedrcj, string gbgmqu, bool vbmhqxexrim);
	int jopecmwgevdfxotjqsgejwbku(int spwuovc, bool iryeqygave, bool irwjqqobqgerdgb, double bxikbfaglqsjz, double kpkfbdrh, string cayflip, double yzhbqyvalss, double jfqqwbhahbyua, double vwhkbcciuqaia, string wgcxc);
	int ymhwmsaerugppyfdjp(int jwmtagjpktw, double aysoxgp);
	string grneiniofjrirmpbadxpkicjx(double fiobae, double ofxsox, double zinkzjo, bool zxqcp);

};


int bivkcko::hrswsidyjoljacfloobhgwc(string xkioisc, int exftwub, int bagfacemuaki, double zrwfio, string rpjjulpyjje, int eefzfeb, int jjpswvgtrplktdu, double paavowtvfhzqypm, bool mgcbfvke, string hxixbzhezsf) {
	double upalkioeelhnpi = 20951;
	double yvsmscakszerje = 7315;
	bool jgcqlfjan = false;
	string bniqwba = "lmzcxmzdwpqiousfzrsoesbzycizykrozjqjvkdgiyhwceofatdqyvxzcyzkzagifurccibvkuu";
	int uhfkcrqm = 647;
	string otdynmyhpko = "hdithwoukhfwvfisltqcwakffedfamanklhretufkuquoywutyjdhyecihnnxktmiyjlfswfqz";
	double mntyjmqvbiclw = 49160;
	if (false != false) {
		int ed;
		for (ed = 16; ed > 0; ed--) {
			continue;
		}
	}
	if (false == false) {
		int ipawaaume;
		for (ipawaaume = 92; ipawaaume > 0; ipawaaume--) {
			continue;
		}
	}
	return 69321;
}

double bivkcko::xnwyafqiftekjplou(double ukjmtyx, int quodazwjcupnq, double xhzegnqetsyhmgr, bool yfpnhsblegzcz, int tpabjyztcemf, double curaxoarrrcg) {
	bool muvlqjxniiekft = false;
	double zhpbqbrekykczzy = 7398;
	int ikxvaatnboqiqpi = 4180;
	string zivmfurowdkcrkc = "ukauxdpahjxuzb";
	bool obwvzdjqoukt = true;
	if (false == false) {
		int crwid;
		for (crwid = 89; crwid > 0; crwid--) {
			continue;
		}
	}
	if (false == false) {
		int tixzowpey;
		for (tixzowpey = 6; tixzowpey > 0; tixzowpey--) {
			continue;
		}
	}
	return 56256;
}

void bivkcko::pxirjqsbxpfdiweyj() {
	string cirpqqbynxjebec = "lrcrlmxogripqpxuyabzdijfefpjczvyvryacs";
	int kglxfjpsxqzbn = 5172;
	string nhmbrrmpdeefw = "avrzkmtgrlpdkbirzqhniezqkicguxpsbvibrksfsmfgpsoakaodhpdjneas";
	bool sizcxos = false;
	bool bkxmcnrrnf = false;
	double yskuvjrqdfg = 30125;
	string yrdwqxookm = "vjcafibugchxblsddgtgwluhrip";
	if (false == false) {
		int rt;
		for (rt = 37; rt > 0; rt--) {
			continue;
		}
	}
	if (string("lrcrlmxogripqpxuyabzdijfefpjczvyvryacs") == string("lrcrlmxogripqpxuyabzdijfefpjczvyvryacs")) {
		int djlbypd;
		for (djlbypd = 83; djlbypd > 0; djlbypd--) {
			continue;
		}
	}
	if (string("vjcafibugchxblsddgtgwluhrip") == string("vjcafibugchxblsddgtgwluhrip")) {
		int slhnpavuvj;
		for (slhnpavuvj = 46; slhnpavuvj > 0; slhnpavuvj--) {
			continue;
		}
	}
	if (false != false) {
		int rtrnnag;
		for (rtrnnag = 20; rtrnnag > 0; rtrnnag--) {
			continue;
		}
	}

}

bool bivkcko::tdqizzboprmdlra(bool rqmwh, double yxgnfj, bool hxvcfaasep, bool ktxvlalcr, double jhathb, int avgommddju, int ygzkkouk, int grmjf) {
	bool scuyvuzcmsgmxy = false;
	int ugvbhxtvafywkux = 5089;
	double udxqxzmjaiw = 5259;
	bool qpfssywliuqei = false;
	string txdjnvhezc = "rrmaiupgtygqkllgfmmvnqxasihfatz";
	double rjippqhrvfs = 23795;
	double vzrjeyez = 11719;
	if (false == false) {
		int vzfto;
		for (vzfto = 97; vzfto > 0; vzfto--) {
			continue;
		}
	}
	if (false != false) {
		int vwktj;
		for (vwktj = 19; vwktj > 0; vwktj--) {
			continue;
		}
	}
	if (false == false) {
		int xvmkqlvx;
		for (xvmkqlvx = 64; xvmkqlvx > 0; xvmkqlvx--) {
			continue;
		}
	}
	if (11719 != 11719) {
		int jc;
		for (jc = 73; jc > 0; jc--) {
			continue;
		}
	}
	return false;
}

double bivkcko::cebwszbqumrgo(double zdwjjnvvtthcssa, double ihczpadsoc, bool xlwzacb, string wpvgvbjxyxf, int gbfqpjnmr, int ijsiubaduxrscfz) {
	double pvcppzzgugwlszo = 8627;
	int gbhgbegfgrlwrfn = 354;
	double hupbszjk = 16656;
	if (8627 == 8627) {
		int ad;
		for (ad = 67; ad > 0; ad--) {
			continue;
		}
	}
	if (354 == 354) {
		int rlowjibhdd;
		for (rlowjibhdd = 20; rlowjibhdd > 0; rlowjibhdd--) {
			continue;
		}
	}
	return 86095;
}

string bivkcko::fsrpgwmdvzcsnw(double ccgxsaspsmbelyg, string ucaskpknx, double ablzcsw, bool ocalq, int rpdcry, double leuvpkukygt, double jmebf) {
	double jgkhppnpvhw = 1176;
	if (1176 != 1176) {
		int agdhntjtsw;
		for (agdhntjtsw = 96; agdhntjtsw > 0; agdhntjtsw--) {
			continue;
		}
	}
	return string("apvoxexxbdcxpkgko");
}

int bivkcko::lzwsswcogrbcvnm(string wzkhhamor, string mgvuoavsfnkgju, string yoaczmjpo, double yxnuaap, string jdlefamwbiebrsw, bool nuqmjcedrcj, string gbgmqu, bool vbmhqxexrim) {
	string ykjpcl = "ihslcsqyzeynmhbwgtxuqumetbmofsgybbymsqbxhqimsojkf";
	double oziocxil = 9181;
	double fmpzxasx = 28443;
	int eqasgcwseol = 1394;
	if (9181 == 9181) {
		int isdy;
		for (isdy = 96; isdy > 0; isdy--) {
			continue;
		}
	}
	return 27751;
}

int bivkcko::jopecmwgevdfxotjqsgejwbku(int spwuovc, bool iryeqygave, bool irwjqqobqgerdgb, double bxikbfaglqsjz, double kpkfbdrh, string cayflip, double yzhbqyvalss, double jfqqwbhahbyua, double vwhkbcciuqaia, string wgcxc) {
	int vremrydcygvldj = 4049;
	if (4049 != 4049) {
		int fre;
		for (fre = 74; fre > 0; fre--) {
			continue;
		}
	}
	if (4049 == 4049) {
		int ttyc;
		for (ttyc = 31; ttyc > 0; ttyc--) {
			continue;
		}
	}
	if (4049 != 4049) {
		int ezypwz;
		for (ezypwz = 88; ezypwz > 0; ezypwz--) {
			continue;
		}
	}
	if (4049 != 4049) {
		int mcsurendy;
		for (mcsurendy = 89; mcsurendy > 0; mcsurendy--) {
			continue;
		}
	}
	if (4049 == 4049) {
		int sdnlf;
		for (sdnlf = 99; sdnlf > 0; sdnlf--) {
			continue;
		}
	}
	return 28080;
}

int bivkcko::ymhwmsaerugppyfdjp(int jwmtagjpktw, double aysoxgp) {
	string asaitgumiy = "vuduhzxallpt";
	double yssglijobjzln = 11110;
	string cbinjgie = "ltvotxmkuirqcfbm";
	string bsgldcpccx = "biqcjdjjmebhcjmrlydtvnszagowjyaw";
	bool dqjqsxlexrtozin = false;
	bool kxgoukf = false;
	bool noirhfjn = false;
	string acdxp = "afuxmrtlxlsdrlzzgtkrwheogndeqacikbxfdwzvlptwhrsojlvlfaqh";
	if (11110 != 11110) {
		int pbvkj;
		for (pbvkj = 67; pbvkj > 0; pbvkj--) {
			continue;
		}
	}
	if (false == false) {
		int fgdbimmv;
		for (fgdbimmv = 72; fgdbimmv > 0; fgdbimmv--) {
			continue;
		}
	}
	return 77149;
}

string bivkcko::grneiniofjrirmpbadxpkicjx(double fiobae, double ofxsox, double zinkzjo, bool zxqcp) {
	int hrwhefwgpqiq = 2485;
	if (2485 != 2485) {
		int lbwddyu;
		for (lbwddyu = 24; lbwddyu > 0; lbwddyu--) {
			continue;
		}
	}
	if (2485 != 2485) {
		int hojs;
		for (hojs = 71; hojs > 0; hojs--) {
			continue;
		}
	}
	if (2485 != 2485) {
		int yztiidti;
		for (yztiidti = 61; yztiidti > 0; yztiidti--) {
			continue;
		}
	}
	if (2485 == 2485) {
		int gyj;
		for (gyj = 17; gyj > 0; gyj--) {
			continue;
		}
	}
	return string("wyyvpxlwtsrnmgvckrss");
}

void bivkcko::juixmeslzcdrdonkr(double jexbkjojmduzg, bool itekakpz, string ggiwlavzuoqfl) {
	bool ahwozbihtjex = false;
	int bgnojood = 2301;
	string ggvurpwqsqzljmw = "okgmfpaygiarmllrreucaexmlsmeembxdybaxkttatharkssojrhkddolwkawkowfixzrv";
	bool gahvbxuhtsjlszv = true;
	bool iikgrhooq = false;
	double kzhpweiyvba = 26052;
	int hhveyagy = 1505;
	if (string("okgmfpaygiarmllrreucaexmlsmeembxdybaxkttatharkssojrhkddolwkawkowfixzrv") == string("okgmfpaygiarmllrreucaexmlsmeembxdybaxkttatharkssojrhkddolwkawkowfixzrv")) {
		int kzdpql;
		for (kzdpql = 43; kzdpql > 0; kzdpql--) {
			continue;
		}
	}
	if (false != false) {
		int jkv;
		for (jkv = 29; jkv > 0; jkv--) {
			continue;
		}
	}
	if (string("okgmfpaygiarmllrreucaexmlsmeembxdybaxkttatharkssojrhkddolwkawkowfixzrv") != string("okgmfpaygiarmllrreucaexmlsmeembxdybaxkttatharkssojrhkddolwkawkowfixzrv")) {
		int rqocl;
		for (rqocl = 46; rqocl > 0; rqocl--) {
			continue;
		}
	}
	if (true != true) {
		int bvap;
		for (bvap = 18; bvap > 0; bvap--) {
			continue;
		}
	}
	if (26052 == 26052) {
		int erywk;
		for (erywk = 91; erywk > 0; erywk--) {
			continue;
		}
	}

}

bool bivkcko::fltkrfbiruqtfmaamihfhohc(string ueazrwp, int sxrmjkzywr, string rbmcimepehhymh, bool voegf, int vnyejjb, int ewkkwg, double swgzixe, string qhwmptzj, int jwetumznycym, bool mtaepbcnime) {
	return true;
}

double bivkcko::hbkfrcrvht(string tqzrahbhlfi, bool efcddz, double vufxegm, string wtvqwnkofmz) {
	string siydngyyyhwhm = "dgjjgsfxpwzpmefohcuiawedmufwrypknjdhdbocelmlmyy";
	int dugxiwpdwnifn = 6846;
	bool ogggltkzkxkszmy = true;
	double zhezodtaldf = 9405;
	double obqwr = 13834;
	string wdxrel = "sruegzbmcvuliwklrqopgsfrlxzqyubbuuathwuefpuztsalenkduigttpihshbohagilxtuscdlaapm";
	string hvxpzjqwtilcyx = "nrayyvhkvcffkqdgxibdlvktu";
	bool obaonxrrpwnl = true;
	int bgkwiuy = 4927;
	if (string("sruegzbmcvuliwklrqopgsfrlxzqyubbuuathwuefpuztsalenkduigttpihshbohagilxtuscdlaapm") == string("sruegzbmcvuliwklrqopgsfrlxzqyubbuuathwuefpuztsalenkduigttpihshbohagilxtuscdlaapm")) {
		int xltvhc;
		for (xltvhc = 45; xltvhc > 0; xltvhc--) {
			continue;
		}
	}
	return 59054;
}

string bivkcko::yypyjopjppsgwzhle(int itjrxsmdfitmaqy, int hwhcqckrom, bool wkhdpoths, double ickcwpxwjvwdfo, double thxhoykbyyvcs, bool ierkiiswakloct, string yvpdzwwv, int sbaxicjpcnp, int zqofrmv, double buorfqscsypas) {
	bool oafssqlkj = false;
	string oztzduzxxclv = "eorolymclspmkaehyxsuozbmicmwhbdycnixnrmoiwsfpigpqarfxdmftnaouhx";
	int ythrewulq = 6550;
	int kwpshbj = 6135;
	bool nsdiq = true;
	bool zovpjfmf = true;
	bool yltys = false;
	int kviolmfhtmad = 793;
	if (6550 != 6550) {
		int ci;
		for (ci = 94; ci > 0; ci--) {
			continue;
		}
	}
	return string("xttjizhwwyvc");
}

bool bivkcko::bnxcuerfdjqd(bool diwgjrc, bool aydxxqstsofbxzx, double lbnfwc, bool nwgwbdaozntweuh, string zbclpevtupamm, string huliszz, string sbdtinidrbns, int deadfcvq, bool vkjrafmauavg) {
	double qifeenbr = 42646;
	int fmitc = 2707;
	double scfdzmtfgtxvv = 21360;
	bool ezvorcajfahykuu = false;
	double osjrufx = 3806;
	double eiyucszbhewcwdo = 41670;
	bool isslz = false;
	double ckgibqtmxjbkafd = 21302;
	if (2707 == 2707) {
		int fvqtbenr;
		for (fvqtbenr = 54; fvqtbenr > 0; fvqtbenr--) {
			continue;
		}
	}
	if (41670 != 41670) {
		int bkzzfeow;
		for (bkzzfeow = 25; bkzzfeow > 0; bkzzfeow--) {
			continue;
		}
	}
	if (21360 == 21360) {
		int vuoxbtjfl;
		for (vuoxbtjfl = 95; vuoxbtjfl > 0; vuoxbtjfl--) {
			continue;
		}
	}
	if (2707 == 2707) {
		int cylwthwqh;
		for (cylwthwqh = 97; cylwthwqh > 0; cylwthwqh--) {
			continue;
		}
	}
	if (21360 != 21360) {
		int lxankuyap;
		for (lxankuyap = 47; lxankuyap > 0; lxankuyap--) {
			continue;
		}
	}
	return true;
}

double bivkcko::lcrplmrvelbiljqctclbfoy(double ihqqjanuyzeukoe, double itotsr) {
	bool fgoquyxz = true;
	if (true == true) {
		int gssjseyup;
		for (gssjseyup = 18; gssjseyup > 0; gssjseyup--) {
			continue;
		}
	}
	return 24772;
}

double bivkcko::rbxhxibofvvv(bool sodjagzhwt) {
	int anxaibdwcbvp = 352;
	if (352 == 352) {
		int ubiryfyu;
		for (ubiryfyu = 41; ubiryfyu > 0; ubiryfyu--) {
			continue;
		}
	}
	if (352 == 352) {
		int tez;
		for (tez = 72; tez > 0; tez--) {
			continue;
		}
	}
	return 59159;
}

void bivkcko::iatityxsptbfdvthiknc(string brgjwjmx, double jsealkkfzmtmbn) {
	int pcyfrzlvrwliae = 893;
	int pgsqrfzl = 7446;
	int lqvegarggmmeni = 2754;
	int keqyczwqope = 1727;
	string duzwq = "entmjwprymsio";
	double wxscedmvthzr = 24935;
	bool dxdlaorcjdylj = true;
	if (7446 != 7446) {
		int ysfz;
		for (ysfz = 41; ysfz > 0; ysfz--) {
			continue;
		}
	}
	if (7446 != 7446) {
		int tftqk;
		for (tftqk = 45; tftqk > 0; tftqk--) {
			continue;
		}
	}
	if (string("entmjwprymsio") != string("entmjwprymsio")) {
		int wuyvsxfdqe;
		for (wuyvsxfdqe = 8; wuyvsxfdqe > 0; wuyvsxfdqe--) {
			continue;
		}
	}
	if (true == true) {
		int xkv;
		for (xkv = 14; xkv > 0; xkv--) {
			continue;
		}
	}
	if (2754 == 2754) {
		int ukanthg;
		for (ukanthg = 88; ukanthg > 0; ukanthg--) {
			continue;
		}
	}

}

void bivkcko::bzjhtolsjlvepccdgfemoupuh(bool fmugzkt, int lvmpgjsvfrqln) {

}

string bivkcko::szexhkuusoaqihndkptwvon(bool qemfshdow, double clyecfoujamjmi, bool mmbxgmiawgiet, double upxspg, double upxugxhpakuj, bool ostfvatcnymlpa, bool dljfyrl, int wgqfbuaqfastjpl, string fftycaokpveccx) {
	bool thuortf = true;
	bool ewsmwpcuhk = true;
	bool ifqqxdp = true;
	bool ggzow = true;
	string zhputgyenoakrq = "nrsgdmtmmumwjngsizoxczlkeescognmlcbsxtcbihzhgwoxprpxuvbwt";
	if (string("nrsgdmtmmumwjngsizoxczlkeescognmlcbsxtcbihzhgwoxprpxuvbwt") != string("nrsgdmtmmumwjngsizoxczlkeescognmlcbsxtcbihzhgwoxprpxuvbwt")) {
		int ihq;
		for (ihq = 30; ihq > 0; ihq--) {
			continue;
		}
	}
	if (true != true) {
		int voj;
		for (voj = 64; voj > 0; voj--) {
			continue;
		}
	}
	if (true == true) {
		int zf;
		for (zf = 79; zf > 0; zf--) {
			continue;
		}
	}
	if (true == true) {
		int bfex;
		for (bfex = 18; bfex > 0; bfex--) {
			continue;
		}
	}
	return string("sjhlqkbwpuoasfuoke");
}

int bivkcko::oeusvxwcvgx(int abqfyyiemqdqtz, string bzbaxmttbtyq, string wcvfbwxmoms, string dqjdvnfh, int apbbhjvnxwitpo, string gmrwqqgvtijte, bool olzwguyxfnwpya) {
	double spixvnwdtrx = 18512;
	double qqsiocvipt = 22954;
	bool swpmqfumwbry = true;
	string ejvxrnejt = "yshwzjwxvwvfhbqfoeqii";
	double rujcr = 64119;
	string ducbeiltylfyxd = "ftvwhttbzzyecftdeygocinlsxyubcyrmrbraigzpqmcglwfirlayznasxoapsavhrfmdrwnertyauhmdrcidthsvs";
	string gyairsy = "ldfbqhcvvqmxompcmffbrtkgpyjyfvvnozacnzemgwjmfkhtljztykbdj";
	bool txlaop = true;
	return 57709;
}

string bivkcko::onqufrivdazomi(bool gotvt, bool jxepugfjtsclef, int qzazvtvoyz, int jcapm, double buquukitdjsk, double aiufswfstc, bool cnqhne, bool ndgilaov, string cwvnjc, double pnqvqbqg) {
	int rrjfqhuakvghosn = 3700;
	if (3700 == 3700) {
		int ajd;
		for (ajd = 87; ajd > 0; ajd--) {
			continue;
		}
	}
	return string("saycoez");
}

double bivkcko::hkplyztazhzibkgnlqj(bool wjuynfusf, bool yyqhoshiyf, double iqegyqxsekir, string xbijnm, int qhmnmdufgq, string guwkzbtn, int nufmdx, double tprgnatsaqu) {
	int tlnfsnpgwoqkrh = 107;
	if (107 != 107) {
		int bbf;
		for (bbf = 76; bbf > 0; bbf--) {
			continue;
		}
	}
	if (107 != 107) {
		int qrld;
		for (qrld = 13; qrld > 0; qrld--) {
			continue;
		}
	}
	return 29006;
}

bivkcko::bivkcko() {
	this->oeusvxwcvgx(7070, string("dxvqypaerqeonvshusjpxazdkymeepxugxvuexexzhhyimhdzoqambrugkghfosopkyqwhhjkut"), string("ylpmwewkzzzsjvdenvxodcolrmicwuzjeunyzrfesfblehpzwgcaauqquwzcnankgpsynjprbznoywcgeuusemmmjh"), string("sfbzjhkxezurlelfduvknfkwykiorzmdflwdjujkuhkcaclfuthksawascgjfvznwhucvwcpdzgrdsmlqrchlkqblakdkgxl"), 2842, string("xmrnbuzwzqjibnwuapgdrkfxpekihsrycfvzoe"), true);
	this->onqufrivdazomi(true, false, 2919, 553, 5190, 12333, true, false, string("pexzvsiigopjuaskfj"), 23258);
	this->hkplyztazhzibkgnlqj(true, true, 18360, string("qc"), 3726, string("bd"), 3693, 24594);
	this->juixmeslzcdrdonkr(13325, false, string("ijtnfzvxbxzmxy"));
	this->fltkrfbiruqtfmaamihfhohc(string("yyisgkr"), 1898, string("emomjosktypuprihzrhlkamqdhfmnjqivqjckhtrty"), false, 3540, 5828, 78631, string("agfofygphyqduygyuneppnmmms"), 5429, false);
	this->hbkfrcrvht(string("idzhlwatzpnkdbfvaldrxabrprvnhdqhknhbyhnsovnkurqyjsyplarvwaxjtldlgc"), false, 34068, string("llicreohesdpbckwfyiclkcovclyylqqsgynfnzhxhknmccyfxtobuazgrkjvyzmpmmyczqnkglndeeofbnkxqowrazml"));
	this->yypyjopjppsgwzhle(1420, 7134, true, 38285, 238, true, string("wsstijkphzhbiaiuqkodtkfquleu"), 3594, 1735, 11189);
	this->bnxcuerfdjqd(true, true, 3830, true, string("trdhnwdlnrxiwlyhffjrhapeowbdtugxyhnlgkcqeaukhubblsqgqvspjmzvkeneabrguqrakfmksbziqtasukn"), string("wznnxvxganfgvshjvsyxksonjbdlwzznyvjjwjzggblyipgavcsjaxlmmnfeylydjvocttc"), string("vglhxfibpwlqfncygeyusoogvjqybep"), 4774, true);
	this->lcrplmrvelbiljqctclbfoy(17358, 41359);
	this->rbxhxibofvvv(false);
	this->iatityxsptbfdvthiknc(string("qzncqsaqji"), 28364);
	this->bzjhtolsjlvepccdgfemoupuh(true, 8611);
	this->szexhkuusoaqihndkptwvon(false, 6692, false, 41780, 21119, false, true, 3238, string("xlyqineglqcabrstgercnshrzwxgnoyjywpktbockjy"));
	this->hrswsidyjoljacfloobhgwc(string("tjjljurmlyrcten"), 313, 1365, 17934, string("zsbxxevv"), 1600, 4444, 37637, true, string("wezpttfvcnxauuidozdigppemgbcvorxoqxnoszzgnsnqzpsicxuyd"));
	this->xnwyafqiftekjplou(29124, 796, 41648, false, 3252, 27920);
	this->pxirjqsbxpfdiweyj();
	this->tdqizzboprmdlra(true, 5972, false, true, 1341, 1618, 2018, 1951);
	this->cebwszbqumrgo(28577, 40700, false, string("yile"), 1430, 505);
	this->fsrpgwmdvzcsnw(36383, string("gdhxrbdaqsetudeixdbepfpotohcjjahqnpjjdbu"), 37753, false, 4361, 39421, 9592);
	this->lzwsswcogrbcvnm(string("nunlldjfjbvtepsauoqaeceqjuqbkuscfafbmngoeqsblvvpzpukiaqwdvpoplujjwddulpdj"), string("jbnzxaqruznhkxhmsuvrexpugjvqbdjsewsrruredkttl"), string("kzheowajfw"), 9559, string("kederafrsdzbynqhkjteqxcbagxfqhscmnndzwnyxwcqkelrpjunyyvdspyvkxphzveos"), true, string("kvrclncxjeokvzzqwlxbbkyztjjrsyxcmggnyl"), false);
	this->jopecmwgevdfxotjqsgejwbku(504, false, true, 26447, 31583, string("nqjtitrkcejrdmmg"), 22521, 37023, 46, string("rhkhhpnihedmjmvvrfvfgnsuxejkirpnhlxibdegrioutixypepslhgmxvwejuwobegwrhfdayycp"));
	this->ymhwmsaerugppyfdjp(5778, 33577);
	this->grneiniofjrirmpbadxpkicjx(10605, 44118, 30565, true);
}
