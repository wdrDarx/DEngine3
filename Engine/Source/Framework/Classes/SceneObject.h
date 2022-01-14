#pragma once
#include "Framework/ObjectBase.h"
#include "Framework/Classes/ObjectComponent.h"
#include "Framework/FrameworkMacros.h"
#include "Framework/ModuleManager.h"
#include "Event/Callback.h"
//#include "Event/ModuleEvent.h"
//#include "Event/AssetEvent.h"
#include "Event/EventDispatcher.h"
#include "Framework/Classes/Components/TransformComponent.h"
//#include "Assets/AssetRef.h"

class PrefabAsset;
class Scene;
/*
* An Object existing in the scope of a Scene, can contain components
* Must call Initialize(const ObjectInitializer& initializer) mannually after constructing
*/
class DENGINE_API SceneObject : public ObjectBase
{
public:
	friend class Scene;

	OBJECT_CLASS_DEF(SceneObject, ObjectBase)

	//override constructor receiving a reference to the scene
	SceneObject(Scene* scene) : ObjectBase(), m_Scene(scene)
	{

	}

	//begin play event
	virtual void OnBeginPlay();
	virtual void OnEndPlay();

	//need explicit destructor to call pre_delete events for all components
	void OnDestroy() override;

	void OnUpdate(const Tick& tick) override;
	void OnConstruct() override;
	void OnPostConstruct() override;
	


	//serializes properties and all current component data like component properties, etc
	size_t Serialize(Buffer& buffer) const override;

	//loads in properties and any component data 
	//must be called after construct as to create any components
	//(this doesnt actually create any components)
	size_t Deserialize(const Buffer& buffer) override;

	Scene* GetScene() const
	{
		ASSERT(m_Scene);
		return m_Scene;
	}

	const std::vector<Ref<ObjectComponent>>& GetComponents() const
	{
		return m_Components;
	}

	/*  
		Creates an ObjectComponent with an assigned pointer to this SceneObject and a random id, then adds it to scene object list here
		ComponentName is used a key in serialization, so this must be unique
		WARNING : When creating from a module you must pass in the module name with ObjectInitializer::Module()
	*/
	template<class T>
	Ref<T> CreateComponent(const std::string& ComponentName, ObjectInitializer& initializer = ObjectInitializer())
	{
		bool valid = std::is_base_of<ObjectComponent, T>::value;
		ASSERT(valid && !ComponentName.empty()); //must be a component and must have a name

		Ref<T> ptr = MakeRef<T>(this);

		Ref<ObjectComponent> comp = Cast<ObjectComponent>(ptr);
		m_Components.push_back(comp);

		//Must have this
		initializer.Flags |= ConstructFlags::RANDOMID;

		// OnPostConstruct for components is called in the scene object's OnPostConstruct
		initializer.Flags |= ConstructFlags::NOPOSTCONSTRUCT;

		initializer.Name = ComponentName;
		ptr->Initialize(initializer);

		return ptr;
	}

	//remove object from Component array (return is the itterator at where the component was erased from the array)
	std::vector<Ref<ObjectComponent>>::iterator DestroyComponent(Ref<ObjectComponent> comp);

	//remove object from Component array
	void DestroyComponent(ObjectComponent* comp);

	//Will call initialize to an already existing Component
	void AddComponent(ObjectComponent* comp);

	bool IsPrefab() const
	{
		return (GetObjectFlags() & ObjectFlags::PREFAB);
	}

// 	void MarkPrefab(const AssetRef<PrefabAsset>& prefab)
// 	{
// 		GetObjectFlagsMutable() = (ObjectFlags)(GetObjectFlags() | ObjectFlags::PREFAB);
// 		m_PrefabAssetReference = prefab;
// 	}

// 	const AssetRef<PrefabAsset>& GetPrefabAssetRef() const
// 	{
// 		ASSERT(IsPrefab()) //must be a prefab
// 		return m_PrefabAssetReference;
// 	}

// 	AssetRef<PrefabAsset>& GetPrefabAssetRefMutable() 
// 	{
// 		ASSERT(IsPrefab()) //must be a prefab
// 		return m_PrefabAssetReference;
// 	}

	Ref<TransformComponent> GetRootComponent() const
	{
		return m_RootComponent;
	}

	void SetRootComponent(Ref<TransformComponent> NewRoot)
	{
		m_RootComponent = NewRoot;
	}

private:
	Scene* m_Scene = nullptr;

	//used to auto destroy on module unload
	Callback<EventModuleUnloaded> m_ModuleUnloadedEvent;

	//used to auto update from prefab assets
	//Callback<AssetEvent> m_AssetCallback;

	//array of owning components
	std::vector<Ref<ObjectComponent>> m_Components;

	//valid when we are a prefab
	//AssetRef<PrefabAsset> m_PrefabAssetReference;

	//ref to root component, could be null
	Ref<TransformComponent> m_RootComponent;
};