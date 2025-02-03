include "../Test2_Lib"

project "Test2"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"

    SetTargetDir()
	SetObjectDir()

    files {
		"src/**",
	}

    includedirs {
		
	}

    links {
		"WangMauna"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        optimize "On"
        symbols "Off"
