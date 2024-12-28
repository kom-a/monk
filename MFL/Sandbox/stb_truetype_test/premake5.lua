project "stb_test"
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
		IncludeDir["MWL"],
		IncludeDir["MOGL"]
	}

	links {
		"MFL",
		"MWL",
		"MOGL"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
