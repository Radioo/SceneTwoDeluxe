#pragma once
#include "pch.h"

char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3);

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo, LPCWSTR modName);

void ParseJson(const char* jsonName);

void HookVoltex6();

void HookVoltex6EAC();

void SceneSwitchSDVX6(unsigned int sceneID);

void SceneSwitchSDVX6EAC(unsigned int sceneID);

void Hook25();

void SceneSwitch25(unsigned int sceneID);

void Hook27();

void SceneSwitch27(unsigned int sceneID);

void Hook28();

void SceneSwitch28(unsigned int sceneID);
