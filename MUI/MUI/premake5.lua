include "../../WangMauna/WangMauna"
include "../../MOGL/MOGL"
include "../../MFL/MFL"

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
		IncludeDir["MFL"],
		IncludeDir["MWL"], --[[ include only if MWL is backend --]]
		IncludeDir["MUI"]
	}

	links {
		"WangMauna",
		"MOGL",
		"MFL",
		"MWL" --[[ link only if MWL is backend --]]
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
