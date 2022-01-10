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

	static float Float();
	static uint Int();
	static uint64 Int64();

private:

	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

DEFINE_SINGLETON(Rand, Get_Rand);
