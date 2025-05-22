#pragma once

#include "ECS/Component.h"

namespace dbg
{
	struct BufferWindowRequestComponent : ecs::EventComponent<BufferWindowRequestComponent> { };
	struct ClientWindowRequestComponent : ecs::EventComponent<ClientWindowRequestComponent> { };
	struct ContainerWindowRequestComponent : ecs::EventComponent<ContainerWindowRequestComponent> { };
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct ImGuiDemoRequestComponent : ecs::EventComponent<ImGuiDemoRequestComponent> { };
	struct ImNodesDemoRequestComponent : ecs::EventComponent<ImNodesDemoRequestComponent> { };
	struct InspectorDemoRequestComponent : ecs::EventComponent<InspectorDemoRequestComponent> { };
	struct NetworkWindowRequestComponent : ecs::EventComponent<NetworkWindowRequestComponent> { };
	struct ServerWindowRequestComponent : ecs::EventComponent<ServerWindowRequestComponent> { };
	struct ShapeWindowRequestComponent : ecs::EventComponent<ShapeWindowRequestComponent> { };
	struct SplineWindowRequestComponent : ecs::EventComponent<SplineWindowRequestComponent> { };
}

namespace dbg::level
{
	struct ReloadRequestComponent : ecs::EventComponent<ReloadRequestComponent> { };
	struct SaveRequestComponent : ecs::EventComponent<SaveRequestComponent> { };
	struct SaveAsRequestComponent : ecs::EventComponent<SaveAsRequestComponent> { };
}

namespace editor
{
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct FlipbookWindowRequestComponent : ecs::EventComponent<FlipbookWindowRequestComponent> { };
	struct InputWindowRequestComponent : ecs::EventComponent<InputWindowRequestComponent> { };
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