#pragma once
#include "App.h"

namespace TUI_File_Browser
{
	class KeyEvent {
		KEY_EVENT_RECORD _keyEvent;
		bool _handled;
	public:
		KeyEvent(INPUT_RECORD const& ir) : _keyEvent(ir.Event.KeyEvent), _handled(false) {}
		bool KeyDown() const { return _keyEvent.bKeyDown == TRUE; }
		char AsciiChar() const { return _keyEvent.uChar.AsciiChar; }
		unsigned short VirtualKeyCode() const { return _keyEvent.wVirtualKeyCode; }
		unsigned short VirtualScanCode() const { return _keyEvent.wVirtualScanCode; }
		bool CapsLock() const { return _keyEvent.dwControlKeyState == CAPSLOCK_ON; }
		bool EnhancedKey() const { return _keyEvent.dwControlKeyState == ENHANCED_KEY; }
		bool LeftAlt() const { return _keyEvent.dwControlKeyState == LEFT_ALT_PRESSED; }
		bool RightAlt() const { return _keyEvent.dwControlKeyState == RIGHT_ALT_PRESSED; }
		bool LetCtrl() const { return _keyEvent.dwControlKeyState == LEFT_CTRL_PRESSED; }
		bool RightCtrl() const { return _keyEvent.dwControlKeyState == RIGHT_CTRL_PRESSED; }
		bool NumLock() const { return _keyEvent.dwControlKeyState == NUMLOCK_ON; }
		bool ScrollLockOn() const { return _keyEvent.dwControlKeyState == SCROLLLOCK_ON; }
		bool ShiftPressed() const { return _keyEvent.dwControlKeyState == SHIFT_PRESSED; }
	};

	class MouseEvent {
	public:
		MOUSE_EVENT_RECORD _mouseEvent;

		MouseEvent(INPUT_RECORD const& ir) : _mouseEvent(ir.Event.MouseEvent) {}
		COORD GetMouseCoord() const { return _mouseEvent.dwMousePosition; }
		bool MouseLeftClick() const;
		bool MouseRightClick() const;
		bool MouseVerticalScroll() const;
	};

	class Event {
		INPUT_RECORD _inputRecord;
	public:
		enum class Type { KEYBOARD = KEY_EVENT, MOUSE = MOUSE_EVENT };
		Event(INPUT_RECORD const& ir) : _inputRecord(ir) { _eventType = GenerateEventType(); }
		KeyEvent GetKeyEvent() const { return KeyEvent(_inputRecord); }
		MouseEvent GetMouseEvent() const { return MouseEvent(_inputRecord); }
		Type GetType() const { return _eventType; }
	private:
		Type _eventType;
		Type GenerateEventType() const;
	};
}

