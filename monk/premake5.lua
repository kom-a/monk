project "Monk"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   SetTargetDir()
   SetObjectDir()

   files {
      "src/**"
   }

   includedirs {
       IncludeDir["WangMauna"],
       IncludeDir["MWL"],
       IncludeDir["MOGL"]
   }

   links {
       "WangMauna",
       "MWL",
       "MOGL",
      "opengl32"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      optimize "On"
      symbols "Off"
