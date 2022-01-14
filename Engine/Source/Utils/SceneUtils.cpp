#include "SceneUtils.h"
#include "DEngine.h"

// Ref<SceneObject> SceneUtils::SpawnPrefabInScene(AssetRef<PrefabAsset> prefabAsset, Scene* scene, const Transform& WorldTransform, ObjectInitializer initializer, bool IgnoreScale)
// {
// 	auto loadedPrefab = scene->GetApplication()->GetAssetManager().LoadAsset(prefabAsset);
// 	if(!loadedPrefab) 
// 	{
// 		LogError("Prefab Asset Invalid for spawn!");
// 		return nullptr;
// 	}
// 
// 	SceneObject* created = Cast<SceneObject>(scene->GetApplication()->GetObjectRegistry().MakeObjectFromClassName(loadedPrefab->GetPrefabBaseClassName()));
// 	if(!created)
// 	{
// 		LogError("Prefab class invalid or not registered!");
// 		return nullptr;
// 	}
// 	auto sceneObject = ToRef<SceneObject>(created);
// 
// 	//initialize the object by calling DefineProperties and OnConstruct so all the props and components are there 
// 	initializer.Flags |= ConstructFlags::NOPOSTCONSTRUCT;
// 	initializer.Flags |= ConstructFlags::RANDOMID;
// 	scene->AddSceneObject(sceneObject, initializer);
// 
// 	//load the prefab data
// 	loadedPrefab->LoadPrefab(sceneObject, false, false);
// 
// 	//mark a prefab
// 	sceneObject->MarkPrefab(prefabAsset);
// 
// 	//and now after being deserialized call on post construct 
// 	sceneObject->OnPostConstruct();
// 	
// 	//set root transform if there is a root
// 	if (auto root = sceneObject->GetRootComponent())
// 	{
// 		//ignore scale?
// 		Transform SpawnTransform = WorldTransform;
// 		if (IgnoreScale)
// 			SpawnTransform.scale = root->GetLocalScale();
// 
// 		root->SetWorldTransform(WorldTransform);
// 	}
// 	
// 	//call on begin play if the game is running
// 	if(scene->GetApplication()->GetAppState() == AppState::GAME)
// 		sceneObject->OnBeginPlay();
// 
// 	return sceneObject;
// }

// void SceneUtils::LoadSceneFromAsset(Ref<SceneAsset> sceneAsset, Scene* scene)
// {
// 	if (!scene || !sceneAsset) return;
// 	scene->DestroyAllSceneObjects();
// 	scene->GetInputManager().ClearInput();
// 
// 	sceneAsset->LoadScene(scene);
// 	
// 	if (scene->GetApplication()->GetAppState() == AppState::GAME)
// 	{
// 		for (auto& obj : scene->GetSceneObjects())
// 		{
// 			obj->OnBeginPlay();
// 		}
// 	}
// }

// Ref<SceneObject> SceneUtils::CloneSceneObject(SceneObject* obj, Scene* TargetScene)
// {
// 	ObjectInitializer temp = obj->GetObjectInitializer();
// 	Ref<PrefabAsset> tempasset = MakeRef<PrefabAsset>();
// 	tempasset->SavePrefab(obj);
// 	tempasset->m_SceneObjectClassName = obj->GetClassType().Name;
// 	
// 	auto sceneObject = ToRef<SceneObject>(Cast<SceneObject>(TargetScene->GetApplication()->GetObjectRegistry().MakeObjectFromClassName(tempasset->GetPrefabBaseClassName())));
// 	if (!sceneObject) return nullptr;
// 
// 	//initialize the object calling DefineProperties and OnConstruct so all the props and components are there 
// 	temp.Flags |= ConstructFlags::NOPOSTCONSTRUCT;
// 	temp.Flags |= ConstructFlags::RANDOMID;
// 	TargetScene->AddSceneObject(sceneObject, temp);
// 
// 	//load the prefab data
// 	tempasset->LoadPrefab(sceneObject, false);
// 
// 	//mark a prefab if og object is a prefab
// 	if(obj->IsPrefab())
// 		sceneObject->MarkPrefab(obj->GetPrefabAssetRef());
// 
// 	//and now after being deserialized call on post construct 
// 	sceneObject->OnPostConstruct();
// 
// 	//set root transform if there is a root
// 	if (auto root = sceneObject->GetRootComponent())
// 		root->SetWorldTransform(obj->GetRootComponent()->GetWorldTransform());
// 
// 	return sceneObject;
// }

// Ref<Scene> SceneUtils::CreateAndLoadSceneFromAsset(Application* app, const Ref<SceneAsset>& sceneAsset)
// {
// 	Scene* out = Cast<Scene>(app->GetObjectRegistry().MakeObjectFromClassName(sceneAsset->m_SceneClassName));
// 	if (!out)
// 	{
// 		LogError("Scene Class unregistered or invalid!");
// 		return nullptr;
// 	}
// 	auto scene = app->AddAppObject(out);
// 	
// 	Pipeline* pipeline = Cast<Pipeline>(app->GetObjectRegistry().MakeObjectFromClassName(sceneAsset->m_PipelineClassName));
// 	pipeline->Create(out, app->GetWindow()->GetRenderAPI());
// 	if (!pipeline)
// 	{
// 		LogError("Pipeline Class unregistered or invalid!");
// 		return nullptr;
// 	}
// 	out->SetPipeline(pipeline, app->GetWindow()->GetRenderAPI());
// 
// 	LoadSceneFromAsset(sceneAsset, out);
// 	return Cast<Scene>(scene);
// }
