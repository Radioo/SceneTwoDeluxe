#pragma once
#include "pch.h"
#include "ParseINI.h"

char __fastcall OnSceneSwitch_hookSDVX6(void* a1, unsigned int sceneID, __int64 a3, char a4, char a5);

char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3);

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo, LPCWSTR modName, std::string uri, std::map<Scene, std::string> sceneMap);

void EnableHookIIDX(uintptr_t sceneSwitchAddr, uintptr_t isDPAddr, void* onSceneSwitchHook, void* OnSceneSwitchOrig);

void EnableHookSDVX(uintptr_t sceneSwitchAddr, void* onSceneSwitchHook, void* OnSceneSwitchOrig);

void SceneSwitchIIDX_2018091900(unsigned int sceneID);

void SceneSwitchIIDX_2020092900(unsigned int sceneID);

void SceneSwitchIIDX_2021091500(unsigned int sceneID);

void SceneSwitchSDVX_2021121400(unsigned int sceneID);

void SceneSwitchSDVX_2022042500(unsigned int sceneID);

void SceneSwitchSDVX_2022052400(unsigned int sceneID);

void SceneSwitchSDVXEAC_2022042600(unsigned int sceneID);
