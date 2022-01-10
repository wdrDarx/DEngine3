#pragma once
#include "Core/Core.h"

//class for storing pointers to singletons 
//not a singleton itself, this object exists for each module
class SingletonCache
{
	public:

		//stores a singlton pointer under a class type
		static void Store(const std::type_index& type, void* ptr);

		//finds the singleton with the specified class, returns nullptr if not found
		static void* Find(const std::type_index& type);

	//stores the type index of the singleton class to the pointer of it
	static std::unordered_map<std::type_index, void*> m_PointerCache;
};

//Makes this class a singleton throughout modules and the application, fetch this singleton with GET_SINGLETON(class)
//Example : DEFINE_SINGLETON(Log, Get_Log)
#define DEFINE_SINGLETON(class, Get_class) \
extern "C" DLL_EXPORT inline class & Get_class() { static class l{}; return l; }

//Fetch a singleton from anywhere, defined with DEFINE_SINGLETON(class, Get_class)
#define GET_SINGLETON(class) [&]() -> class& \
{ std::string name = std::string("Get_") + std::string(#class); \
if(void* ptr = SingletonCache::Find(typeid(class))) { return *(class*)(ptr); } \
class& (*func)() = (class & (*)())GetProcAddress(GetModuleHandle(NULL), name.c_str()); \
class& cl = func(); \
SingletonCache::Store(typeid(class), &cl); \
return cl; }()