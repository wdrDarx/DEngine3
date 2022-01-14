#pragma once
#include "Rendering/Classes/Pipeline.h"
#include "Utils/Thread.h"
#include "Rendering/Classes/Renderers/QuadRenderer.h"

//data containing all render data for a frame
struct SceneRenderProxy
{
	QuadRendererProxy Quads;
};

class DENGINE_API DefaultPipeline : public Pipeline
{
	public:
		OBJECT_CLASS_DEF(DefaultPipeline, Pipeline)

	//once per tick after everything is updated
	void PrepareFrame() override;

	//call for each render target (e.g a viewport)
	void RenderFrame(Ref<Camera> camera) override;

	// call once after finished rendering to all render targets 
	// (used to clear renderer data)
	void ClearFrame() override;

	SceneRenderProxy m_CurrentFrameProxy;
	SceneRenderProxy m_LastFrameProxy;


	//the render thread
	Thread m_RenderThread;
};