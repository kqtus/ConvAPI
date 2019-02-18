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

function useServicesLib()
	links "services"
end
	
project "utilities"
	kind "StaticLib"
	files "src/utilities/**"
	flags { "NoPCH" }

project "formats"
	kind "StaticLib"
	includedirs { "3rd/include", "src/utilities/**" }
	files "src/formats/**"
	flags { "NoPCH" }
	
	useUtilitiesLib()
	links "3rd/lib/x64/debug/libfbxsdk"
	
project "converters"
	kind "StaticLib"
	includedirs { "3rd/include", "src/formats/**" }
	files "src/converters/**"
	flags { "NoPCH" }
	
	defines "FBXSDK_SHARED"
		
	useFormatsLib()
	links "3rd/lib/x64/debug/libfbxsdk"

project "services"
	kind "StaticLib"
	includedirs { "3rd/include", "src/formats/**", "src/converters/**", "src/utilities/**" }
	files "src/services/**"
	flags { "NoPCH" }
	
	useUtilitiesLib()
	useFormatsLib()
	useConvertersLib()
	
project "ConvAPI"
	kind "WindowedApp"
	files "src/conv_api/**"
	
	-- Hardcoded DXSDK_DIRs, to be changed.
	includedirs { "3rd/include", "src", "src/conv_api/**", "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include" }
	libdirs("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
		
	flags { "NoPCH" }
	
	useUtilitiesLib()
	useFormatsLib()
	useConvertersLib()
	useServicesLib()
	
	links { "3rd/lib/x64/debug/libfbxsdk" }