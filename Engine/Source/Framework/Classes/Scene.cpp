#include "Scene.h"
#include "DEngine.h"

void Scene::OnBeginPlay()
{
	Super::OnBeginPlay();

	//if (GetApplication()->GetAppObject<PhysicsWorld>())
		//m_PhysicsScene = GetApplication()->GetAppObject<PhysicsWorld>()->CreatePhysicsScene();

	for (auto& obj : GetSceneObjects())
	{
		obj->OnBeginPlay();
	}
}

void Scene::OnEndPlay()
{
	Super::OnEndPlay();

	for (auto& obj : GetSceneObjects())
	{
		obj->OnEndPlay();
	}

	//SetActiveCamera(nullptr);

	//if (m_PhysicsScene)
	//	m_PhysicsScene = nullptr;
}

void Scene::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);

	//update scene objects
	for (uint i = 0; i < GetSceneObjects().size(); i++)
	{
		auto obj = GetSceneObjects()[i];

		if(CastCheck<SceneObject>(obj))
			obj->OnUpdate(tick);
	}

	SceneObjectCount.Text = STRING(GetSceneObjects().size());

	//simulate in game only
	//if(GetPhysicsScene() && tick.GetTickGroup() == TickGroup::GAME)
	//	GetPhysicsScene()->Simulate(tick.DeltaTime);
}

void Scene::OnConstruct()
{
	Super::OnConstruct();

// 	SceneEvent constructEvent;
// 	constructEvent.m_Scene = this;
// 	constructEvent.m_EventType = SceneEventType::SCENE_ONCONSTRUCT;
// 	constructEvent.m_ObjectClass = SceneObjectClass::SCENE;
// 	GetApplication()->GetEventDispatcher().Dispatch(constructEvent);

	m_ModuleUnloadEvent.Assign([&](EventModuleUnloaded& event)
	{
		if(event.ModuleName == GetAssociatedModuleName())
		{ 
			//unbind auto destruct from all components since its unnecessary now
			for (auto& obj : GetSceneObjects())
			{
				obj->m_ModuleUnloadedEvent.Destroy();
			}

			GetApplication()->DestroyAppObject(this);
		}
	});

	GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadEvent);
}

void Scene::OnDestroy()
{
	Super::OnDestroy();
	//GetPhysicsScene()->OnDestroy();

	//dont call destory twice
	m_ModuleUnloadEvent.Destroy();

	//call on destroy for all scene objects
	for (auto& obj : GetSceneObjects())
	{
		obj->OnDestroy();
	}
}

size_t Scene::Serialize(Buffer& buffer) const
{
	BufferWritter writter(buffer, Super::Serialize(buffer));
	BufferArray SceneObjectBuffer;
	for (auto& obj : GetSceneObjects())
	{
		Buffer FinalObjBuffer;
		BufferWritter ObjWritter(FinalObjBuffer);
		std::string ObjClassName = obj->GetClassType().Name;
		ObjWritter.WriteString(ObjClassName);
		Buffer ObjDataBuffer;
		obj->Serialize(ObjDataBuffer);
		ObjWritter.WriteBuffer(ObjDataBuffer);
		SceneObjectBuffer.AddPiece(FinalObjBuffer);
	}
	writter.WriteBuffer(SceneObjectBuffer.MakeBuffer());
	return writter.GetCurrentOffset();
}

