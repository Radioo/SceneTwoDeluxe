#pragma once
#include "pch.h"

struct MODULEINFOEX {
	LPMODULEINFO lpModuleInfo;
	LPCWSTR lpModuleName;
};

LPCWSTR GetCurrentModuleName();

LPMODULEINFO GetGameModuleInfo();
std::string GetGameVersion(LPMODULEINFO mInfo);
std::vector<LPCWSTR> GetProductVersion(const LPCWSTR filePath);