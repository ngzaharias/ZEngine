package.path = package.path .. ";./Code/_Premake/?.lua"
require "SmartArguments"

workspace "ZEngine"
	location "Solution"
	architecture "x86_64"
	configurations { "Debug", "Debug_Editor", "Release", "Release_Editor" }
	dependson { "ZERO_CHECK" }
	language "C++"
	cppdialect "C++20"
	buildoptions { "/Zc:__cplusplus" }
	platforms { "x64" }
	startproject "Editor"
	inheritdependencies "On"

	targetdir "%{wks.location}/_Build/%{prj.name}/%{cfg.buildcfg}_%{cfg.platform}/"
	objdir "%{wks.location}/_Intermediate/%{prj.name}/%{cfg.buildcfg}_%{cfg.platform}/"
	debugdir "%{wks.location}/_Build/%{prj.name}/%{cfg.buildcfg}_%{cfg.platform}/"

	defines { "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING", "_SILENCE_CXX20_CISO646_REMOVED_WARNING" }
	disablewarnings { "26812" }

	filter "Debug*"
		defines { "_DEBUG", "Z_DEBUG", "USE_OPTICK", "ASSERTS_ENABLED", "LOGGING_ENABLED" }
		flags { "FatalCompileWarnings" }
		optimize "Off"
		runtime "Debug"
		symbols "On"
	filter "Release*"
		defines { "NDEBUG", "Z_RELEASE" }
		editandcontinue "Off"
		flags { "FatalWarnings" }
		optimize "Speed"
		runtime "Release"
		symbols "On"
	filter "*_Editor"
		defines { "Z_EDITOR" }
	filter {} -- disable the filter

	-- https://premake.github.io/docs/flags/
	flags 
	{ 
		-- "MultiProcessorCompile",
		-- "NoIncrementalLink", 
		-- https://stackoverflow.com/questions/3007312/resolving-lnk4098-defaultlib-msvcrt-conflicts-with
		-- "OmitDefaultLibrary",
	}

	files 
	{ 
		"%{wks.location}/../Code/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/%{prj.name}/Resource/**.*",
		"%{wks.location}/../Code/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/../Code/Application/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/Application/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/Application/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/Application/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/Application/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/../Code/Editors/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/Editors/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/Editors/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/Editors/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/Editors/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/../Code/Bindings/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/Bindings/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/Bindings/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/Bindings/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/Bindings/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/../Code/Framework/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/Framework/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/Framework/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/Framework/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/Framework/%{prj.name}/%{prj.name}/**.inl",
		"%{wks.location}/../Code/Modules/%{prj.name}/*.natvis",
		"%{wks.location}/../Code/Modules/%{prj.name}/premake5.*",
		"%{wks.location}/../Code/Modules/%{prj.name}/%{prj.name}/**.h",
		"%{wks.location}/../Code/Modules/%{prj.name}/%{prj.name}/**.cpp",
		"%{wks.location}/../Code/Modules/%{prj.name}/%{prj.name}/**.inl",
	}

	vpaths 
	{ 
		{ ["./"] = { "**premake5.lua", "**.natvis" } },
	}

	-- projects
	group "Application"
		include "Code/Application/Editor/premake5.lua"
		include "Code/Application/Game/premake5.lua"
		include "Code/Application/GameClient/premake5.lua"
		include "Code/Application/GameDebug/premake5.lua"
		include "Code/Application/GameServer/premake5.lua"
		include "Code/Application/GameUI/premake5.lua"
	group "Bindings"
		include "Code/Bindings/Steam/premake5.lua"
	group "Editors"
		include "Code/Editors/InputEditor/premake5.lua"
		include "Code/Editors/FlipbookEditor/premake5.lua"
		include "Code/Editors/SpriteEditor/premake5.lua"
		include "Code/Editors/TableEditor/premake5.lua"
		include "Code/Editors/TextureEditor/premake5.lua"
		include "Code/Editors/ThemeEditor/premake5.lua"
		include "Code/Editors/TrajectoryEditor/premake5.lua"
	group "Framework"
		include "Code/Framework/Core/premake5.lua"
		include "Code/Framework/ECS/premake5.lua"
		include "Code/Framework/Engine/premake5.lua"
		include "Code/Framework/Imgui/premake5.lua"
		include "Code/Framework/Input/premake5.lua"
		include "Code/Framework/Math/premake5.lua"
		include "Code/Framework/Network/premake5.lua"
		include "Code/Framework/Render/premake5.lua"
	group "Modules"
		include "Code/Modules/Camera/premake5.lua"
		include "Code/Modules/ClientCursor/premake5.lua"
		include "Code/Modules/ClientHidden/premake5.lua"
		include "Code/Modules/ClientNetwork/premake5.lua"
		include "Code/Modules/Container/premake5.lua"
		include "Code/Modules/Hexmap/premake5.lua"
		include "Code/Modules/ServerCursor/premake5.lua"
		include "Code/Modules/ServerHidden/premake5.lua"
		include "Code/Modules/ServerNetwork/premake5.lua"
		include "Code/Modules/SharedCursor/premake5.lua"
		include "Code/Modules/SharedHidden/premake5.lua"
		include "Code/Modules/SharedNetwork/premake5.lua"
		include "Code/Modules/Softbody/premake5.lua"
		include "Code/Modules/Tabletop/premake5.lua"
		include "Code/Modules/Tactics/premake5.lua"
		include "Code/Modules/Tilemap/premake5.lua"
		include "Code/Modules/Voxel/premake5.lua"
	group "UnitTests"
		include "Code/Application/GameClient_ut/premake5.lua"
		include "Code/Framework/Core_ut/premake5.lua"
		include "Code/Framework/ECS_ut/premake5.lua"
		include "Code/Framework/Engine_ut/premake5.lua"
		include "Code/Framework/Lua_ut/premake5.lua"
		include "Code/Framework/Math_ut/premake5.lua"
		include "Code/Modules/Container_ut/premake5.lua"
	group ""

project "ZERO_CHECK"
	kind "ConsoleApp"
	language "C++"
	location "%{wks.location}/ZERO_CHECK"
	files { "%{wks.location}/../premake5.lua" }

	-- we must run change to the batch file directory to run it
	buildcommands { "cd %{wks.location}..\\ & call generate.bat" }
	-- by setting a build output that doesn't exist it will always run
	buildoutputs { "unused.txt" }
