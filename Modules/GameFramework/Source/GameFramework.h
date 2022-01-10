#pragma once
#include "Framework/Module.h"
#include "DEngine.h"

#define GAMEFRAMEWORK_API

#ifdef GAMEFRAMEWORK_API
#define GAMEFRAMEWORK __declspec(dllexport)
#else
#define GAMEFRAMEWORK
#endif

class GAMEFRAMEWORK GameFramework : public Module
{
public:
	using Module::Module;

	void OnLoad() override;
	void OnUnload() override;
	void OnUpdate(float DeltaTime) override;

	void TestMethod(const PropArray& args);
};

