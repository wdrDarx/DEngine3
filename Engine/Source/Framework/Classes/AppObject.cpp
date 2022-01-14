#include "AppObject.h"
#include "Framework/Application.h"

void AppObject::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);

	//set last tick
	SetLastTick(tick);
}

void AppObject::OnConstruct()
{
	Super::OnConstruct();

	m_EventModuleUnloaded.Assign([&](EventModuleUnloaded& event)
	{
		//auto destroy if parent module is unloaded
		if (event.ModuleName == GetAssociatedModuleName())
			GetApplication()->DestroyAppObject(this);
	});

	GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_EventModuleUnloaded);
}
