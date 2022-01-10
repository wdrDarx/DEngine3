#pragma once
#include <random>
#include "Core/Core.h"


/*
	static class that provides random value functions (MAKE SURE TO CALL THE RAND FUNCTIONS FROM THE ENGINE MODULE FILES BECAUSE THE RANDOM ENGINE IS A STATIC VAR)
*/
class Rand
{
public:
	Rand();

	float Float();
	uint Int();
	uint64 Int64();

private:

	std::mt19937 m_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> m_Distribution;
};

DEFINE_SINGLETON(Rand, Get_Rand);
