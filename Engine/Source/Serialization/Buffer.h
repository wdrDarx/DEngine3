#pragma once
#include "Core/Core.h"
#include "Framework/ClassType.h"

using Buffer = std::vector<byte>;



struct BufferWritter
{
	BufferWritter(Buffer& buffer) : m_TargetBuffer(buffer)
	{

	}

	BufferWritter(Buffer& buffer, const size_t& startingOffset) : m_TargetBuffer(buffer), m_CurrentOffset(startingOffset)
	{

	}

	size_t GetCurrentOffset() const
	{
		return m_CurrentOffset;
	}

	void Write(const void* data, size_t size);
	void WriteOffset(size_t StartPos, const void* data, size_t size);

	template<typename T>
	void WriteVec(const std::vector<T>& vec)
	{
		//write the size
		size_t vecSize = vec.size();
		Write(&vecSize, sizeof(size_t));
		Write(vec.data(), vec.size() * sizeof(T));
	}

	template<typename T>
	void WriteVecElementSize(const std::vector<T>& vec, const size_t& elementSize)
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
	void WriteBuffer(const Buffer& buffer);
	void WriteClassType(const ClassType& type);

	//in bytes
	size_t m_CurrentOffset = 0;
	Buffer& m_TargetBuffer;
};


struct BufferReader
{
	BufferReader(const Buffer& buffer) : m_TargetBuffer(buffer)
	{

	}

	BufferReader(const Buffer& buffer, const size_t& startingOffset) : m_TargetBuffer(buffer), m_CurrentOffset(startingOffset)
	{

	}

	size_t GetCurrentOffset() const
	{
		return m_CurrentOffset;
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
	void ReadBuffer(Buffer& buffer);

	//Also calls for a name update inside the class type
	void ReadClassType(ClassType& type);

	//in bytes
	size_t m_CurrentOffset = 0;
	const Buffer& m_TargetBuffer;
};



//array of buffers
struct DENGINE_API BufferArray
{
	std::vector<Buffer> m_DataPieces;

	const std::vector<Buffer>& GetDataPieces() const
	{
		return m_DataPieces;
	}

	void AddPiece(const Buffer& piece)
	{
		m_DataPieces.push_back(piece);
	}

	//Construct this BufferArray from a buffer (buffer that was generated with BufferArray::MakeBuffer())
	void FromBuffer(const Buffer& buffer);

	BufferArray(const Buffer& buffer)
	{
		FromBuffer(buffer);
	}

	BufferArray()
	{

	}

	//make a buffer from all the current data pieces
	Buffer MakeBuffer();
};


//map of buffers String corresponds to a buffer
struct DENGINE_API BufferMap
{
	std::unordered_map<std::string, Buffer> m_DataPieces;

	BufferMap(const Buffer& input)
	{
		FromBuffer(input);
	}

	BufferMap()
	{

	}

	const std::unordered_map<std::string, Buffer>& GetDataPieces() const
	{
		return m_DataPieces;
	}

	void AddPiece(const std::string& key, const Buffer& piece)
	{
		m_DataPieces[key] = piece;
	}

	bool HasKey(const std::string& key)
	{
		return (m_DataPieces.count(key));
	}

	//write to the map with a lambda 
	void QuickWrite(const std::string& key, std::function<void(BufferWritter& writter)> WriteFunc);
	//read from the map with a lambda 
	void QuickRead(const std::string& key, std::function<void(BufferReader& reader)> ReadFunc);


	Buffer& operator[](const std::string& key)
	{
		return m_DataPieces[key];
	}

	void FromBuffer(const Buffer& buffer);

	Buffer MakeBuffer();
};


