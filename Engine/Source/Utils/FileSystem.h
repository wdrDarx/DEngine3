#pragma once
#include "Core/Core.h"
#include "Serialization/Buffer.h"

#define FILE_THREADED 

/*
	Class for opening file dialogs
*/
class DENGINE_API FileDialog
{
public:
	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);
};

/*
	Utility class for the filesystem
*/
class DENGINE_API File
{
	public:
		static void ReadFile(const std::string& path, Buffer& buffer);
		static void ReadFile(const std::string& path, Buffer& buffer, const size_t& bytesToRead);
		static std::future<void> AsyncReadFile(const std::string& path, Buffer& buffer);
		static std::string ReadFileAsString(const std::string& path);
		static void WriteFileAsString(const std::string& path, const std::string& string);
		static std::string GetFileExtenstionFromPath(const std::string& path);
		static std::string RemoveFileExtensionFromPath(const std::string& path);
		static std::string GetFileNameFromPath(const std::string& path);
		static std::wstring StringToWideString(const std::string& str);
		static std::string WideStringToString(const std::wstring& str);
		static void WriteFile(const std::string& path, const Buffer& buffer);
		static std::future<void> AsyncWriteFile(const std::string& path, const Buffer& buffer);
		static int GetFileSize(const std::string& path);
		static bool DoesFileExist(const std::string& path);
		static bool DoesPathExist(const std::string& path);
		static void MakePath(const std::string& path);
};

