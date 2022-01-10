#include "Singleton.h"

void SingletonCache::Store(const std::type_index& type, void* ptr)
{
	return; //temp 
	m_PointerCache[type] = ptr;
}

void* SingletonCache::Find(const std::type_index& type)
{
	if(m_PointerCache.empty()) return nullptr;

	for (auto& pair : m_PointerCache)
	{
		if (pair.first == type)
			return pair.second;
	}

	return nullptr;
}

std::unordered_map<std::type_index, void*> SingletonCache::m_PointerCache;
