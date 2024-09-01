#pragma once

#include "ECS/Component.h"

namespace dbg
{
	struct BufferWindowRequestComponent : ecs::EventComponent<BufferWindowRequestComponent> { };
	struct ClientWindowRequestComponent : ecs::EventComponent<ClientWindowRequestComponent> { };
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct ImGuiDemoRequestComponent : ecs::EventComponent<ImGuiDemoRequestComponent> { };
	struct NetworkWindowRequestComponent : ecs::EventComponent<NetworkWindowRequestComponent> { };
	struct NodeGraphRequestComponent : ecs::EventComponent<NodeGraphRequestComponent> { };
	struct ServerWindowRequestComponent : ecs::EventComponent<ServerWindowRequestComponent> { };
	struct ShapeWindowRequestComponent : ecs::EventComponent<ShapeWindowRequestComponent> { };
}

namespace dbg::level
{
	struct OpenRequestComponent : ecs::EventComponent<OpenRequestComponent> { };
	struct ReloadRequestComponent : ecs::EventComponent<ReloadRequestComponent> { };
	struct SaveRequestComponent : ecs::EventComponent<SaveRequestComponent> { };
	struct SaveAsRequestComponent : ecs::EventComponent<SaveAsRequestComponent> { };
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