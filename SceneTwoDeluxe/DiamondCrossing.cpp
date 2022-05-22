#include "pch.h"
#include "DiamondCrossing.h"
#include "WSClient.h"

LPMODULEINFO CurrentModuleInfo;
bool* isDP;

nlohmann::json j;
std::string uri;

char (__fastcall* OnSceneSwitch_origSDVX6)(void* a1, unsigned int sceneID, __int64 a3, char a4, char a5) = nullptr;
char __fastcall OnSceneSwitch_hookSDVX6(void* a1, unsigned int sceneID, __int64 a3, char a4, char a5)
{
	SceneSwitchSDVX6(sceneID);
	return OnSceneSwitch_origSDVX6(a1, sceneID, a3, a4, a5);
}

char(__fastcall* OnSceneSwitch_orig28)(void* a1, unsigned int sceneID, __int64 a3) = nullptr;
char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3)
{
	SceneSwitch28(sceneID);
	return OnSceneSwitch_orig28(a1, sceneID, a3);
}

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo)
{
	CurrentModuleInfo = mInfo;	
	// IIDX28
	if (version.substr(10,10) == "2021091500")
	{
		std::cout << "Found supported version: " << version << std::endl;
		ParseJsonIIDX();
		auto task = std::async(std::launch::async, RunServer, uri);
		Hook28();
	}
	else if (version.substr(10, 10) == "2021121400")
	{
		ParseJsonSDVX();
		auto task = std::async(std::launch::async, RunServer, uri);
		HookVoltex6();
		std::cout << "Found supported version: " << version << std::endl;
	}
	else
	{
		std::cout << "Found unsupported version: " << version << std::endl;
	}
}

void ParseJsonSDVX()
{
	std::ifstream in("SceneTwoVoltex.json");
	in >> j;
	in.close();
	uri = j["obs-address"];
	std::cout << "json dump: " << j.dump(4) << std::endl;
}

void HookVoltex6()
{
	uintptr_t sceneSwitchFuncAddr = 0x43B950;
	uintptr_t addrAfterOffset = (uintptr_t)CurrentModuleInfo->lpBaseOfDll + sceneSwitchFuncAddr;

	MH_CreateHook(
		reinterpret_cast<void*>(addrAfterOffset),
		reinterpret_cast<void*>(OnSceneSwitch_hookSDVX6),
		reinterpret_cast<void**>(&OnSceneSwitch_origSDVX6)
	);
	MH_EnableHook(MH_ALL_HOOKS);

	std::cout << "Hooks enabled" << std::endl;
}

void SceneSwitchSDVX6(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x11:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(j["music-select"]);
		break;
	}
	case 0x2B:
	{
		std::cout << "Stage scene" << std::endl;
		SendSwitchScene(j["stage"]);
		break;
	}
	case 0xF:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(j["result-screen"]);
		break;
	}
	case 0xD:
	{
		std::cout << "Attract screen" << std::endl;
		SendSwitchScene(j["attract-screen"]);
		break;
	}
	case 0xC:
	{
		std::cout << "Title screen" << std::endl;
		SendSwitchScene(j["title-screen"]);
		break;
	}
	case 0x2D:
	{
		std::cout << "Test menu" << std::endl;
		SendSwitchScene(j["test-menu"]);
		break;
	}
	case 0x23:
	{
		std::cout << "Course select" << std::endl;
		SendSwitchScene(j["course-select"]);
		break;
	}
	case 0x10:
	{
		std::cout << "Course result" << std::endl;
		SendSwitchScene(j["course-result"]);
		break;
	}
	default:
	{
		std::cout << "SceneID: " << sceneID << std::endl;
	}
	}
}

void ParseJsonIIDX()
{
	std::ifstream in("SceneTwoDeluxe.json");
	in >> j;
	in.close();
	uri = j["obs-address"];
	std::cout << "json dump: " << j.dump(4) << std::endl;
}

void Hook28()
{
	uintptr_t isDPAddr = 0x50A11C4;
	isDP = (bool*)((uintptr_t)CurrentModuleInfo->lpBaseOfDll + isDPAddr);
	
	uintptr_t sceneSwitchFuncAddr = 0x782E80;
	uintptr_t addrAfterOffset = (uintptr_t)CurrentModuleInfo->lpBaseOfDll + sceneSwitchFuncAddr;

	std::cout << "addrAfterOffset: " << addrAfterOffset << std::endl;

	MH_CreateHook(
		reinterpret_cast<void*>(addrAfterOffset),
		reinterpret_cast<void*>(OnSceneSwitch_hook28),
		reinterpret_cast<void**>(&OnSceneSwitch_orig28)
	);
	MH_EnableHook(MH_ALL_HOOKS);

	std::cout << "Hooks enabled" << std::endl;
}

