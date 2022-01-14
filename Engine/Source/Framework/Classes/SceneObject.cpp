#include "SceneObject.h"
#include "DEngine.h"


void SceneObject::OnBeginPlay()
{
	for (auto& comp : GetComponents())
	{
		comp->OnBeginPlay();
	}
}

void SceneObject::OnEndPlay()
{
	for (auto& comp : GetComponents())
	{
		comp->OnEndPlay();
	}
}

void SceneObject::OnDestroy()
{
	Super::OnDestroy();

	//dont call destroy twice
	m_ModuleUnloadedEvent.Destroy();

	const auto& comps = GetComponents();
	for (auto& it = comps.begin(); it != comps.end();)
	{
		it = DestroyComponent(*it);
	}
}

void SceneObject::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);

	//update components
	for (uint i = 0; i < GetComponents().size(); i++)
	{
		auto obj = GetComponents()[i];

		if (CastCheck<ObjectComponent>(obj))
			obj->OnUpdate(tick);
	}
}


void SceneObject::OnConstruct()
{
	Super::OnConstruct();

	//auto destroy object when its module is unloaded
	m_ModuleUnloadedEvent.Assign([&](EventModuleUnloaded& event)
	{
		if (event.ModuleName == GetAssociatedModuleName())
		{
			//prevent auto delete if scene was deleted prior
			if (!m_ModuleUnloadedEvent.m_OnTrigger) return;

			//unbind auto destruct from all components since its unnecessary now
			for (auto& comp : GetComponents())
			{
				comp->m_ModuleUnloadedEvent.Destroy();
			}

			GetScene()->DestroySceneObject(this);
		}
	});

	GetScene()->GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadedEvent);

#if 0
	//auto load prefab on change
	m_AssetCallback.Assign([&](AssetEvent* event)
	{
		if (IsPrefab() && event->GetEventType() == AssetEventType::ASSETSAVE)
		{
			Ref<PrefabAsset> prefabAsset = Cast<PrefabAsset>(event->GetAsset());
			if (prefabAsset && event->GetAsset()->GetID() == GetPrefabAssetRefMutable().GetAssetHandle()->GetAssetID())
			{
				Buffer MyBuffer; //save myself then load later after the prefab loaded
				Serialize(MyBuffer);

				prefabAsset->LoadPrefab(this, false);

				Deserialize(MyBuffer);
			}
		}
	});

	GetScene()->GetApplication()->GetAssetManager().GetEventDispatcher().Bind(m_AssetCallback);
#endif
}

void SceneObject::OnPostConstruct()
{
	//call on post construct for all components
	for (auto& comp : GetComponents())
	{
		comp->OnPostConstruct();
	}
}	

size_t SceneObject::Serialize(Buffer& buffer) const
{
	BufferWritter writter(buffer, Super::Serialize(buffer));

	//prefab asset ref (still need to serialize an empty buffer even if we arent a prefab)
	Buffer PrefabFileBuffer;
	//if(IsPrefab())
	//	GetPrefabAssetRef().Serialize(PrefabFileBuffer);
	//WRITEBUFFER(PrefabFileBuffer);
	
	BufferArray ComponentsData;
	for (auto& comp : GetComponents())
	{		
		//write ID and Class name for finding the same component on deserialize
		Buffer CompPiece;
		BufferWritter CompWritter(CompPiece);

		CompWritter.Write(&comp->GetID(), sizeof(UID));
		std::string compClassName = comp->GetClassType().Name;
		std::string compName = comp->GetName(); //used as the unique id for deserialization as ID's are generated on construction
		CompWritter.WriteString(compClassName);
		CompWritter.WriteString(compName);

		//write actual component data
		Buffer compBuffer;
		comp->Serialize(compBuffer);
		CompWritter.WriteBuffer(compBuffer);

		ComponentsData.AddPiece(CompPiece);
	}
	writter.WriteBuffer(ComponentsData.MakeBuffer());

	return writter.GetCurrentOffset();
}

size_t SceneObject::Deserialize(const Buffer& buffer)
{
	BufferReader reader(buffer, Super::Deserialize(buffer));

	//prefab asset ref (still need to serialize an empty buffer even if we arent a prefab)
	Buffer PrefabFileBuffer;
	//READBUFFER(PrefabFileBuffer);
	//if (IsPrefab())
	//	GetPrefabAssetRefMutable().Deserialize(PrefabFileBuffer);
	
	Buffer rawComponentsData;
	reader.ReadBuffer(rawComponentsData);

	BufferArray ComponentsData;
	ComponentsData.FromBuffer(rawComponentsData);

	for (auto& piece : ComponentsData.m_DataPieces)
	{
		UID compID;
		std::string compClassName;
		std::string compName;
		Buffer compData;

		BufferReader CompReader(compData);
		CompReader.Read(&compID, sizeof(UID));
		CompReader.ReadString(compClassName);
		CompReader.ReadString(compName);
		CompReader.ReadBuffer(compData);

		//now find a corresponding component and deserialize
		for (auto& comp : GetComponents())
		{
			if(comp->GetName() == compName && comp->GetClassType().Name == compClassName)	
			{ 
				comp->Deserialize(compData);
			}
		}
	}

	return reader.GetCurrentOffset();
}

std::vector<Ref<ObjectComponent>>::iterator SceneObject::DestroyComponent(Ref<ObjectComponent> comp)
{
	//prevent component from deleting itself twice
	comp->m_ModuleUnloadedEvent.Destroy();

	//call on destroy
	comp->OnDestroy();

	//clear root if this was the root
	if(comp == GetRootComponent())
		m_RootComponent = nullptr;

	return m_Components.erase(std::find(m_Components.begin(), m_Components.end(), comp));
}

void SceneObject::DestroyComponent(ObjectComponent* comp)
{
	auto remove = m_Components.end();
	for (auto it = m_Components.begin(); it != m_Components.end(); it++)
	{
		if (comp == (*it).get())
		{
			remove = it;
			break;
		}
	}

	if (remove != m_Components.end())
	{
		//prevent component from deleting itself twice
		(*remove)->m_ModuleUnloadedEvent.Destroy();

		//call on destory for the component
		(*remove)->OnDestroy();

		//finally erase
		m_Components.erase(remove);
	}
}

void SceneObject::AddComponent(ObjectComponent* comp)
{
	ASSERT(comp);

	//assign owner and add to object array
	comp->m_Owner = (this);
	Ref<ObjectComponent> compref = ToRef<ObjectComponent>(comp);
	m_Components.push_back(compref);

	//Must call
	comp->Initialize(ObjectInitializer(ConstructFlags::RANDOMID));
}
