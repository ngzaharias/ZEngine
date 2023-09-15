#pragma once

#include <ECS/Component.h>
#include <ECS/System.h>

class GameTime;

namespace ecs
{
	class EntityWorld;
}

namespace dbg
{
	struct BufferWindowRequestComponent;
	struct EntityWindowRequestComponent;
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
			dbg::ImGuiDemoRequestComponent,
			dbg::LevelDialogRequestComponent,
			dbg::NetworkWindowRequestComponent,
			dbg::NodeGraphRequestComponent,
			dbg::SaveLevelRequestComponent,
			dbg::SaveLevelAsRequestComponent,
			dbg::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}