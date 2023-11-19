project "WangMauna"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
