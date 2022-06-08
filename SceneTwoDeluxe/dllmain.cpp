#include "pch.h"
#include "GetGameModuleInfo.h"
#include "DiamondCrossing.h"
#include "ParseINI.h"

#pragma comment(lib,"Version.lib")

#if _WIN64
#pragma comment(lib, "libMinHook.x64.lib")
#else
#pragma comment(lib, "libMinHook.x86.lib")
#endif

boost::property_tree::ptree pt;
/*
DWORD WINAPI DecideVersion(LPVOID hModule)
{
    LPCWSTR modName = nullptr;
    modName = GetCurrentModuleName();
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
        if (version == "NOT FOUND")
        {
			std::cout << "Game version not found." << std::endl;
		}
		else
		{
            StartSceneHooks(version, mInfo, modName);
        }
    }
    
    return EXIT_SUCCESS;
}
*/
void BeginHooks(std::map<Scene, std::string> sceneMap)
{
	// try to get current game module
    LPCWSTR modName = nullptr;
	modName = GetCurrentModuleName();
    if (modName == NULL)
    {
        return;
    }
	
	// try to get current game module info
    LPMODULEINFO mInfo = GetGameModuleInfo();
    if (mInfo == NULL)
    {
        return;
    }

	// try to get current game version
	std::string version = GetGameVersion(mInfo);
	if (version == "NOT FOUND")
	{
		std::cout << "Game version not found." << std::endl;
	}
	else
	{
		// start scene hooks
		StartSceneHooks(version, mInfo, modName, pt.get<std::string>("Main.obs-address"), sceneMap);
	}
}

DWORD WINAPI Initialize(LPVOID hModule)
{
    std::map<Scene, std::string> sceneMap;
    try
    {
        // get current directory
        LPWSTR path = new WCHAR[MAX_PATH];
        GetModuleFileName((HMODULE)hModule, path, MAX_PATH);
        std::string pathStr = std::string(CW2A(path));
        // replace the end of the path to point to the .ini
        pathStr.replace(pathStr.length() - 18, 18, "SceneTwoDeluxe.ini");
        //std::cout << "Path replaced: " << pathStr << std::endl;
 
        boost::property_tree::ini_parser::read_ini(pathStr, pt);
        AssertINI(pt);
        sceneMap = CreateSceneMap(pt);
        if (pt.get<std::string>("Main.enable-debug") == "true")
		{
            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
            std::cout << "Debug mode enabled." << std::endl;
		}
    }
    catch (std::exception e)
    {
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        std::cout << e.what() << std::endl;
        std::cout << "SceneTwoDeluxe.ini is missing or invalid." << std::endl;
        return EXIT_SUCCESS;
    }

    BeginHooks(sceneMap);

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
        DisableThreadLibraryCalls(hModule);
        MH_Initialize();

		//CreateThread(NULL, NULL, DecideVersion, NULL, NULL, NULL);
		CreateThread(NULL, NULL, Initialize, hModule, NULL, NULL);
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

