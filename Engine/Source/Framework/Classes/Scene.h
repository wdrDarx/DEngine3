#pragma once
#include "AppObject.h"
#include "SceneObject.h"
#include "Event/Callback.h"
#include "Event/EventDispatcher.h"
#include "Rendering/Classes/Renderer.h"
#include "Framework/Classes/InputManager.h"
#include "Framework/ModuleManager.h"
#include "Framework/Classes/Properties/PropertyClasses.h"
#include "Rendering/Classes/Pipeline.h"
//#include "Physics/PhysicsScene.h"

struct EventScenePreDeleteObject : public Event { ObjectBase* Object; };
struct EventScenePostDeleteObject : public Event { ObjectBase* Object; };
struct EventScenePreInitializeObject : public Event { ObjectBase* Object; };
struct EventScenePostInitializeObject : public Event { ObjectBase* Object; };


/*
* An AppObject that contains scoped SceneObjects and relays functions to them
* Contains an array of renderers that can be invoked by scene objects
* Must call Initialize(const ObjectInitializer& initializer) manually after constructing
*/
class Camera;
class DENGINE_API Scene : public AppObject
{
public:
	OBJECT_CLASS_DEF(Scene, AppObject)
		PROPS_BEGIN()
			PROPDEF(SceneObjectCount, EditAnywhere);
		PROPS_END()

	DebugText SceneObjectCount;

	//calls begin play for all scene objects
	void OnBeginPlay() override;

	//calls end play for all scene objects
	void OnEndPlay() override;

	void OnUpdate(const Tick& tick) override;
	void OnConstruct() override;
	void OnDestroy() override;

	size_t Serialize(Buffer& buffer) const override;
	size_t Deserialize(const Buffer& buffer) override;

	const std::vector<Ref<SceneObject>>& GetSceneObjects() const
	{
		return m_SceneObjects;
	}

	/* 
		Creates an SceneObject with an assigned pointer to this scene and a random id, then adds it to scene object list here
		WARNING : does not work when called from a module, use ObjectInitializer::Module(this) or  ObjectInitializer::Module("ModuleName")
	*/
	template<class T>
	Ref<T> CreateSceneObject(ObjectInitializer& initializer = ObjectInitializer())
	{
		bool valid = std::is_base_of<SceneObject, T>::value;
		ASSERT(valid);

		Ref<T> ptr = MakeRef<T>(this);

		Ref<SceneObject> obj = Cast<SceneObject>(ptr);
		m_SceneObjects.push_back(obj);

		//Must call this
		initializer.Flags |= ConstructFlags::RANDOMID;
		ptr->Initialize(initializer);

		return ptr;
	}

	void DestroyAllSceneObjects()
	{
		//call destroy on all scene objects
		for (auto& obj : GetSceneObjects())
		{
			EventScenePreDeleteObject event;
			event.Object = obj.get();

			//Dispatch PRE_DELETE event
			GetSceneEventDipatcher().Dispatch(event);

			//call on destroy
			obj->OnDestroy();

			//Dispatch POST_DELETE event
			EventScenePostDeleteObject event2;
			event2.Object = obj.get();
			GetSceneEventDipatcher().Dispatch(event2);
		}

		//this actually kills memory
		m_SceneObjects.clear();
	}

	//remove object from Scene Object array
	void DestroySceneObject(Ref<SceneObject> obj);

	//remove object from Scene Object array
	void DestroySceneObject(SceneObject* obj);

	//Will call initialize to an already existing scene object 
	//overridable object initializer (default flags are RANDOMID)
	void AddSceneObject(SceneObject* obj, const ObjectInitializer& Initializer = ObjectInitializer(ConstructFlags::RANDOMID));

	//Will call initialize to an already existing scene object 
	//overridable object initializer (default flags are RANDOMID)
	void AddSceneObject(Ref<SceneObject> obj, const ObjectInitializer& Initializer = ObjectInitializer(ConstructFlags::RANDOMID));

	void BindOnPreDeleteObject(Callback<EventScenePreDeleteObject>& callback)
	{
		GetSceneEventDipatcher().Bind(callback);
	}

	void BindOnPostDeleteObject(Callback<EventScenePostDeleteObject>& callback)
	{
		GetSceneEventDipatcher().Bind(callback);
	}

	EventDispatcher& GetSceneEventDipatcher() 
	{
		return m_EventDispatcher;
	}

	InputManager& GetInputManager()
	{
		return m_InputManager;
	}

	Ref<Pipeline> GetPipeline()
	{
		return m_Pipeline;
	}

	void DestroyPipeline()
	{
		if(m_Pipeline)
		{ 
			m_Pipeline->OnDestroy();
			m_Pipeline = nullptr;
		}
	}

	//create a pipeline from a custom class, passing in a ref to this scene and a render API
	template<class T>
	Ref<T> SetPipeline(Ref<RenderAPI> renderApi, ObjectInitializer& initializer = ObjectInitializer())
	{
		bool valid = std::is_base_of<Pipeline, T>::value;
		ASSERT(valid);

		Ref<T> ptr = MakeRef<T>(this, renderApi);

		Ref<Pipeline> obj = Cast<Pipeline>(ptr);
		m_Pipeline = obj;

		//Must call this
		initializer.Flags |= ConstructFlags::RANDOMID;
		ptr->Initialize(initializer);

		return ptr;
	}

	//set the pipeline from an already created pipeline object
	void SetPipeline(Pipeline* pipeline, Ref<RenderAPI> renderApi)
	{
		Ref<Pipeline> ptr = ToRef(pipeline);
		m_Pipeline = ptr;

		if(!pipeline->IsInitialized())
		{ 
			ObjectInitializer initializer;
			initializer.Flags |= ConstructFlags::RANDOMID;
			ptr->Initialize(initializer);
		}
	}

	//used during play, doesnt represet the actual render target
	Ref<Camera> GetActiveCamera()
	{
		return m_ActiveCamera;
	}

	void SetActiveCamera(Ref<Camera> camera)
	{
		m_ActiveCamera = camera;
	}

// 	Ref<PhysicsScene> GetPhysicsScene()
// 	{
// 		return m_PhysicsScene;
// 	}

	//void CreatePhysicsScene(PhysicsWorld* worldContext);

private:

	//for scene events
	EventDispatcher m_EventDispatcher;

	//input manager specific to a scene
	InputManager m_InputManager;

	//array of scene objects
	std::vector<Ref<SceneObject>> m_SceneObjects;

	//destorys the scene 
	Callback<EventModuleUnloaded> m_ModuleUnloadEvent;

	//optional setting, used during play mode 
	Ref<Camera> m_ActiveCamera = nullptr;

	//used for rendering
	Ref<Pipeline> m_Pipeline = nullptr;

	//PhysX Scene
//	Ref<PhysicsScene> m_PhysicsScene = nullptr;
};