include "../../../WangMauna/WangMauna"
include "../../../MWL/MWL"
include "../../../MOGL/MOGL"
include "../../MUI"

project "Test1"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["WangMauna"],	
		IncludeDir["MWL"],
		IncludeDir["MOGL"],
		IncludeDir["MUI"],
	}

	links {
		"MWL",
		"MOGL",
		"WangMauna",
		"MUI"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
