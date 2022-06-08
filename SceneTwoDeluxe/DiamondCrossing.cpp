#include "pch.h"
#include "DiamondCrossing.h"
#include "WSClient.h"
#include "ParseINI.h"

boost::asio::io_context io;

LPMODULEINFO CurrentModuleInfo;
bool* isDP;

std::map<Scene, std::string> sM;

LPCWSTR currentModuleName;
void(*SceneSwitch)(unsigned int sceneID) = nullptr;

// works for IIDX: 2018091900, 2020092900
char (__fastcall* OnSceneSwitch_orig25)(void* a1, unsigned int sceneID, __int64 a3, char a4) = nullptr;
char __fastcall OnSceneSwitch_hook25(void* a1, unsigned int sceneID, __int64 a3, char a4)
{
	SceneSwitch(sceneID);
	return OnSceneSwitch_orig25(a1, sceneID, a3, a4);
}

// works for IIDX: 2021091500
char(__fastcall* OnSceneSwitch_orig28)(void* a1, unsigned int sceneID, __int64 a3) = nullptr;
char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3)
{
	SceneSwitch(sceneID);
	return OnSceneSwitch_orig28(a1, sceneID, a3);
}

// works for SDVX: 2021121400, 2022042500, 2022052400
// works for SDVXEAC: 2022042600, 2022053103
char(__fastcall* OnSceneSwitch_origSDVX6)(void* a1, unsigned int sceneID, __int64 a3, char a4, char a5) = nullptr;
char __fastcall OnSceneSwitch_hookSDVX6(void* a1, unsigned int sceneID, __int64 a3, char a4, char a5)
{
	SceneSwitch(sceneID);
	return OnSceneSwitch_origSDVX6(a1, sceneID, a3, a4, a5);
}

void DelayForMs(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void SceneSwitchTemp(unsigned int sceneID)
{
	std::cout << std::hex << sceneID << std::endl;
}

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo, LPCWSTR modName, std::string uri, std::map<Scene, std::string> sceneMap)
{
	currentModuleName = modName;
	CurrentModuleInfo = mInfo;
	sM = sceneMap;
	
	// IIDX25 2018091900
	if (version.substr(10, 10) == "2018091900")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchIIDX_2018091900;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookIIDX(0x128D70, 0x231B6E4, OnSceneSwitch_hook25, &OnSceneSwitch_orig25);
	}
	
	// IIDX27 2020092900
	else if (version.substr(10, 10) == "2020092900")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchIIDX_2020092900;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookIIDX(0x5C6210, 0x4327714, OnSceneSwitch_hook25, &OnSceneSwitch_orig25);
	}
	
	// IIDX28 2021091500
	else if (version.substr(10,10) == "2021091500")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchIIDX_2021091500;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookIIDX(0x782E80, 0x50A11C4, OnSceneSwitch_hook28, &OnSceneSwitch_orig28);
	}

	// SDVX6 2021121400
	else if (version.substr(10, 10) == "2021121400")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchSDVX_2021121400;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookSDVX(0x43B950, OnSceneSwitch_hookSDVX6, &OnSceneSwitch_origSDVX6);
	}
	
	// SDVX6 2022042500
	else if (version.substr(10, 10) == "2022042500")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchSDVX_2022042500;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookSDVX(0x484E40, OnSceneSwitch_hookSDVX6, &OnSceneSwitch_origSDVX6);
	}
	
	// SDVX6 2022052400
	else if (version.substr(10, 10) == "2022052400")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchSDVX_2022052400;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookSDVX(0x48B1B0, OnSceneSwitch_hookSDVX6, &OnSceneSwitch_origSDVX6);
	}

	// SDVX eac QCV:J:B:A:2022042600
	else if (version.substr(10, 10) == "2022042600")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchSDVXEAC_2022042600;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookSDVX(0x39F920, OnSceneSwitch_hookSDVX6, &OnSceneSwitch_origSDVX6);
	}
	
	// SDVX eac QCV:J:B:A:2022053103
	else if (version.substr(10, 10) == "2022053103")
	{
		std::cout << "Found supported version: " << version << std::endl;
		SceneSwitch = &SceneSwitchSDVXEAC_2022042600;
		auto task = std::async(std::launch::async, RunServer, uri);
		EnableHookSDVX(0x3BA460, OnSceneSwitch_hookSDVX6, &OnSceneSwitch_origSDVX6);
	}
	
	else
	{
		std::cout << "Found unsupported version: " << version << std::endl;
	}
}

