workspace "conv_api"
	language "C++"
	architecture "x86_64"
	configurations { "Debug", "Release" }
	
	filter { "configurations:Debug" }
		flags { "Symbols" }
	
	filter { "configurations:Release" }
		optimize "On"
	
	filter { }
	
	targetdir ("build/bin/%{prj.name}/%{cfg.longname}")
	objdir ("build/obj/%{prj.name}/%{cfg.longname}")
	
	startproject "ConvAPI"
	
function useUtilitiesLib()
	links "utilities"
end

function useFormatsLib()
	links "formats"
end

function useConvertersLib()
	links "converters"
end
	
project "utilities"
	kind "StaticLib"
	files "src/utilities/**"
	flags { "NoPCH" }

project "formats"
	kind "StaticLib"
	includedirs { "src/utilities/**" }
	files "src/formats/**"
	flags { "NoPCH" }
	useUtilitiesLib()
	
project "converters"
	kind "StaticLib"
	includedirs { "src/formats/**" }
	files "src/converters/**"
	flags { "NoPCH" }
	useFormatsLib()

project "ConvAPI"
	kind "ConsoleApp"
	files "src/conv_api/**"

	flags { "NoPCH" }
	
	useUtilitiesLib()
	useFormatsLib()
	useConvertersLib()
	
	includedirs { "src", "src/conv_api/**" }
	
