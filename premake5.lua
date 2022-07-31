workspace "monk"
   configurations { "Debug", "Release" }
   architecture "x86_64"

outputdir = "%{cfg.architecture}-%{cfg.buildcfg}"

project "monk"
   location "monk"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   targetdir ("bin/" .. outputdir)
   objdir ("bin-int/" .. outputdir)

   characterset ("ASCII")

   files {
      "monk/src/**"
   }

   includedirs {
      "./monk/src",
      "./lib/include"
   }

   defines {
       "GLEW_STATIC"
   }

   libdirs {
       "lib/"
   }

   links {
       "glew32s.lib",
       "opengl32.lib"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      optimize "On"
      symbols "Off"