void EnableHookIIDX(uintptr_t sceneSwitchAddr, uintptr_t isDPAddr, void* onSceneSwitchHook, void* OnSceneSwitchOrig)
{
	isDP = (bool*)((uintptr_t)CurrentModuleInfo->lpBaseOfDll + isDPAddr);
	uintptr_t addrAfterOffset = (uintptr_t)CurrentModuleInfo->lpBaseOfDll + sceneSwitchAddr;

	MH_CreateHook(
		reinterpret_cast<void*>(addrAfterOffset),
		reinterpret_cast<void*>(onSceneSwitchHook),
		reinterpret_cast<void**>(OnSceneSwitchOrig)
	);
	MH_EnableHook(MH_ALL_HOOKS);

	std::cout << "Hooks enabled" << std::endl;
}

void EnableHookSDVX(uintptr_t sceneSwitchAddr, void* onSceneSwitchHook, void* OnSceneSwitchOrig)
{
	uintptr_t addrAfterOffset = (uintptr_t)CurrentModuleInfo->lpBaseOfDll + sceneSwitchAddr;

	MH_CreateHook(
		reinterpret_cast<void*>(addrAfterOffset),
		reinterpret_cast<void*>(onSceneSwitchHook),
		reinterpret_cast<void**>(OnSceneSwitchOrig)
	);
	MH_EnableHook(MH_ALL_HOOKS);

	std::cout << "Hooks enabled" << std::endl;
}


void SceneSwitchIIDX_2018091900(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x14:
	{
		std::cout << "Music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x15:
	{
		std::cout << "Result" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_RESULT]);
		break;
	}
	case 0x3D:
	{
		if (*isDP)
		{
			std::cout << "DP pfree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP pfree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x37:
	{
		if (*isDP)
		{
			std::cout << "DP standard stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP standard stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x3C:
	{
		if (*isDP)
		{
			std::cout << "DP step up stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP step up stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x34:
	{
		if (*isDP)
		{
			std::cout << "DP dan stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP danstage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x35:
	{
		if (*isDP)
		{
			std::cout << "DP expert stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP expert stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x38:
	{
		if (*isDP)
		{
			std::cout << "DP free stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP free stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x3E:
	{
		if (*isDP)
		{
			std::cout << "DP arena stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP arena stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x25:
	{
		std::cout << "Arena podium" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_PODIUM]);
		break;
	}
	case 0x24:
	{
		std::cout << "Arena before song" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_BEFORE_SONG]);
		break;
	}
	case 0x23:
	{
		std::cout << "Arena music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x22:
	{
		std::cout << "Arena lobby" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_LOBBY]);
		break;
	}
	case 0x19:
	{
		std::cout << "Expert result" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_EXPERT_RESULT]);
		break;
	}
	case 0x18:
	{
		std::cout << "Expert select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_EXPERT_SELECT]);
		break;
	}
	case 0x1F:
	{
		std::cout << "Gekisou racer" << std::endl;
		break;
	}
	case 0xE:
	{
		std::cout << "Card out" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_OUT]);
		break;
	}
	case 0x17:
	{
		std::cout << "Dan result" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_RESULT]);
		break;
	}
	case 0x16:
	{
		std::cout << "Dan select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_SELECT]);
		break;
	}
	case 0x1C:
	{
		std::cout << "Step up" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x43:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TEST_MENU]);
		break;
	}
	case 0x27:
	{
		std::cout << "Attract screen" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ATTRACT]);
		break;
	}
	case 0x26:
	{
		std::cout << "Title screen" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TITLE]);
		break;
	}
	case 0xF:
	{
		std::cout << "Card in" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_IN]);
		break;
	}
	case 0x11:
	{
		std::cout << "Mode select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MODE_SELECT]);
		break;
	}
	case 0x13:
	{
		std::cout << "DJVIP pass" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DJVIP_PASS_SELECT]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
}

