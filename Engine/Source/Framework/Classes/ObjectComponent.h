#pragma once
#include "Framework/ObjectBase.h"
#include "Event/Callback.h"
#include "Framework/ModuleManager.h"
#include "Framework/FrameworkMacros.h"
#include "Event/EventDispatcher.h"

/*
	inheritable Object existent in the scope of a sceneObject
*/
class SceneObject;
class DENGINE_API ObjectComponent : public ObjectBase
{
public:	
	friend class SceneObject;

	OBJECT_CLASS_DEF(ObjectComponent, ObjectBase)

	//override constructor receiving a reference to the SceneObject
	ObjectComponent(SceneObject* obj) : ObjectBase(), m_Owner(obj)
	{
		
	}

	//begin play event
	virtual void OnBeginPlay()
	{

	}

	virtual void OnEndPlay()
	{

	}

	void OnUpdate(const Tick& tick) override;
	void OnConstruct() override;
	void OnDestroy() override;

	SceneObject* GetOwner() const
	{
		ASSERT(m_Owner);
		return m_Owner;
	}

private:
	SceneObject* m_Owner = nullptr;
	Callback<EventModuleUnloaded> m_ModuleUnloadedEvent;
};

