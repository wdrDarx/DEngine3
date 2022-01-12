#pragma once
#include "Core/Core.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

inline HMODULE GetCurrentModuleHandle()
{
	return (HMODULE)&__ImageBase;
}

DLL_EXPORT std::string GetCurrentModuleName();
DLL_EXPORT bool IsModuleLoaded(const std::string& ModuleName);

