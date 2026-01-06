include "../WangMauna/WangMauna"
include "../MWL/MWL"
include "../MOGL/MOGL"
include "../MML/MML"
include "../MUI/MUI"
include "../MFL/MFL"
include "../Scriptorium/Scriptorium"

local buildMonkStatic = _OPTIONS["monk_static"] ~= nil

project "Monk"
   kind ("StaticLib")
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

   if buildMonkStatic then
      defines { "MONK_STATIC" }
   end

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      optimize "On"
      symbols "Off"
