include "../../WangMauna/WangMauna"
include "../../MOGL/MOGL"

project "MUI"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	characterset "Unicode"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["WangMauna"],
		IncludeDir["MOGL"],
	}

	links {
		"WangMauna",
		"MOGL"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
