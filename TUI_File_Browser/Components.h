#pragma once
#include <map>
#include <App.h>
#include "FileBrowserModel.h"

using namespace TUI;

namespace TUI_File_Browser
{
	class Component : public Observer{
	public:
		SHORT _height;
		SHORT _length;
		COORD _location;
		WORD _bColor;
		WORD _fColor;

		Component() {}
		virtual ~Component() {}
	};

	class Panel : public Component {
	public:
		Panel(COORD const& location, SHORT const& height, SHORT const& length, WORD const& bColor);
		void Update() override;
	};

	class CheckBox : public Component {
	public:
		bool &_isChecked;

		CheckBox(COORD const& location, SHORT const& height, SHORT const& length, bool &isChecked, WORD const& bColor, WORD const& fColor);
		void Update() override;
	};

	class Button : public Component {
	public:
		string _content;

		Button(COORD const& location, SHORT const& height, SHORT const& length, string const& content, WORD const& bColor, WORD const& fColor);
		void Click() { } 
		void Update() override;
	};

	class TextBox : public Component {
	public:
		short _cursorPos;
		short _begginIndex;
		string &_text;

		TextBox(COORD const& location, SHORT const& height, SHORT const& length, string &text, WORD const& bColor, WORD const& fColor);
		void Update() override;
	};

	class Label : public Component {
	public:

		string& _content;

		Label(COORD const& location, SHORT const& length, string &content, WORD const& bColor, WORD const& fColor);
		void Update() override;
	};

	class TextArea : public Component {
	public:
		short _scrollBarLoc;
		int _begginIndex;
		vector<string> &_content;
		WORD _bScrollColor;
		WORD _fScrollColor;

		TextArea(COORD const& location, SHORT const& height, SHORT const& length, vector<string> &content, WORD const& bColor, WORD const& fColor, WORD const& bScrollColor, WORD const& fScrollColor);
		void Update() override;
	};
}