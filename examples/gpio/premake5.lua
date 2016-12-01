----------------------------------------------------------------------------------   
-- define compillers
dofile( "tools/arm_gcc.lua" )("gcc5", "", "-5")
dofile( "tools/arm_gcc.lua" )("arm_gcc", os.getenv("MICROHAL_TOOLCHAIN"), "")

require( "microhal.lua" )
local microhal = premake.extensions.microhal

workspace "microhal_examples"
   configurations { "Debug", "Release" }
   platforms { "stm32f407vgt6" }

project "gpio"
   	kind "ConsoleApp"
   	language "C++"
   	targetdir "build/bin/%{cfg.platform}/%{cfg.buildcfg}"
   	objdir "build/obj"

   	flags { "C++14" }   
   	
	microhal.enable()
   	microhalmodules { "core" }
   	   	
   	files { "*.h", "*.cpp" }         
   
	filter { "platforms:stm32f407vgt6" }
    --	architecture "arm"  
    	toolset "arm_gcc"   	
    	buildoptions { "-mcpu=cortex-m4", "-mthumb", "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16" }
    	linkoptions { "-mcpu=cortex-m4", "-mthumb", "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16", "-T STM32F4xxxG_rom.ld", "-ffreestanding", "-fno-builtin", "-Xlinker --gc-sections", "-L\"/home/pawel/Dokumenty/microHAL_workspace/microHAL_examples/microHAL/ports/STMicroelectronics/STM32F4xx/startup\"" }    	  	
    	microhalos { "nortos" } 
    	includedirs { "boards/stm32f4-discovery" }
    	files { "boards/stm32f4-discovery/*.cpp",
    			"boards/stm32f4-discovery/microhal_bsp.h" }
    	
	filter "configurations:Debug"
    	defines { "DEBUG" }
      	flags { "Symbols" }

   	filter "configurations:Release"
      	defines { "NDEBUG" }
      	optimize "On"
      

