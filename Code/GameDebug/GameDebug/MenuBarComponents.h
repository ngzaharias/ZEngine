#pragma once

#include <ECS/Component.h>

namespace dbg
{
	struct BufferWindowRequestComponent : ecs::EventComponent<BufferWindowRequestComponent> { };
	struct ClientWindowRequestComponent : ecs::EventComponent<ClientWindowRequestComponent> { };
	struct ContainerWindowRequestComponent : ecs::EventComponent<ContainerWindowRequestComponent> { };
	struct EntityWindowRequestComponent : ecs::EventComponent<EntityWindowRequestComponent> { };
	struct ImGuiDemoRequestComponent : ecs::EventComponent<ImGuiDemoRequestComponent> { };
	struct LevelDialogRequestComponent : ecs::EventComponent<LevelDialogRequestComponent> { };
	struct NetworkWindowRequestComponent : ecs::EventComponent<NetworkWindowRequestComponent> { };
	struct NodeGraphRequestComponent : ecs::EventComponent<NodeGraphRequestComponent> { };
	struct SaveLevelRequestComponent : ecs::EventComponent<SaveLevelRequestComponent> { };
	struct SaveLevelAsRequestComponent : ecs::EventComponent<SaveLevelAsRequestComponent> { };
	struct ServerWindowRequestComponent : ecs::EventComponent<ServerWindowRequestComponent> { };
}