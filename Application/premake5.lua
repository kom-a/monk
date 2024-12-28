include "../../Monk/Monk"

project "Application"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	characterset "Unicode"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	includedirs {
		IncludeDir["Monk"],
		IncludeDir["MML"],
		IncludeDir["MOGL"],
		IncludeDir["WangMauna"],
		IncludeDir["MFL"],
		IncludeDir["Scriptorium"]
	}

	links {
		"Monk"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
