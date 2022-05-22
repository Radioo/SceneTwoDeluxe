#pragma once
#include "pch.h"

char __fastcall OnSceneSwitch_hook28(void* a1, unsigned int sceneID, __int64 a3);

void StartSceneHooks(std::string& version, LPMODULEINFO mInfo);

void ParseJson();

void Hook28();

void SceneSwitch28(unsigned int sceneID);
