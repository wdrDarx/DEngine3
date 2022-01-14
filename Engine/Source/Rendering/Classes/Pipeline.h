#pragma once
#include "Framework/ObjectBase.h"
#include "Framework/FrameworkMacros.h"
#include "Rendering/RenderAPI.h"
#include "Event/Callback.h"
#include "Framework/ModuleManager.h"

class Scene;
class Renderer;
class Camera;

/*
* inheritable custom class for modules to implement custom rendering (auto destroyed when a module unloads)
* exists in the scope of a scene
*/
class DENGINE_API Pipeline : public ObjectBase
{
public:
	OBJECT_CLASS_DEF(Pipeline, ObjectBase)

	//override constructor receiving a reference to the scene and renderAPI
	Pipeline(Scene* scene, Ref<RenderAPI> renderApi) : ObjectBase(), m_Scene(scene), m_RenderApi(renderApi)
	{

	}

	//same thing as the constructor but not a constructor
	void Create(Scene* scene, Ref<RenderAPI> renderApi)
	{
		m_Scene = scene;
		m_RenderApi = renderApi;
	}

	Scene* GetScene()
	{
		return m_Scene;
	}

	void OnConstruct() override;
	void OnDestroy() override;

	//create a renderer from a custom class, passing reference to this pipeline
	template<class T>
	Ref<T> CreateRenderer(ObjectInitializer& initializer = ObjectInitializer())
	{
		bool valid = std::is_base_of<Renderer, T>::value;
		ASSERT(valid);

		Ref<T> ptr = MakeRef<T>(this);

		Ref<Renderer> obj = Cast<Renderer>(ptr);
		m_Renderers.push_back(obj);

		//Must call this
		initializer.Flags |= ConstructFlags::RANDOMID;
		ptr->Initialize(initializer);

		return ptr;
	}

	//try get renderer by class if it exists in this scene
	template<class T>
	Ref<T> GetRenderer()
	{
		bool valid = std::is_base_of<Renderer, T>::value;
		ASSERT(valid);

		for (auto& renderer : m_Renderers)
		{
			if (auto out = Cast<T>(renderer))
			{
				return out;
			}
		}

		//ASSERT(false) //renderer not found in this scene
			return nullptr;
	}

	Ref<RenderAPI> GetRenderAPI()
	{
		return m_RenderApi;
	}
	
	//remove renderer from Scene Object array
	void DestroyRenderer(Renderer* renderer);

	//clears all renderers
	void DestroyAllRenderers();

	//once per tick after everything is updated
	virtual void PreUpdate();

	//once per tick after everything is updated
	virtual void PrepareFrame();

	//call for each render target (e.g a viewport)
	virtual void RenderFrame(Ref<Camera> camera);

	// call once after finished rendering to all render targets 
	// (used to clear renderer data)
	virtual void ClearFrame();

private:
	Scene* m_Scene = nullptr;
	Ref<RenderAPI> m_RenderApi = nullptr;
	Callback<EventModuleUnloaded> m_ModuleCallback;
	std::vector<Ref<Renderer>> m_Renderers;
};
