DX9SDK_DIR = os.getenv("DXSDK_DIR")
workspace "CLEOAddon"
    configurations { 
        "Debug", 
        "Release" 
    }

    platforms {
        "Win32"
    }
    language "C++"
    cppdialect "C++latest"
    characterset "MBCS"
    staticruntime "On"
    location "../build"
    targetdir "../build/bin"
    kind "SharedLib"
    targetextension ".cleo"

    files { 
        "../include/**", 
        "../src/**",
        DX9SDK_DIR .. "Include\\**"
    }
    
    includedirs {
        "../include/**",
        DX9SDK_DIR .. "Include\\**"
    }

    libdirs {
        "../lib/",
        DX9SDK_DIR .. "Include\\**"
    }

project "CLEOAddon"
    filter { "platforms:Win32" }
        targetname "CLEOAddon"
        architecture "x86"
        links { 
            "cleo_redux",
            "urlmon",
            "d3dx9"
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
        defines {
            "NDEBUG"
        }