#include "Components.h"

using namespace TUI;

namespace TUI_File_Browser
{
	Panel::Panel(COORD const& location, SHORT const& height, SHORT const& length, WORD const& bColor)
	{
		_height = height;
		_length = length;
		_location = location;
		_bColor = bColor;
		Update();
	}

	void Panel::Update() 
	{
		Win32ThickWrapper::Instance().FillColour(_bColor, _location, _height, _length);			
	}

	CheckBox::CheckBox(COORD const& location, SHORT const& height, SHORT const& length, bool &isChecked, WORD const& bColor, WORD const& fColor) : _isChecked(isChecked)
	{
		_height = height;
		_length = length;
		_location = location;
		_bColor = bColor;
		_fColor = fColor;
		Update();
	}

	Button::Button(COORD const& location, SHORT const& height, SHORT const& length, string const& content, WORD const& bColor, WORD const& fColor) 
	{
		_height = height;
		_length = length;
		_location = location;
		_content = content;
		_bColor = bColor;
		_fColor = fColor;
		Update();
	}

	void Button::Update() 
	{
		Win32ThickWrapper::Instance().FillColour(_bColor, _location, _height, _length);
		COORD contentLoc{ _location.X + (_length - (SHORT)_content.length())/2, _location.Y + _height / 2 };
		Win32ThickWrapper::Instance().WriteToConsole(_content, contentLoc, _fColor, _bColor);
	}

	Label::Label(COORD const& location, SHORT const& length, string &content, WORD const& bColor, WORD const& fColor) : _content(content)
	{
		_length = length;
		_location = location;
		_fColor = fColor;
		_bColor = bColor;
		Update();
	}

	void Label::Update() 
	{
		Win32ThickWrapper::Instance().WriteToConsole(string(_length, ' '), _location, _fColor, _bColor);
		Win32ThickWrapper::Instance().WriteToConsole(_content, _location, _fColor, _bColor);
	}

	void CheckBox::Update() 
	{
		string chkbox = _isChecked ? "X" : " ";
		Win32ThickWrapper::Instance().WriteToConsole(chkbox, _location, _fColor, _bColor);
	}

	TextBox::TextBox(COORD const& location, SHORT const& height, SHORT const& length, string &text, WORD const& bColor, WORD const& fColor) : _text(text)
	{
		_height = height;
		_length = length;
		_location = location;
		_cursorPos = (short)text.length();
		_begginIndex = 0;
		_bColor = bColor;
		_fColor = fColor;
		Update();
	}

	void TextBox::Update() 
	{
		string txtbox = string(_length, ' ');
		Win32ThickWrapper::Instance().WriteToConsole(txtbox, _location, _fColor, _bColor);

		txtbox = _text.substr(_begginIndex, _length);
		Win32ThickWrapper::Instance().WriteToConsole(txtbox, _location, _fColor, _bColor);
	}

	TextArea::TextArea(COORD const& location, SHORT const& height, SHORT const& length, vector<string> &content, WORD const& bColor, WORD const& fColor, WORD const& bScrollColor, WORD const& fScrollColor)
		: _content(content)
	{
		_height = height;
		_length = length;
		_location = location;
		_fColor = fColor;
		_bColor = bColor;
		_bScrollColor = bScrollColor;
		_fScrollColor = fScrollColor;
		_scrollBarLoc = 0;
		_begginIndex = 0;
		Update();
	}

	void TextArea::Update()
	{
		_scrollBarLoc = (short)round((double)(_height - 1) / (_content.size() - _height - 1) * _begginIndex);
		if (_scrollBarLoc == 0 && _begginIndex > 0)
			_scrollBarLoc = 1;
		if (_scrollBarLoc == _height - 1 && _begginIndex < (long)(_content.size() - _height))
			_scrollBarLoc = _height - 2;

		Win32ThickWrapper::Instance().FillColour(_bColor, _location, _height, _length);
		for (auto i = _begginIndex; i < _height + _begginIndex && i < (long)_content.size(); ++i)
			Win32ThickWrapper::Instance().WriteToConsole(_content.operator[](i), COORD{ _location.X, (short)(_location.Y + i - _begginIndex) }, _fColor, _bColor);
			

		for(short i = 0; i < _height; ++i)
			Win32ThickWrapper::Instance().WriteToConsole("|", COORD{ _location.X + _length - 1, _location.Y + i}, _fScrollColor, _bScrollColor);
		Win32ThickWrapper::Instance().WriteToConsole("O", COORD{ _location.X + _length - 1, _location.Y + _scrollBarLoc }, _fScrollColor, _bScrollColor);		
	}
}

