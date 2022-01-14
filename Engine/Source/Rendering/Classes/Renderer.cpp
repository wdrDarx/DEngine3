#include "Renderer.h"
#include "DEngine.h"

void Renderer::OnConstruct()
{
	Super::OnConstruct();

	//auto destroy renderer if its outer module is unloaded
	m_ModuleCallback.Assign([&](EventModuleUnloaded& event)
	{
		if (event.ModuleName == GetAssociatedModuleName())
		{
			GetPipeline()->DestroyRenderer(this);
		}
	});

	GetPipeline()->GetScene()->GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleCallback);
}

void Renderer::PrepareFrame()
{
	if(!GetPipeline()->GetRenderAPI()) return; //no scene render api context to actually render to
}

void Renderer::RenderFrame(Ref<Camera> camera)
{
	if (!GetPipeline()->GetRenderAPI()) return; //no scene render api context to actually render to
}

void Renderer::ClearFrame()
{
	if (!GetPipeline()->GetRenderAPI()) return; //no scene render api context to actually render to
}

