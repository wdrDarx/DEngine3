#include "Rand.h"
#include "Core/Log.h"

Rand::Rand()
{
	m_RandomEngine.seed(std::random_device()());
	LOG_INFO("Created Random Class");
}

float Rand::Float()
{
	return (float)m_Distribution(m_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
}

uint Rand::Int()
{
	return (uint)m_Distribution(m_RandomEngine);
}

uint64 Rand::Int64()
{
	return (uint64_t)Int() * (uint64_t)Int();
}


