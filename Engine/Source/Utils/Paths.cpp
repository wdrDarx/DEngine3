#pragma once
#include "Paths.h"
#include <filesystem>
#include <iostream>

void Paths::CreateBaseDirs()
{
	std::filesystem::create_directory(GetContentDirectory());
	std::filesystem::create_directory(GetEngineDirectory());
	std::filesystem::create_directory(GetModulesDirectory());
	std::filesystem::create_directory(GetSavedDirectory());
	std::filesystem::create_directory(GetConfigDirectory());
	std::filesystem::create_directory(GetCacheDirectory());
}


std::string Paths::GetBaseDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

std::string Paths::GetEngineDirectory()
{
	return GetBaseDirectory() + "\\Engine\\";
}

std::string Paths::GetModulesDirectory()
{
	return GetBaseDirectory() + "\\Modules\\";
}

std::string Paths::GetContentDirectory()
{
	return GetBaseDirectory() + "\\Content\\";
}

std::string Paths::GetSavedDirectory()
{
	return GetBaseDirectory() + "\\Saved\\";
}

std::string Paths::GetCacheDirectory()
{
	return GetSavedDirectory() + "\\Cache\\";
}

std::string Paths::GetConfigDirectory()
{
	return  GetBaseDirectory() + "\\Config\\";
}

std::string Paths::GetModuleContentDirectory(const std::string& ModuleName)
{
	return GetModulesDirectory() + ModuleName + "\\Content\\";
}

std::string Paths::MakeRelativeTo(const std::string& Base, const std::string& Path)
{
	std::filesystem::path base(Base);
	std::filesystem::path path(Path);

	return  std::filesystem::relative(path, base).generic_string();
}
