#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace debug
{
	struct ContainerWindowRequest;
	struct EntityWindowRequest;
	struct FlipbookWindowRequest;
	struct FrameBufferWindowRequest;
	struct ImGuiDemoRequest;
	struct ImNodesDemoRequest;
	struct InspectorDemoRequest;
	struct NetworkWindowRequest;
	struct ShapeWindowRequest;
	struct SplineWindowRequest;
}

namespace debug::level
{
	struct OpenRequest;
	struct ReloadRequest;
	struct SaveRequest;
	struct SaveAsRequest;
}

namespace debug::settings
{
	struct WindowRequest;
}

namespace editor
{
	struct AssetBrowserWindowRequest;
	struct EntityWindowRequest;
	struct FlipbookWindowRequest;
	struct InputWindowRequest;
	struct SettingsWindowRequest;
	struct SpriteWindowRequest;
	struct TableWindowRequest;
	struct TextureWindowRequest;
	struct TrajectoryWindowRequest;
}

namespace editor::settings
{
	struct WindowRequest;
}

namespace eng
{
	class AssetManager;
	class InputManager;
	class TableHeadmaster;
	struct TablesReloaded;
}

namespace debug
{
	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			debug::ContainerWindowRequest,
			debug::EntityWindowRequest,
			debug::FlipbookWindowRequest,
			debug::FrameBufferWindowRequest,
			debug::ImGuiDemoRequest,
			debug::ImNodesDemoRequest,
			debug::InspectorDemoRequest,
			debug::level::OpenRequest,
			debug::level::ReloadRequest,
			debug::level::SaveAsRequest,
			debug::level::SaveRequest,
			debug::NetworkWindowRequest,
			debug::settings::WindowRequest,
			debug::ShapeWindowRequest,
			debug::SplineWindowRequest,
			editor::AssetBrowserWindowRequest,
			editor::EntityWindowRequest,
			editor::FlipbookWindowRequest,
			editor::InputWindowRequest,
			editor::settings::WindowRequest,
			editor::SpriteWindowRequest,
			editor::TableWindowRequest,
			editor::TextureWindowRequest,
			editor::TrajectoryWindowRequest,
			eng::AssetManager,
			eng::TableHeadmaster,
			eng::TablesReloaded>
			::Read<
			eng::InputManager>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}