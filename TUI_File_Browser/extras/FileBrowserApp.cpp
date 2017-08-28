#include "FileBrowserApp.h"


namespace TUI_File_Browser
{

	FileBrowser::FileBrowser() {//bool const& isRecursive, std::string const& regex, std::string filepath) {

		//SetCursorInfo(false);
		//SetWindowTitle(L"FileBrowser - Ryan Slipetz");
		//SetInputMode(Console::MOUSE_INPUT | Console::PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_EXTENDED_FLAGS);
		//_model = std::make_shared<Subject*>(new FileBrowserModel(isRecursive, regex, filepath));

		ModelChangeWindow changeWindow(this);
		ModelStatsWindow statsWindow(this);
		ModelDisplayWindow displayWindow(this);

		//(*_model)->Attach(std::make_shared<Observer*>(new ModelStatsWindow(statsWindow)));
		//(*_model)->Attach(std::make_shared<Observer*>(new ModelDisplayWindow(displayWindow)));
		//(*_model)->Attach(std::make_shared<Observer*>(new ModelChangeWindow(changeWindow)));

		_views.push_back(std::make_shared<Window*>(new ModelChangeWindow(changeWindow)));
		_views.push_back(std::make_shared<Window*>(new ModelDisplayWindow(displayWindow)));
		_views.push_back(std::make_shared<Window*>(new ModelStatsWindow(statsWindow)));

		//_focusedWindow = std::make_shared<Window*>(new ModelDisplayWindow(displayWindow));
		//(*_model)->UpdateObservers();

		RenderWindows();
	}
}

