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
}

namespace dbg::level
{
	struct OpenRequestComponent;
	struct ReloadRequestComponent;
	struct SaveRequestComponent;
	struct SaveAsRequestComponent;
}

namespace editor
{
	struct EntityWindowRequestComponent;
	struct FlipbookWindowRequestComponent;
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
	struct InputComponent;
}

namespace eng::settings
{
	struct LocalComponent;
}

namespace gui::settings
{
	struct OpenRequestComponent;
}

namespace dbg
{
	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
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
			dbg::ShapeWindowRequestComponent,
			editor::EntityWindowRequestComponent,
			editor::FlipbookWindowRequestComponent,
			editor::settings::WindowRequestComponent,
			editor::SpriteWindowRequestComponent,
			editor::TableWindowRequestComponent,
			editor::TextureWindowRequestComponent,
			editor::TrajectoryWindowRequestComponent,
			eng::AssetManager,
			eng::settings::LocalComponent,
			gui::settings::OpenRequestComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}