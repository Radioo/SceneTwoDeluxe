#include "pch.h"
#include "GetGameModuleInfo.h"

#pragma comment(lib, "Version.lib")

struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

// Date code template found in dlls: ***:*:*:*:**********
const std::vector<BYTE> DateCodeTemplate = { 0x2A, 0x2A, 0x2A, 0x3A, 0x2A, 0x3A, 0x2A,
											 0x3A, 0x2A, 0x3A, 0x2A, 0x2A, 0x2A, 0x2A,
											 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A };

static std::regex const re{ R"([\w|\d][\w|\d][\w|\d][:][\w|\d][:][\w|\d][:][\w|\d][:]\d\d\d\d\d\d\d\d\d\d)", std::regex_constants::optimize };

// Names of possible game modules
const std::vector<LPCWSTR> GameModuleNames = { L"bm2dx.dll", L"bm2dx.exe", L"soundvoltex.dll", L"sv6c.exe"};

HMODULE CurrentGameModule = NULL;
LPCWSTR CurrentGameModuleName = NULL;

LPCWSTR GetCurrentModuleName()
{
	// Try to get module handle
	HMODULE hModule = NULL;
	for (LPCWSTR moduleName : GameModuleNames)
	{
		hModule = GetModuleHandle(moduleName);
		if (hModule != NULL)
		{
			CurrentGameModule = hModule;
			CurrentGameModuleName = moduleName;
			std::wcout << "Found game module: " << moduleName << std::endl;
			return moduleName;
		}
	}

	// If can't get module handle, output supported module names and return NULL
	if (hModule == NULL)
	{
		std::cout << GetLastError() << std::endl;
		std::cout << "Failed to get game module handle. This may happen if you renamed your game's module. "
			<< "Supported game modules: " << std::endl;

		for (LPCWSTR moduleName : GameModuleNames)
		{
			std::wcout << moduleName << std::endl;
		}
		return NULL;
	}
}

LPMODULEINFO GetGameModuleInfo()
{
	LPMODULEINFO moduleInfo = new MODULEINFO();
	
	// Try to get module info, return NULL if failed
	if (moduleInfo != NULL)
	{
		if (GetModuleInformation(GetCurrentProcess(), CurrentGameModule, moduleInfo, sizeof(MODULEINFO)))
		{
			return moduleInfo;
		}
		else
		{
			std::cout << GetLastError() << std::endl;
			std::cout << "Failed to get module information." << std::endl;
			return NULL;
		}
	}
	else
	{
		std::cout << GetLastError() << std::endl;
		return NULL;
	}
}

std::string GetGameVersion(LPMODULEINFO mInfo)
{	
	auto pVersion = GetProductVersion(CurrentGameModuleName);
	
	// Check if there's a product version string (INF only I guess)
	if (pVersion.size() == 1)
	{
		return std::string(CW2A(pVersion[0]));
	}
	else
	{
		std::cout << "No product information found, searching for date code..." << std::endl;

		const char* beg = (char*)mInfo->lpBaseOfDll;
		const char* end = beg + mInfo->SizeOfImage;
		std::cmatch m;

		while (!std::regex_search(beg, end, m, re))
		{
			std::cout << "Searching for game version..." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		//std::cout << "Found version string: " << std::hex << (uintptr_t)m[0].first << std::endl;
		return m[0].first;
	}
	return "NOT FOUND";
}

std::vector<LPCWSTR> GetProductVersion(const LPCWSTR filePath)
{
	std::vector<LPCWSTR> out;
	DWORD dwHandle;
	DWORD infoSize = GetFileVersionInfoSize(filePath, &dwHandle);
	LPVOID buffer = malloc(sizeof(TCHAR) * infoSize);

	if (!GetFileVersionInfo(filePath, NULL, infoSize, buffer))
	{
		//std::cout << "GetFileVersionInfo failed" << std::endl;
		return out;
	}

	LPVOID* translationPtr = &buffer;
	UINT translationLen = 0;

	if (VerQueryValue(buffer, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &translationLen))
	{
		translationLen = sizeof(struct LANGANDCODEPAGE) / translationLen;
	}
	else
	{
		std::cout << "Failed to get translation information" << std::endl;
		return out;
	}

	for (int i = 0; i < translationLen; i++)
	{
		HRESULT hr;

		STRSAFE_LPWSTR result = (STRSAFE_LPWSTR)malloc(50);

		hr = StringCchPrintf(result, 50, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),
			lpTranslate[i].wLanguage,
			lpTranslate[i].wCodePage);

		if (FAILED(hr))
		{
			std::cout << "Failed to format string" << std::endl;
			return out;
		}

		LPVOID* productVer = translationPtr;
		UINT productVerLen = 0;

		bool success = VerQueryValue(buffer, result, productVer, &productVerLen);
		if (success)
		{
			out.push_back((LPCWSTR)*productVer);
		}
	}
	return out;
}