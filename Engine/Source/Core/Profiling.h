#pragma once
#include "Core/Core.h"
#include "optick.h"

#define PROFILING_ON
#ifdef PROFILING_ON
#define  PROFILE_FUNC(...) OPTICK_EVENT(__VA_ARGS__)
#define  PROFILE_TICK(...) OPTICK_FRAME(__VA_ARGS__)
#define  PROFILE_TAG(NAME, ...) OPTICK_TAG(NAME, __VA_ARGS__)
#else
#define  PROFILE_FUNC(...) 
#define  PROFILE_TICK(...) 
#define  PROFILE_TAG(NAME, ...)
#endif