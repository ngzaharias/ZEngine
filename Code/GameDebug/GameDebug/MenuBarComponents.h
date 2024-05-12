#pragma once

#include <ECS/Component.h>

namespace dbg
{
	struct BufferWindowRequestComponent : ecs::EventComponent<BufferWindowRequestComponent> { };
	struct ClientWindowRequestComponent : ecs::EventComponent<ClientWindowRequestComponent> { };
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct ImGuiDemoRequestComponent : ecs::EventComponent<ImGuiDemoRequestComponent> { };
	struct LevelDialogRequestComponent : ecs::EventComponent<LevelDialogRequestComponent> { };
	struct NetworkWindowRequestComponent : ecs::EventComponent<NetworkWindowRequestComponent> { };
	struct NodeGraphRequestComponent : ecs::EventComponent<NodeGraphRequestComponent> { };
	struct SaveLevelRequestComponent : ecs::EventComponent<SaveLevelRequestComponent> { };
	struct SaveLevelAsRequestComponent : ecs::EventComponent<SaveLevelAsRequestComponent> { };
	struct ServerWindowRequestComponent : ecs::EventComponent<ServerWindowRequestComponent> { };
}

namespace editor
{
	struct FlipbookWindowRequestComponent : ecs::EventComponent<FlipbookWindowRequestComponent> { };
	struct TableWindowRequestComponent : ecs::EventComponent<TableWindowRequestComponent> { };
	struct TextureWindowRequestComponent : ecs::EventComponent<TextureWindowRequestComponent> { };
	struct SpriteWindowRequestComponent : ecs::EventComponent<SpriteWindowRequestComponent> { };
	struct TrajectoryWindowRequestComponent : ecs::EventComponent<TrajectoryWindowRequestComponent> { };
	struct TransformWindowRequestComponent : ecs::EventComponent<TransformWindowRequestComponent> { };
}