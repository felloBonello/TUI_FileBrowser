#pragma once
#include <App.h>
#include "FileBrowserModel.h"
#include "Components.h"
#include "FileBrowserController.h"
using namespace TUI;

namespace TUI_File_Browser
{
	static const short PANEL_B_COLOR = TUI::B_DARK_RED;
	static const short SCROLLBAR_B_COLOR = TUI::B_DARK_BLUE;
	static const short SCROLLBAR_F_COLOR = TUI::F_GREEN;
	static const short LABEL_B_COLOR = TUI::B_DARK_RED;
	static const short LABEL_F_COLOR = TUI::F_CYAN;
	static const short CTRL_B_COLOR = TUI::B_BLACK;
	static const short CTRL_F_COLOR = TUI::F_WHITE;
	static const short BTN_F_COLOR = TUI::F_WHITE;
	static const short BTN_B_COLOR = TUI::B_BLUE;

	class FileBrowserApp : public App
	{
	public:
		vector<Component*> observers;
		Component* focusedComp;
		Win32ThickWrapper console;
		shared_ptr<FileBrowserModel> model;
		map<Component*, Command::ptr> uiMap;

		void IdentifyArgs();

		FileBrowserApp();
		~FileBrowserApp();
		int Execute() override;

		void EventHandler();
		void MouseEventHandler(MOUSE_EVENT_RECORD const& me);
		void KeyEventHandler(KEY_EVENT_RECORD const& e);

	} FileBrowserApp;
}

