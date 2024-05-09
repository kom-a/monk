include "../../WangMauna/WangMauna"

project "MWL"
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
		IncludeDir["WangMauna"]
	}

	links {
		"WangMauna",
		"opengl32",
		"UxTheme"
	}

	filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"

	filter "configurations:Release"
	optimize "On"
	symbols "Off"
