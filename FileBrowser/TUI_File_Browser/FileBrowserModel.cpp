#include "FileBrowserModel.h"

namespace TUI_File_Browser
{
	void Subject::Attach(Observer* p) { observers.insert(p); }
	void Subject::Detach(Observer* p) { observers.erase(p); }

	void Subject::Notify() 
	{
		for (auto const& observer : observers)
			observer->Update();
	}


	FileBrowserModel::FileBrowserModel(string directory, string regexStr, bool isRecursive)
	{
		_done = false;
		SetRegex(regexStr);
		SetDirectory(directory);
		_isRecursive = isRecursive;
		RefreshModel();
	}

	void FileBrowserModel::SetDirectory(string dir)
	{
		_dirStr = dir;
		_directory = path(dir);
	}

	void FileBrowserModel::SetRegex(string regexStr)
	{
		_regexStr = regexStr;
		_regex = regex(regexStr);
	}

	void FileBrowserModel::RefreshModel()
	{
		_statusMsg = "Parsing directory..";
		_folders = 0;
		_files = 0;
		_filesMatched = 0;
		_filesMatchedSize = 0;
		_foldersStr = "0";
		_filesStr = "0";
		_filesMatchedStr = "0";
		_filesMatchedSizeStr = "0";
		_results.clear();

		Notify();

		SetDirectory(_dirStr);
		SetRegex(_regexStr);
		DirParse(_directory, 0);

		_statusMsg = "Directory has been parsed successfully!";
		_foldersStr = to_string(_folders);
		_filesStr = to_string(_files);
		_filesMatchedStr = to_string(_filesMatched);
		_filesMatchedSizeStr = to_string(_filesMatchedSize);

		Notify();
	}

	long FileBrowserModel::GetFileSize(path const& dir)
	{
		struct stat filestatus;
		stat(dir.string().c_str(), &filestatus);
		return filestatus.st_size;
	}

	void FileBrowserModel::DirParse( path const& dir, int tabCount)
	{
		try
		{
			directory_iterator end_itr;
			for (directory_iterator dirIte(dir); dirIte != end_itr; ++dirIte)
			{
				string name = dirIte->path().filename().string();
				path curDir(dirIte->path());
				if (!is_directory(dirIte->status()))
				{	
					++_files;
					if (regex_match(name, _regex))
					{
						_results.push_back(string((tabCount * 2), ' ') + name);
						++_filesMatched;
						_filesMatchedSize += GetFileSize(curDir);
					}				
				}
				else
				{

					++_folders;
					_results.push_back(string((tabCount * 2), ' ') + "\\" + name);
					if (_isRecursive)
						DirParse(curDir, tabCount + 1);
				}
			}
		}
		catch (...)
		{
			//to-do
		}
	}
}
