#include "pch.h"
#include "defs.cpp"
#include "GetGameModuleInfo.h"
#include "DiamondCrossing.h"

#pragma comment(lib,"Version.lib")

#if _WIN64
#pragma comment(lib, "libMinHook.x64.lib")
bool Is64bit = true;
#else
#pragma comment(lib, "libMinHook.x86.lib")
bool Is64bit = false;
#endif

const short* IsDP = (short*)0x14241A894; // address of the byte that controls the current playstyle

DWORD WINAPI DecideVersion(LPVOID hModule)
{
    LPMODULEINFO mInfo = GetGameModuleInfo();

    if (mInfo == NULL)
    {
        std::cout << "module info is null" << std::endl;
    }
    else
    {
        //std::cout << mInfo->EntryPoint << std::endl;
        //std::cout << mInfo->lpBaseOfDll << std::endl;
        //std::cout << std::hex << mInfo->SizeOfImage << std::endl;
        std::string version = GetGameVersion(mInfo);
        StartSceneHooks(version, mInfo);
    }
    
    return EXIT_SUCCESS;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        DisableThreadLibraryCalls(hModule);
        MH_Initialize();

		CreateThread(NULL, NULL, DecideVersion, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {
        break;
	}
    }
    return TRUE;
}

