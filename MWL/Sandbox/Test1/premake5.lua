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
		IncludeDir["MOGL"],
		IncludeDir["WangMauna"]
	}

	links {
		"MWL",
		"MOGL",
		"WangMauna"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
