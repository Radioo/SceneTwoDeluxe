#pragma once
#include "pch.h"

LPMODULEINFO GetGameModuleInfo();
std::string GetGameVersion(LPMODULEINFO mInfo);
std::vector<LPCWSTR> GetProductVersion(const LPCWSTR filePath);