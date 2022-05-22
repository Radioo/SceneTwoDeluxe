#pragma once
#include "pch.h"

char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3);

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo);

void ParseJsonSDVX();

void HookVoltex6();

void SceneSwitchSDVX6(unsigned int sceneID);

void ParseJsonIIDX();

void Hook28();

void SceneSwitch28(unsigned int sceneID);
