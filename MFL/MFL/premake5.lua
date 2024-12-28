include "../../Scriptorium/Scriptorium"

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
		"WangMauna"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
