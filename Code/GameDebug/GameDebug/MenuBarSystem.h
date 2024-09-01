#pragma once

#include "ECS/System.h"

namespace dbg
{
	struct BufferWindowRequestComponent;
	struct EntityWindowRequestComponent;
	struct FlipbookWindowRequestComponent;
	struct ImGuiDemoRequestComponent;
	struct NetworkWindowRequestComponent;
	struct NodeGraphRequestComponent;
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
	struct FlipbookWindowRequestComponent;
	struct SpriteWindowRequestComponent;
	struct TableWindowRequestComponent;
	struct TextureWindowRequestComponent;
	struct TrajectoryWindowRequestComponent;
	struct TransformWindowRequestComponent;
}

namespace eng
{
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
			dbg::EntityWindowRequestComponent,
			dbg::FlipbookWindowRequestComponent,
			dbg::ImGuiDemoRequestComponent,
			dbg::level::OpenRequestComponent,
			dbg::level::ReloadRequestComponent,
			dbg::level::SaveRequestComponent,
			dbg::level::SaveAsRequestComponent,
			dbg::NetworkWindowRequestComponent,
			dbg::NodeGraphRequestComponent,
			dbg::ShapeWindowRequestComponent,
			editor::FlipbookWindowRequestComponent,
			editor::SpriteWindowRequestComponent,
			editor::TableWindowRequestComponent,
			editor::TextureWindowRequestComponent,
			editor::TrajectoryWindowRequestComponent,
			editor::TransformWindowRequestComponent,
			eng::settings::LocalComponent,
			gui::settings::OpenRequestComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}