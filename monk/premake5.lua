project "Monk"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   SetTargetDir()
   SetObjectDir()

   files {
      "monk/src/**",
      "monk/res/**"
   }

   includedirs {
      "./monk/src",
   }

   links {
      "opengl32"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      optimize "On"
      symbols "Off"
