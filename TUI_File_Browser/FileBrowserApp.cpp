#include "FileBrowserApp.h"
using namespace TUI;

namespace TUI_File_Browser
{
	FileBrowserApp::FileBrowserApp() { }

	FileBrowserApp::~FileBrowserApp() { }

	void FileBrowserApp::IdentifyArgs()
	{
		bool recursive = true;
		string match = ".*";
		string dir = ".";

		for each (auto arg in GetArgs())
		{
			if (is_directory(arg))
				dir = arg;
			else if (arg == "-r")
				recursive = true;
			else
				match = arg;
		}

		model = make_shared<FileBrowserModel>(dir, match, recursive);
	}

	int FileBrowserApp::Execute()
	{
		Win32ThickWrapper console;
		console.SaveConsole();

		IdentifyArgs();

		console.SetTitle("File Browser");
		console.ResizeWindow(140, 30);
		console.HideCursor(true);
		console.SetBackgroundColour(B_WHITE);
		
		Panel ctrlPanel(COORD{ 0, 0 }, 3, console.GetConsoleWidth(), PANEL_B_COLOR);
		TextArea displayTxt(COORD{ 0, 3 }, console.GetConsoleHeight() - 6, console.GetConsoleWidth(), model->_results, CTRL_B_COLOR, CTRL_F_COLOR, SCROLLBAR_B_COLOR, SCROLLBAR_F_COLOR);
		Panel statsPanel(COORD{ 0, console.GetConsoleHeight() - 3 }, 3, console.GetConsoleWidth(), PANEL_B_COLOR);

		TextBox dirInput(COORD{ 19, 1 }, 1, 20, model->_dirStr, CTRL_B_COLOR, CTRL_F_COLOR);
		TextBox filter(COORD{ 49, 1 }, 1, 13, model->_regexStr, CTRL_B_COLOR, CTRL_F_COLOR);
		CheckBox isRecursive(COORD{ 75, 1 }, 1, 1, model->_isRecursive, CTRL_B_COLOR, CTRL_F_COLOR);
		
		Button exitBtn(COORD{ console.GetConsoleWidth() - 10, 0 }, 3, 6, "Exit", BTN_B_COLOR, BTN_F_COLOR);
		Button scanBtn(COORD{ console.GetConsoleWidth() - 20, 0 }, 3, 6, "Scan", BTN_B_COLOR, BTN_F_COLOR);

		Label dirLbl(COORD{ 3, 1 }, 15, string("Root Directory:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label filterLbl(COORD{ 41, 1 }, 7, string("Filter:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label recursiveLbl(COORD{ 64, 1 }, 10, string("Recursive:"), LABEL_B_COLOR, LABEL_F_COLOR);

		Label statusLbl(COORD{ 3, console.GetConsoleHeight() - 3 }, 7, string("Status:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label statusVal(COORD{ 12, console.GetConsoleHeight() - 3 }, console.GetConsoleWidth() - 12, model->_statusMsg, LABEL_B_COLOR, LABEL_F_COLOR);
		Label foldersLbl(COORD{ 3, console.GetConsoleHeight() - 2 }, 8, string("Folders:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label foldersVal(COORD{ 12, console.GetConsoleHeight() - 2 }, 10, model->_foldersStr, LABEL_B_COLOR, LABEL_F_COLOR);
		Label filesLbl(COORD{ 22, console.GetConsoleHeight() - 2 }, 6, string("Files:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label filesVal(COORD{ 29, console.GetConsoleHeight() - 2 }, 11, model->_filesStr, LABEL_B_COLOR, LABEL_F_COLOR);
		Label filesMatchedLbl(COORD{ 40, console.GetConsoleHeight() - 2 }, 14, string("Files Matched:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label filesMatchedVal(COORD{ 55, console.GetConsoleHeight() - 2 }, 15, model->_filesMatchedStr, LABEL_B_COLOR, LABEL_F_COLOR);
		Label matchedSizeLbl(COORD{ 70, console.GetConsoleHeight() - 2 }, 22, string("Size of Files Matched:"), LABEL_B_COLOR, LABEL_F_COLOR);
		Label filesMatchedSizeVal(COORD{ 93, console.GetConsoleHeight() - 2 }, console.GetConsoleWidth() - 93, model->_filesMatchedSizeStr, LABEL_B_COLOR, LABEL_F_COLOR);

		uiMap[&dirInput] = Command::ptr(new TextBoxEvent(dirInput));
		uiMap[&filter] = Command::ptr(new TextBoxEvent(filter));
		uiMap[&isRecursive] = Command::ptr(new CheckBoxEvent(isRecursive));
		uiMap[&displayTxt] = Command::ptr(new TextAreaEvent(displayTxt));
		uiMap[&scanBtn] = Command::ptr(new ScanButtonEvent(*model));
		uiMap[&exitBtn] = Command::ptr(new ExitButtonEvent(*model));

		observers = {
			&displayTxt, &statusVal, &foldersVal, &filesVal, & filesMatchedVal, &filesMatchedSizeVal
		};

		for (auto c : observers)
			model->Attach(c);

		while (!model->_done)
			EventHandler();

		console.RestoreConsole();
		return EXIT_SUCCESS;
	}

	void FileBrowserApp::EventHandler() 
	{
		vector<INPUT_RECORD> input = console.GetEvents();
		for(INPUT_RECORD r : input) 
		{
			switch (r.EventType)
			{
			case KEY_EVENT:
				KeyEventHandler(r.Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				MouseEventHandler(r.Event.MouseEvent);
				break;
			default:
				break;
			}
		}
	}

	void FileBrowserApp::MouseEventHandler(MOUSE_EVENT_RECORD const& e) 
	{
		if (e.dwEventFlags == MOUSE_MOVED && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && focusedComp)
		{
			uiMap[focusedComp]->ExecuteMouseEvent(e);
			return;
		}

		if (e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			console.HideCursor(true);
			focusedComp = nullptr;
			COORD mouseClicked = e.dwMousePosition;

			for (pair<Component*, Command::ptr> c : uiMap)
			{
				if (c.first->_location.Y <= mouseClicked.Y && mouseClicked.Y <= c.first->_location.Y + c.first->_height - 1
					&& c.first->_location.X <= mouseClicked.X && mouseClicked.X <= c.first->_location.X + c.first->_length - 1)
				{
					focusedComp = c.first;
					break;
				}
			}
			if (focusedComp)
			{
				uiMap[focusedComp]->ExecuteMouseEvent(e);
			}
		}

		if (e.dwEventFlags == MOUSE_WHEELED && focusedComp)
		{
			uiMap[focusedComp]->ExecuteMouseEvent(e);
			return;
		}		
	}

	void FileBrowserApp::KeyEventHandler(KEY_EVENT_RECORD const& e) 
	{
		if (focusedComp && e.bKeyDown) 
		{
			uiMap[focusedComp]->ExecuteKeyEvent(e);
		}
	}
}

