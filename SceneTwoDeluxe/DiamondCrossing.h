#pragma once
#include "pch.h"

char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3);

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo, LPCWSTR modName);

void ParseJson(const char* jsonName);

void HookVoltex6();

void HookVoltex6_2022042500();

void HookVoltex6EAC();

void HookVoltex6EAC_2022053103();

void SceneSwitchSDVX6(unsigned int sceneID);

void SceneSwitchSDVX6_2022042500(unsigned int sceneID);

void SceneSwitchSDVX6EAC(unsigned int sceneID);

void Hook25();

void SceneSwitch25(unsigned int sceneID);

void Hook27();

void SceneSwitch27(unsigned int sceneID);

void Hook28();

void SceneSwitch28(unsigned int sceneID);
