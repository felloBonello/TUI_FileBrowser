#pragma once

#include <memory>
#include <functional>
#include <map>
#include "App.h"
#include "Events.h"
//#include "ObserverAndSubject.h"

using namespace TUI;

namespace TUI_File_Browser
{
	class WindowComponent;

	class Window {
	public:
		SHORT _windowHeight;
		SHORT _windowLength;
		COORD _startPoint;
		WORD _backgroundColor = TUI::B_BLACK;
		WORD _fontColor = TUI::F_WHITE;
		map<string, shared_ptr<WindowComponent*>> _componentList;
	public:
		Window() : _windowHeight(0), _windowLength(0), _startPoint({ 0, 0 }) {}
		Window(SHORT const& height, SHORT const& length, COORD const& startPoint = { 0, 0 }) : _windowHeight(height), _windowLength(length), _startPoint(startPoint) {}
		virtual ~Window() {}
		void SetWindowHeight(SHORT const& height) { _windowHeight = height; }
		void SetWindowLength(SHORT const& length) { _windowLength = length; }
		void SetStartPoint(COORD const& startPoint) { _startPoint = startPoint; }
		void SetBackgroundColor(WORD const& color) { _backgroundColor = color; }
		void SetFontColor(WORD const& color) { _fontColor = color; }

		SHORT GetWindowHeight() const { return _windowHeight; }
		SHORT GetWindowLength() const { return _windowLength; }
		COORD GetStartPoint() const { return _startPoint; }
		WORD GetBackgroundColor() const { return _backgroundColor; }
		WORD GetFontColor() const { return _fontColor; }

		void AttachComponent(std::string const& key, std::shared_ptr<WindowComponent*> component) { _componentList[key] = component; }

		std::map<std::string, std::shared_ptr<WindowComponent*>> GetComponents() const { return _componentList; }

		virtual void RouteKeyEvent(KeyEvent const& ke) = 0;
		virtual void RouteMouseEvent(MouseEvent const& me) = 0;
	};

	class WindowComponent {
	public:
		SHORT _componentHeight;
		SHORT _componentLength;
		COORD _startPoint;
		WORD _backgroundColor;
		WORD _fontColor;

	public:
		WindowComponent() {}
		virtual ~WindowComponent() {}

		SHORT GetComponentHeight() const { return _componentHeight; }
		SHORT GetComponentLength() const { return _componentLength; }
		COORD GetStartPoint() const { return _startPoint; }
		WORD GetBackgroundColor() const { return _backgroundColor; }
		WORD GetFontColor() const { return _fontColor; }

		void SetBackroundColor(WORD const& color) { _backgroundColor = color; }
		void SetFontColor(WORD const& color) { _fontColor = color; }

		virtual std::vector<std::string> RenderOutputString() = 0;
	};

	class ToggleButton : public WindowComponent {
	private:
		std::string _contentString;
		bool _toggled = false;
	public:
		ToggleButton(SHORT const& height, SHORT const& length, COORD const& startPoint) {
			_componentHeight = height;
			_componentLength = length;
			_startPoint = startPoint;
			_backgroundColor = TUI::B_RED;
			_fontColor = TUI::F_BLACK;
		}

		void Toggle() { _toggled = !_toggled; SetBackroundColor(_toggled ? TUI::B_GREEN : TUI::B_RED); }
		void SetContentString(std::string const& content) { _contentString = content; }
		std::string GetContentString() const { return _contentString; }

		bool IsToggled() const { return _toggled; }
		void SetToggled(bool const& isOn) { _toggled = isOn; }

		std::string GenerateStringRow(std::string const& content);
		std::vector<std::string> RenderOutputString() override;
	};

	class TextBox : public WindowComponent {
	private:
		std::string _label;
		std::string _text;
		WORD _labelForeground;
		WORD _labelBackground;
		size_t _outputPos;
	public:
		TextBox(SHORT const& length, COORD const& startPoint, std::string const& label = "", std::string const& text = "") : _label(label), _text(text), _labelBackground(TUI::B_YELLOW), _labelForeground(TUI::F_BLACK), _outputPos(0) {
			_componentHeight = 1;
			_componentLength = length;
			_startPoint = startPoint;
			_backgroundColor = TUI::B_BLACK;
			_fontColor = TUI::F_WHITE;
		}

		void SetLabel(std::string const& label) { _label = label; }
		std::string GetLabel() const { return _label; }

		void SetText(std::string const& text) { _text = text; }
		std::string GetText() const { return _text; }

		void SetLabelBackgroundColor(WORD const& color) { _labelBackground = color; }
		void SetLabelFontColor(WORD const& color) { _labelForeground = color; }

		WORD GetLabelBackgroundColor() const { return _labelBackground; }
		WORD GetLabelFontColor() const { return _labelForeground; }

		size_t GetStringPos() const { return _outputPos; }
		void SetStringPos(size_t const& pos) { _outputPos = pos; }

		std::vector<std::string> RenderOutputString() override;
	};

	class TextPad : public WindowComponent {
	private:
		std::vector<std::string> _results;
		size_t _startNum;
	public:
		TextPad(SHORT const& height, SHORT const& length, COORD const& startPoint, std::vector<std::string> const& results) : _results(results), _startNum(0) {
			_componentHeight = height;
			_componentLength = length;
			_startPoint = startPoint;
			_fontColor = TUI::F_BLACK;
			_backgroundColor = TUI::B_WHITE;
		}

		void SetResults(std::vector<std::string> const& results) { _results = results; }
		void SetStartNum(size_t const& start) { _startNum = start; }
		std::vector<std::string> RenderOutputString() override;

		size_t GetStartNum() const { return _startNum; }
		size_t GetResultsSize() const { return _results.size(); }
	};
}