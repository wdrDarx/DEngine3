project "GameFramework"
	location "Modules/GameFramework"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Modules/%{prj.name}/Source/**.h",
		"Modules/%{prj.name}/Source/**.cpp"
	}

	postbuildcommands
	{
		("{MKDIR} %{cfg.buildtarget.relpath}/../../Editor/Modules/%{prj.name}"),
		("{COPY} %{cfg.buildtarget.relpath} ../../bin/" .. outputdir .. "/Editor/Modules/%{prj.name}" )		
	}

	includedirs
	{
		"Engine/Source",
		"Modules/%{prj.name}/Source",
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

project "Project"
	location "Modules/Project"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Modules/%{prj.name}/Source/**.h",
		"Modules/%{prj.name}/Source/**.cpp"
	}

	postbuildcommands
	{
		("{MKDIR} %{cfg.buildtarget.relpath}/../../Editor/Modules/%{prj.name}"),
		("{COPY} %{cfg.buildtarget.relpath} ../../bin/" .. outputdir .. "/Editor/Modules/%{prj.name}" )		
	}

	includedirs
	{
		"Engine/Source",
		"Modules/GameFramework/Source",
		"Modules/%{prj.name}/Source",
		IncludeDir["GLFW"],
		IncludeDir["GLAD"],
		IncludeDir["ImGui"],
		IncludeDir["GLM"],
		IncludeDir["Objloader"],
		IncludeDir["STB_IMAGE"],
		IncludeDir["ASSIMP"],
		IncludeDir["OpenVR"],
		IncludeDir["Json"],
		IncludeDir["PhysX"]
	}

	links
	{
		"Engine",
		"GameFramework"
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