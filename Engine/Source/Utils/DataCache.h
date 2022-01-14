#pragma once
#include "Core/Core.h"

struct DataObject
{
	DataObject (Ref<void> d) : ptr(d) 
	{
		
	}

	DataObject()
	{
	
	}

	~DataObject()
	{
		
	}

	Ref<void> ptr = nullptr;
};

/*
	class for storing Data pieces retrievable with a tag
	Data is stored with a Ref, so data is only unloaded after
	losing all references and being explicitly unloaded from the cache
*/
class DataCache
{
public:

	/*
		Caches a pointer with a tag,
		Can replace existing tags
	*/
	void Set(const std::string& tag, Ref<void> data)
	{	

		m_ObjectCache[tag] = DataObject(data);
	}

	/*
		removes data at the specified tag
	*/
	void Remove(const std::string& tag)
	{
		m_ObjectCache.erase(tag);
	}

	/*
		removes all data 
	*/
	void Clear()
	{
		m_ObjectCache.clear();
	}
	
	/*	
		Gets data pointer at the specified tag
	*/
	Ref<void> Get(const std::string& tag) const
	{
		for (const auto& element : m_ObjectCache)
		{
			if (tag == element.first)
			{
				return element.second.ptr;
			}
		}

		return nullptr;
	}

private:
	 std::unordered_map<std::string, DataObject> m_ObjectCache;
};

