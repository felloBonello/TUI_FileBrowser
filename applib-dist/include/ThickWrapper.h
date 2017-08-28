#pragma once
#undef min
#include <ThinWrapper.h>
#include <vector>
#include <algorithm>
#include <memory>

#if defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-s.lib")
#endif



namespace TUI
{
	static WORD const F_BLACK = 0;
	static WORD const F_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	static WORD const F_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
	static WORD const F_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	static WORD const F_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static WORD const F_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
	static WORD const F_DARK_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	static WORD const F_DARK_RED = FOREGROUND_RED;
	static WORD const F_DARK_GREEN = FOREGROUND_GREEN;
	static WORD const F_DARK_BLUE = FOREGROUND_BLUE;
	static WORD const F_DARK_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN;
	static WORD const F_DARK_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN;
	static WORD const F_DARK_MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED;


	static WORD const B_BLACK = 0;
	static WORD const B_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	static WORD const B_RED = BACKGROUND_RED | BACKGROUND_INTENSITY;
	static WORD const B_GREEN = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_BLUE = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	static WORD const B_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	static WORD const B_MAGENTA = BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY;
	static WORD const B_DARK_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	static WORD const B_DARK_RED = BACKGROUND_RED;
	static WORD const B_DARK_GREEN = BACKGROUND_GREEN;
	static WORD const B_DARK_BLUE = BACKGROUND_BLUE;
	static WORD const B_DARK_YELLOW = BACKGROUND_RED | BACKGROUND_GREEN;
	static WORD const B_DARK_CYAN = BACKGROUND_BLUE | BACKGROUND_GREEN;
	static WORD const B_DARK_MAGENTA = BACKGROUND_BLUE | BACKGROUND_RED;

	class Win32ThickWrapper
	{
		typedef bool(*CtrlHandler)(unsigned long);

	private:
		struct ConsoleState {
			CONSOLE_SCREEN_BUFFER_INFO	CSBI;
			CONSOLE_CURSOR_INFO			CCI;
			vector<CHAR_INFO>			buffer;
			COORD						bufferCoord;
			DWORD						consoleMode;
			WORD						consoleWidth = 0;
			SMALL_RECT					bufferRect{ 0 };
			string						title;
		};

		Win32ThinWrapper Win32;	
		vector<INPUT_RECORD> buffer;

	public:
		ConsoleState console_state;
		static Win32ThickWrapper& Instance();

		Win32ThickWrapper();
		~Win32ThickWrapper();

		void SaveConsole();
		void RestoreConsole();
		void SetTitle(string title);
		void ResizeWindow(short width, short height);
		void SetControlHandler(CtrlHandler handler, bool add);
		void SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour);
		short GetConsoleWidth();
		short GetConsoleHeight();
		void SetBackgroundColour(unsigned short colour);
		void FillColour(unsigned short colour, COORD location, SHORT length, SHORT width);
		void WriteToConsole(string const& outputString, COORD const& startLocation, WORD const& foreground, WORD const& background);
		void SetCursor(COORD location, DWORD size);
		void HideCursor(bool hidden);
		vector<INPUT_RECORD> GetEvents();
	};

	
}

