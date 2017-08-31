#pragma once
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include <sys/stat.h>
#include <regex>
#include <set>

using namespace std;
using namespace std::tr2::sys;


namespace TUI_File_Browser
{
	class Subject;

	class Observer {
	public:
		virtual void Update() = 0;
	};

	class Subject {
		set<Observer*> observers;
	public:
		void Attach(Observer* p);
		void Detach(Observer* p);
		void Notify();
	};

	class FileBrowserModel : public Subject
	{
	public:
		string _dirStr;
		string _regexStr;
		string _foldersStr;
		string _filesStr;
		string _filesMatchedStr;
		string _filesMatchedSizeStr;
		string _statusMsg;

		path _directory;		
		regex _regex;		
		bool _isRecursive;
		vector<string> _results;
		int _folders;
		int _files;
		int _filesMatched;
		long long _filesMatchedSize;
		bool _done;
		


		FileBrowserModel(string directory, string regex, bool isRecursive);
		~FileBrowserModel() {}
		void SetRegex(string regex);
		void SetDirectory(string path);
		void RefreshModel();
		void DirParse(path const& dir, int tabCount);
		long GetFileSize(path const& dir);
	};
}
