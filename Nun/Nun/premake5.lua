include "../../Jsonetic/Jsonetic"
include "../../Scriptorium/Scriptorium"
include "../../MML/MML"
include "../../WangMauna/WangMauna"

project "Nun"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    
	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["Jsonetic"],
		IncludeDir["Scriptorium"],
		IncludeDir["MML"],
		IncludeDir["WangMauna"],
	}

	links {
		"Jsonetic",
		"Scriptorium",
		"MML",
		"WangMauna"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
