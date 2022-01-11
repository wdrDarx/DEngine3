project "Optick"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/*.*",
		"src/*.*"
	}

	defines
	{
		"OPTICK_ENABLE_GPU_VULKAN=0"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"

	filter "configurations:Runtime"
		runtime "Release"
		optimize "Speed"
