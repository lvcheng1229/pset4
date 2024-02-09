#include <cxxopts.hpp>
#include <iostream>

#include "core/Log.h"
#include "core/PsetThread.h"
#include "core\PtApplication.h"

int main(int argc, char* argv[])
{
	CPtApplication ptApp;
	ptApp.ParseCommnadLine(argc, argv);
	ptApp.Init();
	ptApp.Run();

    return 1;
}