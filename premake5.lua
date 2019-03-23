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
	
function useRendererLib()
	links "renderer"
end 

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
	
function useConvApiLib()
	links "ConvAPI"
end 

function useQtLibs()
	links { "Qt5Widgetsd", "Qt5Guid", "Qt5Cored" }
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
	
project "Renderer"
	kind "StaticLib"
	files "src/renderer/**"
	
	includedirs { "src/renderer/**" }
		
	flags { "NoPCH" }
	
	useUtilitiesLib()
	useFormatsLib()
	useConvertersLib()
	useServicesLib()
	
	links { "3rd/lib/x64/debug/Effects11d", "d3d11", "D3DCompiler" }
	
project "ConvAPI"
	kind "WindowedApp"
	files "src/conv_api/**"
	
	includedirs { "3rd/include", "src", "src/conv_api/**" }
		
	flags { "NoPCH" }
	
	useUtilitiesLib()
	useServicesLib()
	useConvertersLib()
	useRendererLib()
	
	links { "3rd/lib/x64/debug/libfbxsdk" }

project "LED"
	kind "ConsoleApp"
	files "src/LED/**"
	
	includedirs { 
		"3rd/include/**", 
		os.getenv("QTDIR") .. "/include/",
		os.getenv("QTDIR") .. "/include/QtGui",
		os.getenv("QTDIR") .. "/include/QtCore",
		os.getenv("QTDIR") .. "/include/QtWidgets"
	}
	

	libdirs {
		os.getenv("QTDIR") .. "/lib/"
	}
	
	flags { "NoPCH" }
	
	
	useServicesLib()
	useRendererLib()

	-- Qt stuff
	
	useQtLibs();
	defines { "QT_CORE_LIB", "QT_GUI_LIB", "QT_WIDGETS_LIB" }
	
	
	filter 'files:**.h'
		buildmessage 'Creating moc file for %{file.relpath}'
		
		
		buildcommands {
			'"$(QTDIR)/bin/moc.exe" %{file.relpath} -o "%{file.directory}/moc/moc_%{file.basename}.cpp"  -DUNICODE -DWIN32 -DWIN64 -DQT_DLL -DQT_NO_DEBUG -DNDEBUG -DQT_CORE_LIB -DQT_GUI_LIB -DQT_SQL_LIB -DQT_OPENGL_LIB -DQT_PRINTSUPPORT_LIB -DQT_SVG_LIB -DQT_WIDGETS_LIB -DQT_XML_LIB'
		}
		
		buildoutputs {
			'%{file.directory}/moc/moc_%{file.basename}.cpp'
		}