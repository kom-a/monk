include "../../Nun"
include "../../../MOGL/MOGL"
include "../../../MWL/MWL"

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
		IncludeDir["MML"],
		IncludeDir["Nun"],
		IncludeDir["MOGL"],
		IncludeDir["MWL"]
	}

	links {
		"Nun",
		"MOGL",
		"MWL"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
