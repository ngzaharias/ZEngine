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
	group "Debuggers"
		include "Code/Debuggers/DebugEntity/premake5.lua"
	group "Editors"
		include "Code/Editors/AssetBrowser/premake5.lua"
		include "Code/Editors/EntityEditor/premake5.lua"
		include "Code/Editors/FlipbookEditor/premake5.lua"
		include "Code/Editors/InputEditor/premake5.lua"
		include "Code/Editors/NodeGraph/premake5.lua"
		include "Code/Editors/SpriteEditor/premake5.lua"
		include "Code/Editors/TableEditor/premake5.lua"
		include "Code/Editors/TextureEditor/premake5.lua"
		include "Code/Editors/ThemeEditor/premake5.lua"
		include "Code/Editors/TrajectoryEditor/premake5.lua"
	group "Framework"
		include "Code/Framework/Core/premake5.lua"
		include "Code/Framework/ECS/premake5.lua"
		include "Code/Framework/Engine/premake5.lua"
		include "Code/Framework/Icons/premake5.lua"
		include "Code/Framework/Imgui/premake5.lua"
		include "Code/Framework/Input/premake5.lua"
		include "Code/Framework/Math/premake5.lua"
		include "Code/Framework/Network/premake5.lua"
		include "Code/Framework/Render/premake5.lua"
		include "Code/Framework/Serialize/premake5.lua"
	group "Modules/Camera"
		include "Code/Modules/Camera/SharedCamera/premake5.lua"
	group "Modules/Crafting"
		include "Code/Modules/Crafting/DebugCrafting/premake5.lua"
		include "Code/Modules/Crafting/SharedCrafting/premake5.lua"
		include "Code/Modules/Crafting/SharedCrafting_ut/premake5.lua"
	group "Modules/Cursor"
		include "Code/Modules/Cursor/ClientCursor/premake5.lua"
		include "Code/Modules/Cursor/ServerCursor/premake5.lua"
		include "Code/Modules/Cursor/SharedCursor/premake5.lua"
	group "Modules/GameState"
		include "Code/Modules/GameState/SharedGameState/premake5.lua"
	group "Modules/Hexmap"
		include "Code/Modules/Hexmap/SharedHexmap/premake5.lua"
	group "Modules/Hidden"
		include "Code/Modules/Hidden/ClientHidden/premake5.lua"
		include "Code/Modules/Hidden/ServerHidden/premake5.lua"
		include "Code/Modules/Hidden/SharedHidden/premake5.lua"
	group "Modules/Inventory"
		include "Code/Modules/Inventory/DebugInventory/premake5.lua"
		include "Code/Modules/Inventory/SharedInventory/premake5.lua"
		include "Code/Modules/Inventory/SharedInventory_ut/premake5.lua"
	group "Modules/Network"
		include "Code/Modules/Network/ClientNetwork/premake5.lua"
		include "Code/Modules/Network/ServerNetwork/premake5.lua"
		include "Code/Modules/Network/SharedNetwork/premake5.lua"
	group "Modules/Replication"
		include "Code/Modules/Replication/ServerReplication/premake5.lua"
	group "Modules/Softbody"
		include "Code/Modules/Softbody/SharedSoftbody/premake5.lua"
	group "Modules/Spellcraft"
		include "Code/Modules/Spellcraft/EditorSpellcraft/premake5.lua"
		include "Code/Modules/Spellcraft/SharedSpellcraft/premake5.lua"
	group "Modules/Tabletop"
		include "Code/Modules/Tabletop/SharedTabletop/premake5.lua"
	group "Modules/Tactics"
		include "Code/Modules/Tactics/SharedTactics/premake5.lua"
	group "Modules/Tilemap"
		include "Code/Modules/Tilemap/DebugTilemap/premake5.lua"
		include "Code/Modules/Tilemap/ServerTilemap/premake5.lua"
		include "Code/Modules/Tilemap/SharedTilemap/premake5.lua"
	group "Modules/Voxel"
		include "Code/Modules/Voxel/ClientVoxel/premake5.lua"
		include "Code/Modules/Voxel/ServerVoxel/premake5.lua"
		include "Code/Modules/Voxel/SharedVoxel/premake5.lua"
	group "UnitTests"
		include "Code/Application/GameClient_ut/premake5.lua"
		include "Code/Editors/NodeGraph_ut/premake5.lua"
		include "Code/Framework/Core_ut/premake5.lua"
		include "Code/Framework/ECS_ut/premake5.lua"
		include "Code/Framework/Engine_ut/premake5.lua"
		include "Code/Framework/Lua_ut/premake5.lua"
		include "Code/Framework/Math_ut/premake5.lua"
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
