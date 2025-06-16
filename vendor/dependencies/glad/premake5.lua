project "GLAD"
    kind "StaticLib"  -- or "SharedLib" if you want shared
    language "C"

    targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")

    -- Include directory
    includedirs { "include" }

    -- Sources (adjust paths as needed)
    files {
        "include/glad/glad.h",
        "src/glad.c"
    }

    -- Defines (example)
    defines { "GLAD_GLAPI_EXPORT" }

    filter "system:windows"
        -- If using MSVC and want static runtime (like CMake's /MT)
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter "configurations:Distribution"
        optimize "On"
        defines { "NDEBUG" }

    -- Custom prebuild step to run Python glad generator if needed
    prebuildcommands {
        -- This assumes python and glad module are installed and in PATH
        "python -m glad --profile=compatibility --out-path=include --api= --generator=c"
    }
