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

//Event
#include "Event/Callback.h"
#include "Event/Event.h"
#include "Event/EventDispatcher.h"

//Misc
#include "Misc/Singleton.h"

//Framework
#include "Framework/Tick.h"
#include "Framework/Application.h"
#include "Framework/Engine.h"
#include "Framework/AppObject.h"
#include "Framework/ObjectBase.h"
#include "Framework/ModuleManager.h"
#include "Framework/Registry.h"
#include "Framework/Module.h"
#include "Framework/Property.h"
#include "Framework/ClassType.h"
#include "Framework/StaticClass.h"

//FrameworkClasses
#include "Framework/Classes/Properties/PropertyClasses.h"

//Rendering
#include "Rendering/RenderCore.h"

//UI
#include "UI/ImGui/ImGuiLayer.h"


