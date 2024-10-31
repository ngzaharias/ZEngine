#pragma once

#include "ECS/Component.h"

namespace dbg
{
	struct BufferWindowRequestComponent : ecs::EventComponent<BufferWindowRequestComponent> { };
	struct ClientWindowRequestComponent : ecs::EventComponent<ClientWindowRequestComponent> { };
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct ImGuiDemoRequestComponent : ecs::EventComponent<ImGuiDemoRequestComponent> { };
	struct ImNodesDemoRequestComponent : ecs::EventComponent<ImNodesDemoRequestComponent> { };
	struct InspectorDemoRequestComponent : ecs::EventComponent<InspectorDemoRequestComponent> { };
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
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct FlipbookWindowRequestComponent : ecs::EventComponent<FlipbookWindowRequestComponent> { };
	struct SettingsWindowRequestComponent : ecs::EventComponent<SettingsWindowRequestComponent> { };
	struct SpriteWindowRequestComponent : ecs::EventComponent<SpriteWindowRequestComponent> { };
	struct TableWindowRequestComponent : ecs::EventComponent<TableWindowRequestComponent> { };
	struct TextureWindowRequestComponent : ecs::EventComponent<TextureWindowRequestComponent> { };
	struct TrajectoryWindowRequestComponent : ecs::EventComponent<TrajectoryWindowRequestComponent> { };
}

namespace editor::settings
{
	struct WindowRequestComponent : ecs::EventComponent<WindowRequestComponent> { };
}