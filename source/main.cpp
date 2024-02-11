#include <cxxopts.hpp>
#include <iostream>

#include "core/Log.h"
#include "core/PsetThread.h"
#include "core\PtApplication.h"

int main(int argc, char* argv[])
{
	CPtApplication* pPtApplication = GetPtApplication();
	pPtApplication->ParseCommnadLine(argc, argv);
	pPtApplication->Init();
	pPtApplication->Run();

    return 1;
}