void SceneSwitchIIDX_2020092900(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x14:
	{
		std::cout << "Music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x3D:
	{
		if (*isDP)
		{
			std::cout << "DP pfree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP pfree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x37:
	{
		if (*isDP)
		{
			std::cout << "DP standard stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP standard stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x3E:
	{
		if (*isDP)
		{
			std::cout << "DP arena stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP arena stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x15:
	{
		std::cout << "Result" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_RESULT]);
		break;
	}
	case 0x24:
	{
		std::cout << "Arena before song" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_BEFORE_SONG]);
		break;
	}
	case 0x23:
	{
		std::cout << "Arena music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x25:
	{
		std::cout << "Arena podium" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_PODIUM]);
		break;
	}
	case 0x22:
	{
		std::cout << "Arena lobby" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_LOBBY]);
		break;
	}
	case 0x38:
	{
		if (*isDP)
		{
			std::cout << "DP dree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP dree stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x34:
	{
		if (*isDP)
		{
			std::cout << "DP dan stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP dan stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0xE:
	{
		std::cout << "Card out" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_OUT]);
		break;
	}
	case 0x17:
	{
		std::cout << "Dan result" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_RESULT]);
		break;
	}
	case 0x16:
	{
		std::cout << "Dan select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_SELECT]);
		break;
	}
	case 0x11:
	{
		std::cout << "Mode select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MODE_SELECT]);
		break;
	}
	case 0x3C:
	{
		if (*isDP)
		{
			std::cout << "DP step up stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
			break;
		}
		else
		{
			std::cout << "SP step up stage" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
			break;
		}
	}
	case 0x12:
	{
		std::cout << "DJVIP pass select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DJVIP_PASS_SELECT]);
		break;
	}
	case 0x1C:
	{
		std::cout << "Step up" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x27:
	{
		std::cout << "Attract screen" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ATTRACT]);
		break;
	}
	case 0x26:
	{
		std::cout << "Title screen" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TITLE]);
		break;
	}
	case 0x43:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TEST_MENU]);
		break;
	}
	case 0xF:
	{
		std::cout << "Card in" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_IN]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
}

void SceneSwitchIIDX_2021091500(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x15:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x43:
	{
		if (*isDP)
		{
			std::cout << "PFree DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "PFree SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x3E:
	{
		if (*isDP)
		{
			std::cout << "Free DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "Free SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x3D:
	{
		if (*isDP)
		{
			std::cout << "Standard DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "Standard SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x16:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_RESULT]);
		break;
	}
	case 0x36:
	{
		std::cout << "Monitor check scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MONITOR_CHECK]);
		break;
	}
	case 0x2C:
	{
		std::cout << "Attract screen scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ATTRACT]);
		break;
	}
	case 0x2B:
	{
		std::cout << "Title screen scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TITLE]);
		break;
	}
	case 0x10:
	{
		std::cout << "Card in scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_IN]);
		break;
	}
	case 0x12:
	{
		std::cout << "Mode select scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MODE_SELECT]);
		break;
	}
	case 0x4A:
	{
		std::cout << "Test menu scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_TEST_MENU]);
		break;
	}
	case 0x1D:
	{
		std::cout << "Step up music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x42:
	{
		if (*isDP)
		{
			std::cout << "Step up DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "Step up SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x17:
	{
		std::cout << "Dan select stage" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_SELECT]);
		break;
	}
	case 0x3A:
	{
		if (*isDP)
		{
			std::cout << "Dan DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "Dan SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x18:
	{
		std::cout << "Dan result scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DAN_RESULT]);
		break;
	}
	case 0xF:
	{
		std::cout << "Card out scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_CARD_OUT]);
		break;
	}
	case 0x23:
	{
		std::cout << "Arena lobby scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_LOBBY]);
		break;
	}
	case 0x24:
	{
		std::cout << "Arena music select scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x25:
	{
		std::cout << "Arena before song starts" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_BEFORE_SONG]);
		break;
	}
	case 0x44:
	{
		if (*isDP)
		{
			std::cout << "Arena DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "Arena SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x26:
	{
		std::cout << "Arena podium scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ARENA_PODIUM]);
		break;
	}
	case 0x27:
	{
		std::cout << "BPL lobby" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_BPL_LOBBY]);
		break;
	}
	case 0x28:
	{
		std::cout << "BPL music select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_MUSIC_SELECT]);
		break;
	}
	case 0x29:
	{
		std::cout << "BPL before song starts" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_BPL_BEFORE_SONG]);
		break;
	}
	case 0x45:
	{
		if (*isDP)
		{
			std::cout << "BPL DP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_DP_STAGE]);
		}
		else
		{
			std::cout << "BPL SP stage scene" << std::endl;
			SendSwitchScene(sM[Scene::IIDX_SP_STAGE]);
		}
		break;
	}
	case 0x2A:
	{
		std::cout << "BPL podium scene" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_BPL_PODIUM]);
		break;
	}
	case 0x14:
	{
		std::cout << "DJVIP pass select" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DJVIP_PASS_SELECT]);
		break;
	}
	case 0x32:
	{
		std::cout << "Attract loop tutorial" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_ATTRACT_TUTORIAL]);
		break;
	}
	case 0x2F:
	{
		std::cout << "Demo play" << std::endl;
		SendSwitchScene(sM[Scene::IIDX_DEMO_PLAY]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
	
}


void SceneSwitchSDVX_2021121400(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x11:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_MUSIC_SELECT], sM[Scene::SDVX_MUSIC_SELECT_DELAY]);
		break;
	}
	case 0x2B:
	{
		std::cout << "Stage scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_STAGE], sM[Scene::SDVX_STAGE_DELAY]);
		break;
	}
	case 0xF:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_RESULT], sM[Scene::SDVX_RESULT_DELAY]);
		break;
	}
	case 0xD:
	{
		std::cout << "Attract screen" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_ATTRACT]);
		break;
	}
	case 0xC:
	{
		std::cout << "Title screen" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TITLE]);
		break;
	}
	case 0x2D:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TEST_MENU]);
		break;
	}
	case 0x23:
	{
		std::cout << "Course select" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_SELECT]);
		break;
	}
	case 0x10:
	{
		std::cout << "Course result" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_RESULT]);
		break;
	}
	default:
	{
		std::cout << "SceneID: " << std::hex << sceneID << std::endl;
	}
	}
}

