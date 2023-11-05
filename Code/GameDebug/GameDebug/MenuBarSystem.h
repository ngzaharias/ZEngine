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
	struct SpellWindowRequestComponent;
	struct TrajectoryWindowRequestComponent;
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
			editor::FlipbookWindowRequestComponent,
			editor::SpellWindowRequestComponent,
			editor::TrajectoryWindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}