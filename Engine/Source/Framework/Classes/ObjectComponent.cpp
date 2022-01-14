#include "ObjectComponent.h"
#include "DEngine.h"

void ObjectComponent::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);
}

void ObjectComponent::OnDestroy()
{
	Super::OnDestroy();

	//dont call destroy twice
	m_ModuleUnloadedEvent.Destroy();
}

void ObjectComponent::OnConstruct()
{
	Super::OnConstruct();

	//auto destroy object when its module is unloaded
	m_ModuleUnloadedEvent.Assign([&](EventModuleUnloaded& event)
	{
		//prevent auto delete if scene object was deleted prior
		if(!m_ModuleUnloadedEvent.m_OnTrigger) return;

		if (event.ModuleName == GetAssociatedModuleName())
		{
			GetOwner()->DestroyComponent(this);
		}
	});

	GetOwner()->GetScene()->GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadedEvent);
}


