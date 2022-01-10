#pragma once
#include "Core/Core.h"

using Buffer = std::vector<byte>;

struct BufferWritter
{
	BufferWritter(Buffer& buffer) : m_TargetBuffer(buffer)
	{

	}

	void Write(const void* data, size_t size);
	void WriteOffset(size_t StartPos, const void* data, size_t size);

	template<typename T>
	void WriteVec(std::vector<T>& vec)
	{
		//write the size
		size_t vecSize = vec.size();
		Write(&vecSize, sizeof(size_t));
		Write(vec.data(), vec.size() * sizeof(T));
	}

	template<typename T>
	void WriteVecElementSize(std::vector<T>& vec, const size_t& elementSize)
	{
		//write the size	
		size_t vecSize = vec.size();
		Write(&vecSize, sizeof(size_t));
		Write(vec.data(), vec.size() * elementSize);
	}

	template<typename T>
	void WriteVecCustom(const std::vector<T>& vec, std::function<void(const T& elem)> WriteFunc)
	{
		//write the size	
		size_t vecSize = vec.size();
		Write(&vecSize, sizeof(size_t));

		//write each element with function
		for (uint i = 0; i < vecSize; i++)
		{
			WriteFunc(vec[i]);
		}
	}

	void WriteStringVector(const std::vector<std::string>& vec);

	void WriteString(const std::string& str);

	//in bytes
	size_t m_CurrentOffset = 0;
	Buffer& m_TargetBuffer;
};


struct BufferReader
{
	BufferReader(const Buffer& buffer) : m_TargetBuffer(buffer)
	{

	}

	void Read(void* data, size_t size);
	void ReadOffset(size_t StartPos, void* data, size_t size);

	template<typename T>
	void ReadVec(std::vector<T>& vec)
	{
		//read the size	
		size_t size = 0;
		Read(&size, sizeof(size_t));

		//resize if too small
		if (vec.size() < size)
			vec.resize(size);

		Read(vec.data(), size * sizeof(T));
	}

	template<typename T>
	void ReadVecElementSize(std::vector<T>& vec, const size_t& elementSize)
	{
		//read the size	
		size_t size = 0;
		Read(&size, sizeof(size_t));

		//resize if too small
		if (vec.size() < size)
			vec.resize(size);

		Read(vec.data(), size * elementSize);
	}

	template<typename T>
	void ReadVecCustom(std::vector<T>& vec, std::function<void(T& elem)> ReadFunc)
	{
		//read the size	
		size_t size = 0;
		Read(&size, sizeof(size_t));

		//resize if too small
		if (vec.size() < size)
			vec.resize(size);

		//read each element with function
		for (uint i = 0; i < size; i++)
		{
			T temp;
			ReadFunc(temp);
			vec[i] = temp;
		}
	}

	void ReadStringVector(std::vector<std::string>& vec);

	void ReadString(std::string& str);

	//in bytes
	size_t m_CurrentOffset = 0;
	const Buffer& m_TargetBuffer;
};


