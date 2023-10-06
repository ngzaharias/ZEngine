#pragma once

#include <ECS/Component.h>
#include <ECS/System.h>

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace editor
{
	struct FlipbookWindowRequestComponent;
}

namespace dbg
{
	struct BufferWindowRequestComponent;
	struct EntityWindowRequestComponent;
	struct FlipbookWindowRequestComponent;
	struct ImGuiDemoRequestComponent;
	struct LevelDialogRequestComponent;
	struct NetworkWindowRequestComponent;
	struct NodeGraphRequestComponent;
	struct SaveLevelRequestComponent;
	struct SaveLevelAsRequestComponent;
	struct TrajectoryWindowRequestComponent;

	class MenuBarSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			dbg::BufferWindowRequestComponent,
			dbg::EntityWindowRequestComponent,
			dbg::FlipbookWindowRequestComponent,
			dbg::ImGuiDemoRequestComponent,
			dbg::LevelDialogRequestComponent,
			dbg::NetworkWindowRequestComponent,
			dbg::NodeGraphRequestComponent,
			dbg::SaveLevelRequestComponent,
			dbg::SaveLevelAsRequestComponent,
			dbg::TrajectoryWindowRequestComponent,
			editor::FlipbookWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}