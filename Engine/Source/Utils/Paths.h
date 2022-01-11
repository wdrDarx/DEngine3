#pragma once
#include "Core/Core.h"

/*
	static class to store paths
*/
class DENGINE_API Paths
{
public:
	static void CreateBaseDirs();
	static std::string GetBaseDirectory();
	static std::string GetEngineDirectory();
	static std::string GetModulesDirectory();
	static std::string GetContentDirectory();
	static std::string GetSavedDirectory();
	static std::string GetCacheDirectory();
	static std::string GetConfigDirectory();
	static std::string GetModuleContentDirectory(const std::string& ModuleName);
	static std::string MakeRelativeTo(const std::string& Base, const std::string& Path);
};