void SceneSwitch28(unsigned int sceneID)
{
	switch (sceneID)
	{
	case 0x15:
	{
		std::cout << "Music select scene" << std::endl;
		SendSwitchScene(j["music-select"]);
		break;
	}
	case 0x43:
	{
		if (*isDP)
		{
			std::cout << "PFree DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "PFree SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x3E:
	{
		if (*isDP)
		{
			std::cout << "Free DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "Free SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x3D:
	{
		if (*isDP)
		{
			std::cout << "Standard DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "Standard SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x16:
	{
		std::cout << "Result scene" << std::endl;
		SendSwitchScene(j["result-screen"]);
		break;
	}
	case 0x36:
	{
		std::cout << "Monitor check scene" << std::endl;
		SendSwitchScene(j["monitor-check"]);
		break;
	}
	case 0x2C:
	{
		std::cout << "Attract screen scene" << std::endl;
		SendSwitchScene(j["attract-screen"]);
		break;
	}
	case 0x2B:
	{
		std::cout << "Title screen scene" << std::endl;
		SendSwitchScene(j["title-screen"]);
		break;
	}
	case 0x10:
	{
		std::cout << "Card in scene" << std::endl;
		SendSwitchScene(j["card-in"]);
		break;
	}
	case 0x12:
	{
		std::cout << "Mode select scene" << std::endl;
		SendSwitchScene(j["mode-select"]);
		break;
	}
	case 0x4A:
	{
		std::cout << "Test menu scene" << std::endl;
		SendSwitchScene(j["test-menu"]);
		break;
	}
	case 0x1D:
	{
		std::cout << "Step up music select" << std::endl;
		SendSwitchScene(j["music-select"]);
		break;
	}
	case 0x42:
	{
		if (*isDP)
		{
			std::cout << "Step up DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "Step up SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x17:
	{
		std::cout << "Dan select stage" << std::endl;
		SendSwitchScene(j["dan-select"]);
		break;
	}
	case 0x3A:
	{
		if (*isDP)
		{
			std::cout << "Dan DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "Dan SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x18:
	{
		std::cout << "Dan result scene" << std::endl;
		SendSwitchScene(j["dan-result"]);
		break;
	}
	case 0xF:
	{
		std::cout << "Card out scene" << std::endl;
		SendSwitchScene(j["card-out"]);
		break;
	}
	case 0x23:
	{
		std::cout << "Arena lobby scene" << std::endl;
		SendSwitchScene(j["arena-lobby"]);
		break;
	}
	case 0x24:
	{
		std::cout << "Arena music select scene" << std::endl;
		SendSwitchScene(j["music-select"]);
		break;
	}
	case 0x25:
	{
		std::cout << "Arena before song starts" << std::endl;
		SendSwitchScene(j["arena-before-song"]);
		break;
	}
	case 0x44:
	{
		if (*isDP)
		{
			std::cout << "Arena DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "Arena SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x26:
	{
		std::cout << "Arena podium scene" << std::endl;
		SendSwitchScene(j["arena-podium"]);
		break;
	}
	case 0x27:
	{
		std::cout << "BPL lobby" << std::endl;
		SendSwitchScene(j["bpl-lobby"]);
		break;
	}
	case 0x28:
	{
		std::cout << "BPL music select" << std::endl;
		SendSwitchScene(j["music-select"]);
		break;
	}
	case 0x29:
	{
		std::cout << "BPL before song starts" << std::endl;
		SendSwitchScene(j["bpl-before-song"]);
		break;
	}
	case 0x45:
	{
		if (*isDP)
		{
			std::cout << "BPL DP stage scene" << std::endl;
			SendSwitchScene(j["dp-stage"]);
		}
		else
		{
			std::cout << "BPL SP stage scene" << std::endl;
			SendSwitchScene(j["sp-stage"]);
		}
		break;
	}
	case 0x2A:
	{
		std::cout << "BPL podium scene" << std::endl;
		SendSwitchScene(j["bpl-podium"]);
		break;
	}
	case 0x14:
	{
		std::cout << "DJVIP pass select" << std::endl;
		SendSwitchScene(j["djvip-pass-select"]);
		break;
	}
	case 0x32:
	{
		std::cout << "Attract loop tutorial" << std::endl;
		SendSwitchScene(j["attract-tutorial"]);
		break;
	}
	case 0x2F:
	{
		std::cout << "Demo play" << std::endl;
		SendSwitchScene(j["demo-play"]);
		break;
	}
	default:
	{
		std::cout << "sceneID: " << std::hex << sceneID << std::endl;
	}
	}
	
}