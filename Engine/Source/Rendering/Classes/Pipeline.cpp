#include "Pipeline.h"
#include "DEngine.h"

void Pipeline::OnConstruct()
{
	Super::OnConstruct();

	//auto destroy pipeline if its outer module is unloaded
	m_ModuleCallback.Assign([&](EventModuleUnloaded& event)
	{
		if (event.ModuleName == GetAssociatedModuleName())
		{
			GetScene()->DestroyPipeline();
		}
	});

	GetScene()->GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleCallback);
}

void Pipeline::OnDestroy()
{
	Super::OnDestroy();

	DestroyAllRenderers();
}

void Pipeline::DestroyRenderer(Renderer* renderer)
{
	auto remove = m_Renderers.end();
	for (auto it = m_Renderers.begin(); it != m_Renderers.end(); it++)
	{
		if (renderer == (*it).get())
		{
			remove = it;
			break;
		}
	}

	if (remove != m_Renderers.end())
	{
		//call on destroy
		(*remove)->OnDestroy();
		m_Renderers.erase(remove);
	}
}

void Pipeline::DestroyAllRenderers()
{
	//call destroy on all renderers
	for (auto& renderer : m_Renderers)
	{
		renderer->OnDestroy();
	}

	m_Renderers.clear();
}

void Pipeline::PrepareFrame()
{
	if(!GetRenderAPI()) return;

	for (uint i = 0; i < m_Renderers.size(); i++)
	{
		m_Renderers[i]->PrepareFrame();
	}
}

void Pipeline::RenderFrame(Ref<Camera> camera)
{
	if (!GetRenderAPI()) return;

	for (uint i = 0; i < m_Renderers.size(); i++)
	{
		m_Renderers[i]->RenderFrame(camera);
	}
}

void Pipeline::ClearFrame()
{
	if (!GetRenderAPI()) return;

	for (uint i = 0; i < m_Renderers.size(); i++)
	{
		m_Renderers[i]->ClearFrame();
	}
}

