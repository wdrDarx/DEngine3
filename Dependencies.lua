
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Vendor/ImGui"
IncludeDir["GLM"] = "%{wks.location}/Vendor/GLM"
IncludeDir["Objloader"] = "%{wks.location}/Vendor/ObjLoader"
IncludeDir["STB_IMAGE"] = "%{wks.location}/Vendor/stb_image"
IncludeDir["ASSIMP"] = "%{wks.location}/Vendor/Assimp/include"
IncludeDir["OpenVR"] = "%{wks.location}/Vendor/OpenVR/include"
IncludeDir["PhysX"] = "%{wks.location}/Vendor/PhysX/include"
IncludeDir["Json"] = "%{wks.location}/Vendor/Json"

LibraryDir = {}
LibraryDir["PhysX"] = "%{wks.location}/vendor/PhysX/lib/%{cfg.buildcfg}"
LibraryDir["Assimp"] = "%{wks.location}/vendor/Assimp/lib/%{cfg.buildcfg}"

Library = {}
Library["PhysX"] = "%{LibraryDir.PhysX}/PhysX_static_64.lib"
Library["PhysXCharacterKinematic"] = "%{LibraryDir.PhysX}/PhysXCharacterKinematic_static_64.lib"
Library["PhysXCommon"] = "%{LibraryDir.PhysX}/PhysXCommon_static_64.lib"
Library["PhysXCooking"] = "%{LibraryDir.PhysX}/PhysXCooking_static_64.lib"
Library["PhysXExtensions"] = "%{LibraryDir.PhysX}/PhysXExtensions_static_64.lib"
Library["PhysXFoundation"] = "%{LibraryDir.PhysX}/PhysXFoundation_static_64.lib"
Library["PhysXPvd"] = "%{LibraryDir.PhysX}/PhysXPvdSDK_static_64.lib"

--Library["OpenVR"] = "%{wks.location}/Vendor/OpenVR/libs/openvr_api.lib"

Library["Assimp"] = "%{LibraryDir.Assimp}/assimp.lib"
Library["Zlib"] = "%{wks.location}/Vendor/zlib/lib/zlibstatic.lib"
Library["IRRXLM"] = "%{wks.location}/Vendor/irrXML/lib/IrrXML.lib" 	
