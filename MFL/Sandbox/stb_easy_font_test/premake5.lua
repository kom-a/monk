include "../../../MOGL/MOGL"
include "../../../MWL/MWL"

project "stb_easy_font_test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["MFL"],
		IncludeDir["MOGL"],
		IncludeDir["MWL"]
	}

	links {
		"MFL",
		"MOGL",
		"MWL"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
