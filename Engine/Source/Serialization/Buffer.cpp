#include "Buffer.h"

void BufferWritter::Write(const void* data, size_t size)
{
	WriteOffset(m_CurrentOffset, data, size);
	m_CurrentOffset += size;
}

void BufferWritter::WriteOffset(size_t StartPos, const void* data, size_t size)
{
	if (m_TargetBuffer.size() < StartPos + size)
		m_TargetBuffer.resize(StartPos + size);

	memcpy(m_TargetBuffer.data() + StartPos, data, size);
}

void BufferWritter::WriteStringVector(const std::vector<std::string>& vec)
{
	WriteVecCustom<std::string>(vec, [&](const std::string& str) { WriteString(str); });
}

void BufferWritter::WriteString(const std::string& str)
{
	std::vector<char> charVec(str.begin(), str.end());
	WriteVec(charVec);
}

void BufferWritter::WriteBuffer(const Buffer& buffer)
{
	WriteVec(buffer);
}

void BufferReader::Read(void* data, size_t size)
{
	ReadOffset(m_CurrentOffset, data, size);
	m_CurrentOffset += size;
}

void BufferReader::ReadOffset(size_t StartPos, void* data, size_t size)
{
	memcpy(data, m_TargetBuffer.data() + StartPos, size);
}

void BufferReader::ReadStringVector(std::vector<std::string>& vec)
{
	ReadVecCustom<std::string>(vec, [&](std::string& str) { ReadString(str); });
}

void BufferReader::ReadString(std::string& str)
{
	std::vector<char> charVec;
	ReadVec(charVec);
	str = std::string(charVec.begin(), charVec.end());
}

void BufferReader::ReadBuffer(Buffer& buffer)
{
	ReadVec(buffer);
}

void BufferArray::FromBuffer(const Buffer& buffer)
{
	uint size = 0;
	uint offset = 0;
	while (offset < buffer.size())
	{
		memcpy(&size, buffer.data() + offset, sizeof(uint));
		offset += sizeof(uint);

		Buffer piece(size);
		memcpy(piece.data(), buffer.data() + offset, size);
		offset += size;
		AddPiece(piece);
	}
}

Buffer BufferArray::MakeBuffer()
{
	Buffer buffer;

	for (auto& piece : m_DataPieces)
	{
		uint size = (uint)piece.size();
		buffer.resize(buffer.size() + sizeof(uint));
		memcpy(buffer.data() + buffer.size() - sizeof(uint), &size, sizeof(uint));

		buffer.insert(buffer.end(), piece.begin(), piece.end());
	}

	return buffer;
}

void BufferMap::QuickWrite(const std::string& key, std::function<void(BufferWritter& writter)> WriteFunc)
{
	Buffer buffer;
	BufferWritter writter(buffer);
	WriteFunc(writter);
	AddPiece(key, buffer);
}

void BufferMap::QuickRead(const std::string& key, std::function<void(BufferReader& reader)> ReadFunc)
{
	const Buffer& buffer = m_DataPieces[key];
	BufferReader reader(buffer);
	ReadFunc(reader);
}

void BufferMap::FromBuffer(const Buffer& buffer)
{
	BufferArray PieceBuffer;
	PieceBuffer.FromBuffer(buffer);

	for (const auto& piece : PieceBuffer.GetDataPieces())
	{
		std::string Key;
		Buffer Data;

		BufferReader reader(piece);
		reader.ReadString(Key);
		reader.ReadBuffer(Data);

		AddPiece(Key, Data);
	}
}

Buffer BufferMap::MakeBuffer()
{
	BufferArray PieceBuffer;
	for (auto& piece : m_DataPieces)
	{
		Buffer BufferPiece;

		BufferWritter writter(BufferPiece);
		writter.WriteString(piece.first);
		writter.WriteBuffer(piece.second);

		PieceBuffer.AddPiece(BufferPiece);
	}

	return PieceBuffer.MakeBuffer();
}
