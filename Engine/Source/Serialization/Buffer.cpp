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
