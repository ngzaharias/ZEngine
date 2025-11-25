#pragma once

#include "ECS/Component.h"

namespace dbg
{
	struct BufferWindowRequest : ecs::Event<BufferWindowRequest> { };
	struct ClientWindowRequest : ecs::Event<ClientWindowRequest> { };
	struct ContainerWindowRequest : ecs::Event<ContainerWindowRequest> { };
	struct EntityWindowRequest : ecs::Event<EntityWindowRequest> { };
	struct ImGuiDemoRequest : ecs::Event<ImGuiDemoRequest> { };
	struct ImNodesDemoRequest : ecs::Event<ImNodesDemoRequest> { };
	struct InspectorDemoRequest : ecs::Event<InspectorDemoRequest> { };
	struct NetworkWindowRequest : ecs::Event<NetworkWindowRequest> { };
	struct ServerWindowRequest : ecs::Event<ServerWindowRequest> { };
	struct ShapeWindowRequest : ecs::Event<ShapeWindowRequest> { };
	struct SplineWindowRequest : ecs::Event<SplineWindowRequest> { };
}

namespace dbg::level
{
	struct ReloadRequest : ecs::Event<ReloadRequest> { };
	struct SaveRequest : ecs::Event<SaveRequest> { };
	struct SaveAsRequest : ecs::Event<SaveAsRequest> { };
}

namespace editor
{
	struct AssetBrowserWindowRequest : ecs::Event<AssetBrowserWindowRequest> { };
	struct EntityWindowRequest : ecs::Event<EntityWindowRequest> { };
	struct FlipbookWindowRequest : ecs::Event<FlipbookWindowRequest> { };
	struct InputWindowRequest : ecs::Event<InputWindowRequest> { };
	struct SettingsWindowRequest : ecs::Event<SettingsWindowRequest> { };
	struct SpriteWindowRequest : ecs::Event<SpriteWindowRequest> { };
	struct TableWindowRequest : ecs::Event<TableWindowRequest> { };
	struct TextureWindowRequest : ecs::Event<TextureWindowRequest> { };
	struct TrajectoryWindowRequest : ecs::Event<TrajectoryWindowRequest> { };
}

namespace editor::settings
{
	struct WindowRequest : ecs::Event<WindowRequest> { };
}