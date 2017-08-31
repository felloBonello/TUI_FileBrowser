#pragma once
#include <App.h>
#include "Components.h"

using namespace std;

namespace TUI_File_Browser
{
	class Command {
	public:
		using ptr = shared_ptr<Command>;

		virtual void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) = 0;
		virtual void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) = 0;
	};


	class TextAreaEvent : public Command {
		TextArea& _view; 
	public:
		TextAreaEvent(TextArea& view) : _view(view) {}
		void ScrollUp(int numLines);
		void ScrollDown(int numLines);
		void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) override;
		void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) override;
	};


	class TextBoxEvent : public Command {
		TextBox& _view;
	public:
		TextBoxEvent(TextBox& view) : _view(view) {}
		void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) override;
		void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) override;
	};


	class CheckBoxEvent : public Command {
		CheckBox& _view;
	public:
		CheckBoxEvent(CheckBox& view) : _view(view) {}
		void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) override;
		void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) override;
	};


	class ScanButtonEvent : public Command {
		FileBrowserModel& _model;
	public:
		ScanButtonEvent(FileBrowserModel& model) : _model(model) {}
		void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) override;
		void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) override;
	};


	class ExitButtonEvent : public Command {
		FileBrowserModel& _model;
	public:
		ExitButtonEvent(FileBrowserModel& model) : _model(model) {}
		void ExecuteMouseEvent(MOUSE_EVENT_RECORD const& e) override;
		void ExecuteKeyEvent(KEY_EVENT_RECORD const& e) override;
	};
}
