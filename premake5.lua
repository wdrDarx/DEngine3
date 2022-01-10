workspace "DEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Runtime"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	startproject "Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"	

group "Dependecies"
include "Vendor/GLFW"
include "Vendor/ImGui"
include "Vendor/GLAD"
include "Vendor/Json"
group "" --Dependecies

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		IncludeDir["GLFW"],
		IncludeDir["GLAD"],
		IncludeDir["ImGui"],
		IncludeDir["GLM"],
		IncludeDir["Objloader"],
		IncludeDir["STB_IMAGE"],
		IncludeDir["ASSIMP"],
		IncludeDir["Json"],
		IncludeDir["PhysX"]
	}


	links
	{			
		Library["PhysX"],
		Library["PhysXCharacterKinematic"],
		Library["PhysXCommon"],
		Library["PhysXCooking"],
		Library["PhysXExtensions"],
		Library["PhysXFoundation"],
		Library["PhysXPvd"],

		Library["Assimp"],
		Library["Zlib"],
		Library["IRRXLM"],

		"opengl32.lib",
		"imGui",
		"Json",
		"GLFW",
		"GLAD"
	}

	defines
	{
		"DENGINE_EXPORT", 
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS",
		"PX_PHYSX_STATIC_LIB"
	}

	filter "configurations:Debug"
      symbols "On"

	filter "configurations:Release"
      optimize "On"
	  symbols "On"
	  defines
	  {
			"NDEBUG" --for physx
	  }

	filter "configurations:Runtime"
      optimize "Speed"
	  defines
	  {
			"NDEBUG" --for physx
	  }

group "Applications"

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Engine/Source",
		"%{prj.name}/Source",
		IncludeDir["GLFW"],
		IncludeDir["GLAD"],
		IncludeDir["ImGui"],
		IncludeDir["GLM"],
		IncludeDir["Objloader"],
		IncludeDir["STB_IMAGE"],
		IncludeDir["ASSIMP"],
		IncludeDir["Json"],
		IncludeDir["PhysX"]
	}

	defines
	{
		"STB_IMAGE_IMPLEMENTATION"
	}

	links
	{
		"Engine"
	}

	postbuildcommands
	{
		"{MKDIR} %{cfg.buildtarget.relpath}/../Modules",
		"{MKDIR} %{cfg.buildtarget.relpath}/../Content",
		"{MKDIR} %{cfg.buildtarget.relpath}/../Engine",
		("{COPY} ../EngineContent/  %{cfg.buildtarget.relpath}/../Engine")
	}

	filter "configurations:Debug"
      symbols "On"

	filter "configurations:Release"
      optimize "On"
	  symbols "On"
	  defines
	  {
			"NDEBUG" --for physx
	  }

	filter "configurations:Runtime"
      optimize "Speed"
	  defines
	  {
			"NDEBUG" --for physx
	  }

project "Runtime"
	location "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Engine/Source",
		IncludeDir["GLFW"],
		IncludeDir["GLAD"],
		IncludeDir["ImGui"],
		IncludeDir["GLM"],
		IncludeDir["Objloader"],
		IncludeDir["STB_IMAGE"],
		IncludeDir["ASSIMP"],
		IncludeDir["Json"],
		IncludeDir["PhysX"]
	}

	defines
	{
		"STB_IMAGE_IMPLEMENTATION",
		"DENGINE_RUNTIME"
	}

	links
	{
		"Engine"
	}

	filter "configurations:Debug"
      symbols "On"

	filter "configurations:Release"
      optimize "On"
	  symbols "On"
	  defines
	  {
			"NDEBUG" --for physx
	  }

	filter "configurations:Runtime"
      optimize "Speed"
	  defines
	  {
			"NDEBUG" --for physx
	  }

group "" --Applications

group "Modules"
include "Modules.lua"
group "" --Modules