#pragma once
#include "DEngine.h"

#define PROJECT_API

#ifdef PROJECT_API
#define PROJECT __declspec(dllexport)
#else
#define PROJECT
#endif

class PROJECT Project : public Module
{
public:
	using Module::Module;

	void OnLoad() override;
	void OnUnload() override;
	void OnUpdate(float DeltaTime) override;
};

