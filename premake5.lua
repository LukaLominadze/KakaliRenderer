workspace "KakaliRenderer"
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	architecture "x64"

	startproject "Kakali"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Core"
	include "Kakali"
group ""

group "Dependencies"
	include "vendor/dependencies/glfw"
	include "vendor/dependencies/glad"
	include "vendor/dependencies/stb"
	include "vendor/dependencies/imgui"
group ""

