workspace "MFL_Sandbox"
	configurations { "Debug", "Release" }
	architecture "x86_64"

include "../../Common.lua"

include "stb_truetype_test"
include "stb_easy_font_test"
include "Test1"

include "../MFL"
