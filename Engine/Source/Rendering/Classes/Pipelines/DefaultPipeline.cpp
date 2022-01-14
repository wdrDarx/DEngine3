#include "DefaultPipeline.h"
#include "DEngine.h"

void DefaultPipeline::PrepareFrame()
{
	Super::PrepareFrame();

}

void DefaultPipeline::RenderFrame(Ref<Camera> camera)
{
	Super::RenderFrame(camera);

	RenderUtils::SetClearColor({1,1,1});
	RenderUtils::Clear();

}

void DefaultPipeline::ClearFrame()
{
	Super::ClearFrame();

}

