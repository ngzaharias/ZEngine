#pragma once

#include "ECS/System.h"

namespace dbg
{
	struct BufferWindowRequestComponent;
	struct ContainerWindowRequestComponent;
	struct EntityWindowRequestComponent;
	struct FlipbookWindowRequestComponent;
	struct ImGuiDemoRequestComponent;
	struct ImNodesDemoRequestComponent;
	struct InspectorDemoRequestComponent;
	struct NetworkWindowRequestComponent;
	struct ShapeWindowRequestComponent;
	struct SplineWindowRequestComponent;
}

namespace dbg::level
{
	struct OpenRequestComponent;
	struct ReloadRequestComponent;
	struct SaveRequestComponent;
	struct SaveAsRequestComponent;
}

namespace dbg::settings
{
	struct WindowRequestComponent;
}

namespace editor
{
	struct AssetBrowserWindowRequestComponent;
	struct EntityWindowRequestComponent;
	struct FlipbookWindowRequestComponent;
	struct InputWindowRequestComponent;
	struct SettingsWindowRequestComponent;
	struct SpriteWindowRequestComponent;
	struct TableWindowRequestComponent;
	struct TextureWindowRequestComponent;
	struct TrajectoryWindowRequestComponent;
}

namespace editor::settings
{
	struct WindowRequestComponent;
}

namespace eng
{
	class AssetManager;
	class InputManager;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace dbg
{
	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView <
			// Resources
			const eng::InputManager,
			// Components
			dbg::BufferWindowRequestComponent,
			dbg::ContainerWindowRequestComponent,
			dbg::EntityWindowRequestComponent,
			dbg::FlipbookWindowRequestComponent,
			dbg::ImGuiDemoRequestComponent,
			dbg::ImNodesDemoRequestComponent,
			dbg::InspectorDemoRequestComponent,
			dbg::level::OpenRequestComponent,
			dbg::level::ReloadRequestComponent,
			dbg::level::SaveRequestComponent,
			dbg::level::SaveAsRequestComponent,
			dbg::NetworkWindowRequestComponent,
			dbg::settings::WindowRequestComponent,
			dbg::ShapeWindowRequestComponent,
			dbg::SplineWindowRequestComponent,
			editor::AssetBrowserWindowRequestComponent,
			editor::EntityWindowRequestComponent,
			editor::FlipbookWindowRequestComponent,
			editor::InputWindowRequestComponent,
			editor::settings::WindowRequestComponent,
			editor::SpriteWindowRequestComponent,
			editor::TableWindowRequestComponent,
			editor::TextureWindowRequestComponent,
			editor::TrajectoryWindowRequestComponent,
			eng::AssetManager,
			eng::settings::LocalComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);
	};
}