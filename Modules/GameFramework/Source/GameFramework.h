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

struct FunctionProperty : public Property
{
	PROP_CLASS_DEF(FunctionProperty, std::function<void()>)

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		
	}
};


