#include "FileBrowserController.h"

namespace TUI_File_Browser
{
	void TextAreaEvent::ScrollUp(int numLines)
	{
		for (int i = 0; i < numLines; ++i)	//for number of lines scroll until at the beggining then return
			if (_view._begginIndex > 0)
				--_view._begginIndex;
			else
				return;
	}

	void TextAreaEvent::ScrollDown(int numLines)
	{
		for (int i = 0; i < numLines; ++i)  //for number of lines scroll until at the end then return
			if ((size_t)(_view._begginIndex + _view._height) < _view._content.size())
				++_view._begginIndex;
			else
				return;
	}

	void TextAreaEvent::ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e)
	{
		if (e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED	//if click is on scroll bar
			&& e.dwMousePosition.X == _view._location.X + _view._length - 1)
		{
			if (e.dwMousePosition.Y > _view._location.Y + _view._scrollBarLoc)
				ScrollDown((int)(_view._content.size() / 100));  //move down 1% of the lines in the content buffer
			if (e.dwMousePosition.Y < _view._location.Y + _view._scrollBarLoc)
				ScrollUp((int)(_view._content.size() / 100));  //move up 1% of the lines in the content buffer
			_view.Update();
		}
	}

	void TextAreaEvent::ExecuteKeyEvent(KEY_EVENT_RECORD const& e)
	{
		switch (e.wVirtualKeyCode)
		{
		case VK_UP: //arrow up
		case VK_NUMPAD8:
			ScrollUp(1);
			break;
		case VK_DOWN: //arrow down
		case VK_NUMPAD2:
			ScrollDown(1);
			break;
		case 33:  //page up
			ScrollUp(_view._height - 2);
			break;
		case 34:  //page down
			ScrollDown(_view._height - 2);
			break;
		}
		_view.Update();
	}


	void TextBoxEvent::ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e)
	{
		if (e.dwMousePosition.X > _view._location.X + _view._length - 1 && _view._text.length() > (size_t)_view._length)
			_view._cursorPos = _view._length - 1;   //put the cursor at end of text box
		else if (e.dwMousePosition.X < _view._location.X)
			_view._cursorPos = 0;					//put cursor at beggining
		else if ((size_t)e.dwMousePosition.X < _view._location.X + _view._text.length())
			_view._cursorPos = e.dwMousePosition.X - _view._location.X;	//put cursor at end of text
		else
			_view._cursorPos = (short)_view._text.length(); //put cursor at clicked location

		Win32ThickWrapper::Instance().SetCursor(COORD{ _view._cursorPos + _view._location.X, _view._location.Y }, 10);
	}

	void TextBoxEvent::ExecuteKeyEvent(KEY_EVENT_RECORD const& e)
	{
		char key = e.uChar.AsciiChar;
		if (key)
		{
			if (key != 127 && key != 8)				//if key isn't backspace or delete
			{
				_view._text.insert(_view._cursorPos + _view._begginIndex, string(1, key));
				if (_view._cursorPos + 1 < _view._length)
					++_view._cursorPos;
				else
					++_view._begginIndex;
			}
			else if (_view._cursorPos + _view._begginIndex > 0)	//if cursor isn't at beggining of textbox && key is backspace
			{
				_view._text.erase(_view._cursorPos + _view._begginIndex - 1, 1);
				if (_view._begginIndex == 0)
					--_view._cursorPos;
				else
					--_view._begginIndex;
			}
		}
		else
		{
			switch (e.wVirtualKeyCode)
			{
			case VK_LEFT:
			case VK_NUMPAD4:	//left arrow pressed
				if (_view._cursorPos == 0 && _view._begginIndex > 0)
					--_view._begginIndex;	//if cursor is at begging of text box and buffer isn't decrease text buffer
				if (_view._cursorPos > 0)
					--_view._cursorPos;		//if cursor pos isn't at beggining of buffer decrease cursor pos
				break;
			case VK_RIGHT:
			case VK_NUMPAD6:	//right arrow pressed
				if (_view._cursorPos == _view._length - 1 && (size_t)_view._begginIndex < (_view._text.length() - _view._length))
					++_view._begginIndex;	//if cursor is at end of textbox increase beggining position of text buffer
				if (_view._cursorPos < _view._length - 1 && (size_t)_view._cursorPos < _view._text.length())
					++_view._cursorPos;		//if cursor isn't at end of text increase cursor position
				break;
			default:
				break;
			}
		}

		Win32ThickWrapper::Instance().SetCursor(COORD{ _view._location.X + _view._cursorPos, _view._location.Y }, 10);
		_view.Update();
	}

	void CheckBoxEvent::ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e)
	{
		if (e.dwEventFlags == MOUSE_MOVED) //exit if mouse moved event
			return;
		_view._isChecked = !_view._isChecked; //toggle state
		_view.Update();
	}
	void CheckBoxEvent::ExecuteKeyEvent(KEY_EVENT_RECORD const& e) {}


	void ScanButtonEvent::ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e)
	{
		_model.RefreshModel(); //refresh model and observers
	}
	void ScanButtonEvent::ExecuteKeyEvent(KEY_EVENT_RECORD const& e) {}


	void ExitButtonEvent::ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e)
	{
		_model._done = true; //set finished flag to true
	}

	void ExitButtonEvent::ExecuteKeyEvent(KEY_EVENT_RECORD const& e) {}
}