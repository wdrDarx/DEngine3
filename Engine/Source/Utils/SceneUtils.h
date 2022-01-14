#pragma once
#include "Core/Core.h"
#include "Framework/Classes/Scene.h"
#include "Framework/Classes/SceneObject.h"
//#include "Assets/Classes/PrefabAsset.h"
//#include "Assets/Classes/SceneAsset.h"
//#include "Assets/AssetRef.h"


/*
	Util class for scene actions
*/
class SceneUtils
{
public:

	/*
		Loads a prefab from an asset and spawns it in the specified scene at the speciefied transform
	*/
	//static Ref<SceneObject> SpawnPrefabInScene(AssetRef<PrefabAsset> prefabAsset, Scene* scene, const Transform& transform, ObjectInitializer initializer, bool IgnoreScale = true);

	/*
		Wipes a scene fully before loading in all the data from the asset
	*/
	//static void LoadSceneFromAsset(Ref<SceneAsset> sceneAsset, Scene* scene);

	/*
		clones a scene object data to a new object
		returns the new cloned object
	*/
	static Ref<SceneObject> CloneSceneObject(SceneObject* obj, Scene* TargetScene);

	/*
		Creates a scene from a custom class defined in the asset and loads the scene data
	*/
	//static Ref<Scene>  CreateAndLoadSceneFromAsset(Application* app, const Ref<SceneAsset>& sceneAsset);

};