size_t Scene::Deserialize(const Buffer& buffer)
{
	BufferReader reader(buffer, Super::Deserialize(buffer));
	Buffer SceneObjectBuffer;
	reader.ReadBuffer(SceneObjectBuffer);
	BufferArray SceneObjectBufferArray;
	SceneObjectBufferArray.FromBuffer(SceneObjectBuffer);

	for (uint i = 0; i < SceneObjectBufferArray.GetDataPieces().size(); i++)
	{
		const Buffer& piece = SceneObjectBufferArray.GetDataPieces()[i];
		BufferReader ObjReader(piece);
		std::string ObjClassName;
		ObjReader.ReadString(ObjClassName);
		ObjectBase* test = GET_SINGLETON(ObjectRegistry).MakeObjectFromClassName(ObjClassName);
		auto sceneObject = ToRef<SceneObject>(Cast<SceneObject>(test));
		if(!sceneObject) 
		{
			//either class is not registered (check if the associated module is loaded maybe) or the class name is invalid
			LOG_ERROR("Deserialized object class not registered : {0}" , ObjClassName);
			continue;
		}

		Buffer ObjBuffer;
		ObjReader.ReadBuffer(ObjBuffer);
		AddSceneObject(sceneObject, ObjectInitializer(ConstructFlags::NOPOSTCONSTRUCT | ConstructFlags::RANDOMID)); //dont call on post construct yet
		sceneObject->Deserialize(ObjBuffer);
		sceneObject->OnPostConstruct(); //now after deserializing call on post construct
	}
	return reader.GetCurrentOffset();
}

void Scene::DestroySceneObject(SceneObject* obj)
{
	auto remove = m_SceneObjects.end();
	for (auto it = m_SceneObjects.begin(); it != m_SceneObjects.end(); it++)
	{
		if (obj == (*it).get())
		{
			remove = it;
			break;
		}
	}

	if (remove != m_SceneObjects.end())
	{
		EventScenePreDeleteObject event;
		event.Object = obj;

		//Dispatch PRE_DELETE event
		GetSceneEventDipatcher().Dispatch(event);

		//call on destroy
		(*remove)->OnDestroy();
		m_SceneObjects.erase(remove);

		//Dispatch POST_DELETE event
		EventScenePostDeleteObject event2;
		event2.Object = obj;
		GetSceneEventDipatcher().Dispatch(event2);
	}
}

void Scene::DestroySceneObject(Ref<SceneObject> obj)
{
	EventScenePreDeleteObject event;
	event.Object = obj.get();

	//Dispatch PRE_DELETE event
	GetSceneEventDipatcher().Dispatch(event);

	//call on destroy
	obj->OnDestroy();

	//finally erase
	m_SceneObjects.erase(std::find(m_SceneObjects.begin(), m_SceneObjects.end(), obj));

	//Dispatch POST_DELETE event
	EventScenePostDeleteObject event2;
	event2.Object = obj.get();
	GetSceneEventDipatcher().Dispatch(event2);
}

void Scene::AddSceneObject(SceneObject* obj, const ObjectInitializer& Initializer)
{
	ASSERT(obj);

	EventScenePreInitializeObject event;
	event.Object = obj;
	//Dispatch PRE_INITIALIZE event
	GetSceneEventDipatcher().Dispatch(event);

	//assign scene and add to scene object array
	obj->m_Scene = (this);
	Ref<SceneObject> objref = ToRef<SceneObject>(obj);
	m_SceneObjects.push_back(objref);

	//Must call
	if(!obj->IsInitialized())
		obj->Initialize(Initializer);

	//Dispatch POST_INITIALIZE event
	EventScenePostInitializeObject event2;
	event2.Object = obj;
	GetSceneEventDipatcher().Dispatch(event2);
}

void Scene::AddSceneObject(Ref<SceneObject> obj, const ObjectInitializer& Initializer)
{
	ASSERT(obj);

	EventScenePreInitializeObject event;
	event.Object = obj.get();
	//Dispatch PRE_INITIALIZE event
	GetSceneEventDipatcher().Dispatch(event);

	//assign scene and add to scene object array
	obj->m_Scene = (this);
	m_SceneObjects.push_back(obj);

	//Must call
	if (!obj->IsInitialized())
		obj->Initialize(Initializer);

	//Dispatch POST_INITIALIZE event
	EventScenePostInitializeObject event2;
	event2.Object = obj.get();
	GetSceneEventDipatcher().Dispatch(event2);
}

// void Scene::CreatePhysicsScene(PhysicsWorld* worldContext)
// {
// 	m_PhysicsScene = worldContext->CreatePhysicsScene();
// }
