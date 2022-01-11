#pragma once
#include "Core/Core.h"

struct RemoveResult
{
	uint RemovedIndex;
	bool WasRemoved;
};

class VectorUtils
{
public:
	/*
		returns the erased index
	*/
	template<class T>
	static RemoveResult RemovePointerFromRefVector(T* Object, std::vector<Ref<T>>& vector);

	template<typename T>
	static RemoveResult RemoveFromVector(const T& object, std::vector<T>& vector);

	template<typename T>
	static uint IndexOf(const T& Object, std::vector<T>& vector);

	template<typename T>
	static bool Contains(const T& Object, std::vector<T>& vector);
};

template<class T>
RemoveResult VectorUtils::RemovePointerFromRefVector(T* Object, std::vector<Ref<T>>& vector)
{
	auto remove = vector.end();
	for (auto it = vector.begin(); it != vector.end(); it++)
	{
		if ((*it).get() == Object)
			remove = it;
	}
	if (remove != vector.end())
	{
		uint erased = std::distance(vector.begin(), remove);
		vector.erase(remove);
		return { erased , true };
	}

	return { 0, false };
}

template<typename T>
RemoveResult VectorUtils::RemoveFromVector(const T& Object, std::vector<T>& vector)
{
	auto remove = std::find(vector.begin(), vector.end(), Object);
	if (remove != vector.end())
	{
		uint erased = std::distance(vector.begin(), remove);
		vector.erase(remove);
		return { erased , true };
	}

	return { 0, false };
}

template<typename T>
uint VectorUtils::IndexOf(const T& Object, std::vector<T>& vector)
{
	auto remove = std::find(vector.begin(), vector.end(), Object);
	return std::distance(vector.begin(), remove);
}

template<typename T>
bool VectorUtils::Contains(const T& Object, std::vector<T>& vector)
{
	auto remove = std::find(vector.begin(), vector.end(), Object);
	return remove != vector.end();
}
