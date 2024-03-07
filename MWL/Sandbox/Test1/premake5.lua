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
		IncludeDir["MWL"],
		IncludeDir["MOGL"]
	}

	links {
		"MWL",
		"MOGL"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
