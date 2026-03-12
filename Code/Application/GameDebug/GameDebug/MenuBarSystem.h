#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct FrameBufferWindowEvent;
	struct ImGuiDemoEvent;
	struct ImNodesDemoEvent;
	struct InspectorDemoEvent;
	struct NetworkWindowEvent;
	struct ShapeWindowEvent;
	struct SplineWindowEvent;
}

namespace debug::entity
{
	struct WindowEvent;
}

namespace debug::inventory
{
	struct WindowEvent;
}

namespace debug::level
{
	struct OpenEvent;
	struct ReloadEvent;
	struct SaveEvent;
	struct SaveAsEvent;
}

namespace debug::settings
{
	struct WindowEvent;
}

namespace editor
{
	struct AssetBrowserWindowEvent;
	struct SettingsWindowEvent;
}

namespace editor::input
{
	struct WindowEvent;
}

namespace editor::settings
{
	struct WindowEvent;
}

namespace editor::table
{
	struct WindowEvent;
}

namespace editor::theme
{
	struct WindowEvent;
}

namespace editor::trajectory
{
	struct WindowEvent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class TableHeadmaster;
	struct TablesReloadedEvent;
}

namespace debug
{
	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::entity::WindowEvent,
			debug::FrameBufferWindowEvent,
			debug::ImGuiDemoEvent,
			debug::ImNodesDemoEvent,
			debug::InspectorDemoEvent,
			debug::inventory::WindowEvent,
			debug::level::OpenEvent,
			debug::level::ReloadEvent,
			debug::level::SaveAsEvent,
			debug::level::SaveEvent,
			debug::NetworkWindowEvent,
			debug::settings::WindowEvent,
			debug::ShapeWindowEvent,
			debug::SplineWindowEvent,
			editor::AssetBrowserWindowEvent,
			editor::input::WindowEvent,
			editor::settings::WindowEvent,
			editor::table::WindowEvent,
			editor::theme::WindowEvent,
			editor::trajectory::WindowEvent,
			eng::AssetManager,
			eng::InputManager,
			eng::TableHeadmaster,
			eng::TablesReloadedEvent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}