workspace "CineOchh"
    configurations { "Debug", "Release" }
    location "build"

project "CineOchh"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}" 
    
    files {
        "src/**.cpp",
        "include/**.h",
        "assets/**"
    }
    
    includedirs {
        "include",
        "lib/json/include",
        "lib/crypto"
    }
    
    links { "crypto" }

    filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

 filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    
project "crypto"
    kind "StaticLib"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
  
    files { "lib/crypto/**.cpp", "lib/crypto/**.h" }
    includedirs { "include" }
  
    filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
  
    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On" 