workspace "WangMauna_Sandbox"
	configurations { "Debug", "Release" }
	architecture "x86_64"

include "../../Common.lua"

include "Test1"
include "Test2"
include "Test2_Lib"

include "../WangMauna"