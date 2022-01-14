#pragma once
#include "Framework/ObjectBase.h"
#include "Framework/FrameworkMacros.h"
#include "Rendering/RenderAPI.h"
#include "Event/Callback.h"
#include "Framework/ModuleManager.h"

class Pipeline;
class Camera;

/*
* inheritable custom class for modules to implement custom rendering (auto destroyed when a module unloads)
* exists in the scope of a scene
*/
class DENGINE_API Renderer : public ObjectBase
{
	public:	
		OBJECT_CLASS_DEF(Renderer, ObjectBase)

		//override constructor receiving a reference to the scene
		Renderer(Pipeline* pipeline) : ObjectBase(), m_Pipeline(pipeline)
		{

		}

		Pipeline* GetPipeline()
		{
			return m_Pipeline;
		}

		void OnConstruct() override;

		//called after every object ran their update function so its safe to handle data from there (CALLED ONCE PER TICK)
		virtual void PrepareFrame();

		//called for every camera that wants a render of this renderer
		virtual void RenderFrame(Ref<Camera> camera);

		//called once per tick after all the frames are rendered and data is safe to discard
		virtual void ClearFrame();

	private:
		Pipeline* m_Pipeline = nullptr;
		Callback<EventModuleUnloaded> m_ModuleCallback;
};
