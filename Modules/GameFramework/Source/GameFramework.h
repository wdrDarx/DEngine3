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


struct IntProperty : public Property, AutoRegister<IntProperty, GameFramework>
{
	AUTO_REGISTER()
	PROP_CLASS_DEF(IntProperty, int);

	Buffer MakeValueBuffer(const void* ValuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(ValuePtr, sizeof(int));
		return out;
	}

	void ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(TargetValuePtr, sizeof(int));
	}

};

