workspace "Scriptorium_Sandbox"
	configurations { "Debug", "Release" }
	architecture "x86_64"

    startproject "Test1"

include "../../Common.lua"

include "../Scriptorium"
include "Test1"
