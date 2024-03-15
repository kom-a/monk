project "MMath"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	characterset "Unicode"

	SetTargetDir()
	SetObjectDir()

	files {
		"src/**",
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Off"