void SceneSwitchSDVX_2022042500(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x12:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_MUSIC_SELECT], sM[Scene::SDVX_MUSIC_SELECT_DELAY]);
		break;
	}
	case 0x2C:
	{
		std::cout << "Stage scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_STAGE], sM[Scene::SDVX_STAGE_DELAY]);
		break;
	}
	case 0x10:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_RESULT], sM[Scene::SDVX_RESULT_DELAY]);
		break;
	}
	case 0x2E:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TEST_MENU]);
		break;
	}
	case 0x24:
	{
		std::cout << "Course select" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_SELECT]);
		break;
	}
	case 0x11:
	{
		std::cout << "Course result" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_RESULT]);
		break;
	}
	case 0xE:
	{
		std::cout << "Attract" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_ATTRACT]);
		break;
	}
	case 0xD:
	{
		std::cout << "Title" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TITLE]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
}

void SceneSwitchSDVX_2022052400(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x12:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_MUSIC_SELECT], sM[Scene::SDVX_MUSIC_SELECT_DELAY]);
		break;
	}
	case 0x2C:
	{
		std::cout << "Stage scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_STAGE], sM[Scene::SDVX_STAGE_DELAY]);
		break;
	}
	case 0x10:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_RESULT], sM[Scene::SDVX_RESULT_DELAY]);
		break;
	}
	case 0x2F:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TEST_MENU]);
		break;
	}
	case 0x24:
	{
		std::cout << "Course select" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_SELECT]);
		break;
	}
	case 0x11:
	{
		std::cout << "Course result" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_RESULT]);
		break;
	}
	case 0xE:
	{
		std::cout << "Attract" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_ATTRACT]);
		break;
	}
	case 0xD:
	{
		std::cout << "Title" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TITLE]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
}

// also works for: 2022053103
void SceneSwitchSDVXEAC_2022042600(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x10:
	{
		std::cout << "Music select" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_MUSIC_SELECT], sM[Scene::SDVX_MUSIC_SELECT_DELAY]);
		break;
	}
	case 0x29:
	{
		std::cout << "Stage scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_STAGE], sM[Scene::SDVX_STAGE_DELAY]);
		break;
	}
	case 0xE:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_RESULT], sM[Scene::SDVX_RESULT_DELAY]);
		break;
	}
	case 0xC:
	{
		std::cout << "Attract screen" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_ATTRACT]);
		break;
	}
	case 0xB:
	{
		std::cout << "Title screen" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_TITLE]);
		break;
	}
	case 0x22:
	{
		std::cout << "Course select" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_SELECT]);
		break;
	}
	case 0xF:
	{
		std::cout << "Course result" << std::endl;
		SendSwitchScene(sM[Scene::SDVX_COURSE_RESULT]);
		break;
	}
	default:
	{
		std::cout << "SceneID: " << std::hex << sceneID << std::endl;
	}
	}
}