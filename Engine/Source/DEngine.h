#pragma once

//Dependencies
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "json/json.h"

//Core
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Profiling.h"

//Utils
#include "Utils/Rand.h"
#include "Utils/Timer.h"
#include "Utils/ModuleUtils.h"
#include "Utils/Paths.h"
#include "Utils/VectorUtils.h"
#include "Utils/FileSystem.h"
#include "Utils/ClassUtils.h"
#include "Utils/ObjectUtils.h"
#include "Utils/InputUtils.h"
#include "Utils/RenderUtils.h"
#include "Utils/SceneUtils.h"

//Event
#include "Event/Callback.h"
#include "Event/Event.h"
#include "Event/EventDispatcher.h"

//Misc
#include "Misc/Singleton.h"
#include "Misc/SafeObjectPtr.h"

//Framework
#include "Framework/Tick.h"
#include "Framework/Application.h"
#include "Framework/Engine.h"
#include "Framework/ObjectBase.h"
#include "Framework/ModuleManager.h"
#include "Framework/Registry.h"
#include "Framework/Module.h"
#include "Framework/Property.h"
#include "Framework/ClassType.h"
#include "Framework/StaticClass.h"
#include "Framework/Method.h"
#include "Framework/StructBase.h"

//FrameworkClasses
#include "Framework/Classes/Properties/PropertyClasses.h"
#include "Framework/Classes/AppObject.h"
#include "Framework/Classes/Scene.h"
#include "Framework/Classes/SceneObject.h"
#include "Framework/Classes/ObjectComponent.h"
#include "Framework/Classes/InputManager.h"

//Components
#include "Framework/Classes/Components/TransformComponent.h"

//Rendering
#include "Rendering/RenderCore.h"

//Rendering Classes
#include "Rendering/Classes/Pipelines/DefaultPipeline.h"

//UI
#include "UI/ImGui/ImGuiLayer.h"


