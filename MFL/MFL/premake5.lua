include "../../Scriptorium/Scriptorium"
include "../../WangMauna/WangMauna"

project "MFL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    
	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["Scriptorium"],
		IncludeDir["WangMauna"]
	}

	links {
		"Scriptorium",
		"WangMauna"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
