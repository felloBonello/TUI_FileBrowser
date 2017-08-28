#include <App.h>

#ifdef _DEBUG
	//must define both _CRTDBG_MAP_ALLOC and _CRTDBG_MAP_ALLOC_NEW
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW

	#include <stdlib.h>
	#include <crtdbg.h>
	//if you won't use this macro you'll get all new as called from crtdbg.h      
	#define DEBUG_NEW   new( _CLIENT_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif



int main(int argc, char* argv[]) try
{
	return App::main(argc, argv);
}
catch (TUI::XError const& e)
{
	cerr << "TUI::consoleException: " << e.msg() << std::endl;
	return EXIT_FAILURE;
}
catch (std::exception const& e)
{
	cerr << "std::exception: " << e.what() << endl;
	return EXIT_FAILURE;
}
catch (...)
{
	cerr << "Error: an exception has been caught...\n";
	return EXIT_FAILURE;
}

int App::main(int argc, char* argv[])
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	for (int i = 1; i < argc; ++i)
		thisApp->_args.push_back(argv[i]);

	int res = thisApp->Execute();
	return res;
}

App* App::thisApp = nullptr;

App::App()
{
	if (thisApp)
		throw std::exception("Error: App already initialized");
	thisApp = this;
}

int App::Execute() {
	cout << "App console framework, (c) us, 2017\n";
	return EXIT_SUCCESS;
}

