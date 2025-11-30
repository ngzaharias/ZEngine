#pragma once

#include "ECS/System.h"

namespace dbg
{
	struct BufferWindowRequest;
	struct ContainerWindowRequest;
	struct EntityWindowRequest;
	struct FlipbookWindowRequest;
	struct ImGuiDemoRequest;
	struct ImNodesDemoRequest;
	struct InspectorDemoRequest;
	struct NetworkWindowRequest;
	struct ShapeWindowRequest;
	struct SplineWindowRequest;
}

namespace dbg::level
{
	struct OpenRequest;
	struct ReloadRequest;
	struct SaveRequest;
	struct SaveAsRequest;
}

namespace dbg::settings
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

namespace dbg
{
	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			eng::AssetManager,
			eng::TableHeadmaster,
			const eng::InputManager,
			// Events
			dbg::BufferWindowRequest,
			dbg::ContainerWindowRequest,
			dbg::EntityWindowRequest,
			dbg::FlipbookWindowRequest,
			dbg::ImGuiDemoRequest,
			dbg::ImNodesDemoRequest,
			dbg::InspectorDemoRequest,
			dbg::level::OpenRequest,
			dbg::level::ReloadRequest,
			dbg::level::SaveAsRequest,
			dbg::level::SaveRequest,
			dbg::NetworkWindowRequest,
			dbg::settings::WindowRequest,
			dbg::ShapeWindowRequest,
			dbg::SplineWindowRequest,
			editor::AssetBrowserWindowRequest,
			editor::EntityWindowRequest,
			editor::FlipbookWindowRequest,
			editor::InputWindowRequest,
			editor::settings::WindowRequest,
			editor::SpriteWindowRequest,
			editor::TableWindowRequest,
			editor::TextureWindowRequest,
			editor::TrajectoryWindowRequest,
			eng::TablesReloaded>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}