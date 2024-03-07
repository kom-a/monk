workspace "MWL_Sandbox"
	configurations { "Debug", "Release" }
	architecture "x86_64"

include "../../Common.lua"

include "Test1"

include "../MWL"
include "../../MOGL/MOGL"
