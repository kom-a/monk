workspace "Monk"
	configurations { "Debug", "Release" }
	architecture "x86_64"

newoption {
   trigger     = "monk_static",
   description = "Build Monk as a static library"
}

newoption {
   trigger     = "monk_dynamic",
   description = "Build Monk as a dynamic library"
}

include "Common.lua"

include "WangMauna/WangMauna"
include "MWL/MWL"
include "MOGL/MOGL"
include "MML/MML"
include "MUI/MUI"
include "MFL/MFL"
include "Monk"
include "Application"