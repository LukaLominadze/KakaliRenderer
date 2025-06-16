project "Kakali"
	kind "ConsoleApp"
	staticruntime "off"

	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")

	defines { "GLEW_STATIC" }
	
	files {
		"src/**.h",
		"src/**.cpp"
	}

	vpaths {
		["Source Files"] = "src/**.cpp",
		["Header Files"] = "src/**.h"
	}

	includedirs {  "src",
				  "%{wks.location}/vendor/dependencies/glad/include",
				  "%{wks.location}/vendor/dependencies/glfw/include",
				  "%{wks.location}/vendor/dependencies/stb",
				  "%{wks.location}/vendor/dependencies/imgui",
				  "%{wks.location}/vendor/dependencies/glm",
				  "%{wks.location}/vendor/dependencies/assimp/include" }

	libdirs { "%{wks.location}/vendor/dependencies/libs/GLFW",
			  "%{wks.location}/vendor/dependencies/libs/STB",
			  "%{wks.location}/vendor/dependencies/libs/ImGui",
			  "%{wks.location}/vendor/dependencies/assimp/Debug" }

	links {
		"GLAD",
		"GLFW",
		"STB",
		"ImGui",
		"assimp-vc143-mtd.lib",
		"unit.lib"
		 }

	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	postbuildcommands {
		"{COPYDIR} %{wks.location}/vendor/dependencies/assimp/assimp-vc143-mtd.dll %{wks.location}/bin/" ..outputdir.. "/%{prj.name}"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "KR_PLATFORM_WINDOWS" }
		
		links { "opengl32.lib" }
	
	filter "system:linux"
		cppdialect "C++20"
		systemversion "latest"
		
		defines { "GLFW_USE_X11", "KR_PLATFORM_LINUX" }
		
		links { "GL",
			"pthread",
        		"dl" }

	filter "configurations:Debug"
		
		defines { "_DEBUG", "_CONSOLE" }
		
		architecture "x64"
		symbols "on"

	filter "configurations:Release"
		
		defines { "NDEBUG", "_CONSOLE" }

		architecture "x64"
		optimize "on"

	filter "configurations:Distribution"

		defines { "DISTRIBUTION", "_CONSOLE" }
	
		architecture "x64"
		optimize "on"
