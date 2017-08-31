#include <ThickWrapper.h>

namespace TUI
{

	Win32ThickWrapper* g_Win32 = nullptr;

	Win32ThickWrapper& Win32ThickWrapper::Instance()
	{
		static unique_ptr<Win32ThickWrapper> _instance(new Win32ThickWrapper);
		return *_instance;
	}

	Win32ThickWrapper::Win32ThickWrapper()
	{
		Win32ThinWrapper Win32;
		Win32.SetInputMode(ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);		

		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();
		buffer.resize(sbi.dwSize.X * sbi.dwSize.Y);
		g_Win32 = this;
	}

	Win32ThickWrapper::~Win32ThickWrapper()
	{
		g_Win32 = nullptr;

	}

	void Win32ThickWrapper::SaveConsole()
	{
		// Get the title
		console_state.title = Win32.GetTitle(64 * 1024);

		// Get the old window/buffer size
		console_state.CSBI = Win32.GetScreenBufferInfo();

		// Save the desktop
		console_state.buffer.resize(console_state.CSBI.dwSize.X*console_state.CSBI.dwSize.Y);
		console_state.bufferCoord = COORD{ 0 };
		console_state.bufferRect.Right = console_state.CSBI.dwSize.X - 1;
		console_state.bufferRect.Bottom = console_state.CSBI.dwSize.Y - 1;
		Win32.ReadOutput(console_state.buffer.data(), console_state.CSBI.dwSize, console_state.bufferCoord, &console_state.bufferRect);

		// Save the cursor
		console_state.CCI = Win32.GetCursorInfo();
	}

	void Win32ThickWrapper::RestoreConsole()
	{
		// Restore the original settings/size
		SMALL_RECT sr{ 0 };
		Win32.SetWindowInfo(TRUE, &sr);
		Win32.SetScreenBufferSize(console_state.CSBI.dwSize);
		Win32.SetWindowInfo(TRUE, &console_state.CSBI.srWindow);

		// Restore the desktop contents
		console_state.bufferRect = SMALL_RECT{ 0 };
		console_state.bufferRect.Right = console_state.CSBI.dwSize.X - 1;
		console_state.bufferRect.Bottom = console_state.CSBI.dwSize.Y - 1;
		Win32.WriteOutput(console_state.buffer.data(), console_state.CSBI.dwSize, console_state.bufferCoord, &console_state.bufferRect);
		Win32.SetTitle(console_state.title);

		// Restore the cursor
		Win32.SetCursorInfo(&console_state.CCI);
		Win32.SetCursorPosition(console_state.CSBI.dwCursorPosition);
	}

	void Win32ThickWrapper::SetTitle(string title)
	{
		Win32.SetTitle(title);
	}

	void Win32ThickWrapper::ResizeWindow(short width, short height)
	{
		SMALL_RECT sr{ 0 };
		Win32.SetWindowInfo(TRUE, &sr);

		COORD bufferSize;
		bufferSize.X = width;
		bufferSize.Y = height;
		Win32.SetScreenBufferSize(bufferSize);

		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();

		sr.Top = sr.Left = 0;
		width = min(width, sbi.dwMaximumWindowSize.X);
		height = min(height, sbi.dwMaximumWindowSize.Y);
		sr.Right = width - 1;
		sr.Bottom = height - 1;

		Win32.SetWindowInfo(TRUE, &sr);
	}

	void Win32ThickWrapper::SetControlHandler(CtrlHandler handler, bool add)
	{
		Win32.SetCtrlHandler((PHANDLER_ROUTINE)handler, add);
	}

	void Win32ThickWrapper::SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour)
	{
		CONSOLE_SCREEN_BUFFER_INFO sbi = Win32.GetScreenBufferInfo();
		COORD loc{ 0 };
		loc.X = min((short)x, sbi.dwMaximumWindowSize.X);
		loc.Y = min((short)y, sbi.dwMaximumWindowSize.Y);
		Win32.WriteOutputAttribute(&(WORD)colour, 1, loc);
	}

	void Win32ThickWrapper::SetBackgroundColour(unsigned short colour)
	{
		DWORD charsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi = Win32.GetScreenBufferInfo();
		DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
		COORD cursorHomeCoord{ 0, 0 };
		charsWritten = Win32.FillOutputCharacter(' ', consoleSize, cursorHomeCoord);
		charsWritten = Win32.FillOutputAttribute((WORD)colour, consoleSize, cursorHomeCoord);
	}

	short Win32ThickWrapper::GetConsoleWidth()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi = Win32.GetScreenBufferInfo();
		return csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}

	short Win32ThickWrapper::GetConsoleHeight()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi = Win32.GetScreenBufferInfo();
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	}

	void Win32ThickWrapper::FillColour(unsigned short colour, COORD location, SHORT height, SHORT length)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi = Win32.GetScreenBufferInfo();
		for (int i = 0; i < height; ++i)
		{
			Win32.FillOutputCharacter(' ', length, location);
			Win32.FillOutputAttribute(colour, length, location);
			location = COORD{location.X, location.Y + 1};
		}		
	}

	void Win32ThickWrapper::WriteToConsole(string const& outputString, COORD const& startLocation, WORD const& foreground, WORD const& background) {
		vector<WORD> outputAttributes(outputString.size(), foreground | background );
		Win32.WriteOutputCharacter(outputString.c_str(), (DWORD)outputString.length(), startLocation);
		Win32.WriteOutputAttribute(outputAttributes.data(), (DWORD)outputAttributes.size(), startLocation);
	}

	void Win32ThickWrapper::HideCursor(bool hidden)
	{
		auto newCCI = Win32.GetCursorInfo();
		newCCI.bVisible = !hidden;
		Win32.SetCursorInfo(&newCCI);
	}

	void Win32ThickWrapper::SetCursor(COORD location, DWORD size)
	{
		CONSOLE_CURSOR_INFO cci{ size, TRUE };
		Win32.SetCursorInfo(&cci);
		Win32.SetCursorPosition(location);
	}


	vector<INPUT_RECORD> Win32ThickWrapper::GetEvents()
	{
		vector<INPUT_RECORD> input;
		unsigned long numEvents = Win32.ReadInput(buffer);
		for (unsigned long iEv = 0; iEv < numEvents; ++iEv)
			input.push_back(buffer[iEv]);
		return input;
	}
};