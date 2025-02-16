include "../WangMauna/WangMauna"
include "../MWL/MWL"
include "../MOGL/MOGL"
include "../MML/MML"
include "../MUI/MUI"
include "../MFL/MFL"
include "../Scriptorium/Scriptorium"

project "Monk"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"

   SetTargetDir()
   SetObjectDir()

   files {
      "src/**"
   }

   removefiles {
       "src/utils/**",
       "src/gui/**"
   }

   includedirs {
       IncludeDir["WangMauna"],
       IncludeDir["MWL"],
       IncludeDir["MOGL"],
       IncludeDir["MML"],
       IncludeDir["MUI"],
       IncludeDir["MFL"],
       IncludeDir["Scriptorium"],

       "src/"
   }

   links {
       "WangMauna",
       "MWL",
       "MOGL",
       "MML",
       "MUI",
       "MFL",
       "Scriptorium",
      "opengl32"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      optimize "On"
      symbols "Off"
