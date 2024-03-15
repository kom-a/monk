include "../../WangMauna/WangMauna"

project "MWL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	characterset "Unicode"

	SetTargetDir()
	SetObjectDir()

	environmentVariables = {
		"MYAPP_DATA=%{wks.location}"
	}

	debugenvs(environmentVariables)

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["WangMauna"],
		"./monk/src",
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
