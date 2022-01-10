#include "Rand.h"
#include "Core/Log.h"

Rand::Rand()
{
	s_RandomEngine.seed(std::random_device()());
	LOG_INFO("Created Random Class");
}

float Rand::Float()
{
	return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
}

uint Rand::Int()
{
	return (uint)s_Distribution(s_RandomEngine);
}

uint64 Rand::Int64()
{
	return (uint64_t)Int() * (uint64_t)Int();
}

std::mt19937 Rand::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Rand::s_Distribution;

