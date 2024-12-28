include "../../../Monk"
include "../../../MML/MML"
include "../../../MUI/MUI"

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
		IncludeDir["MFL"],
		IncludeDir["Scriptorium"],
		IncludeDir["MOGL"],
		IncludeDir["MWL"],
		IncludeDir["Monk"],
		IncludeDir["WangMauna"],
		IncludeDir["MML"]
	}

	links {
		"MFL",
		"Scriptorium",
		"MOGL",
		"MWL",
		"Monk",
		"WangMauna",
		"MML"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
