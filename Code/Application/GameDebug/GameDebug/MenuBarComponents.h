#pragma once

#include "ECS/Component.h"

namespace dbg
{
	struct BufferWindowRequest final : public ecs::Event<BufferWindowRequest> { };
	struct ClientWindowRequest final : public ecs::Event<ClientWindowRequest> { };
	struct ContainerWindowRequest final : public ecs::Event<ContainerWindowRequest> { };
	struct EntityWindowRequest final : public ecs::Event<EntityWindowRequest> { };
	struct ImGuiDemoRequest final : public ecs::Event<ImGuiDemoRequest> { };
	struct ImNodesDemoRequest final : public ecs::Event<ImNodesDemoRequest> { };
	struct InspectorDemoRequest final : public ecs::Event<InspectorDemoRequest> { };
	struct NetworkWindowRequest final : public ecs::Event<NetworkWindowRequest> { };
	struct ServerWindowRequest final : public ecs::Event<ServerWindowRequest> { };
	struct ShapeWindowRequest final : public ecs::Event<ShapeWindowRequest> { };
	struct SplineWindowRequest final : public ecs::Event<SplineWindowRequest> { };
}

namespace dbg::level
{
	struct ReloadRequest final : public ecs::Event<ReloadRequest> { };
	struct SaveRequest final : public ecs::Event<SaveRequest> { };
	struct SaveAsRequest final : public ecs::Event<SaveAsRequest> { };
}

namespace editor
{
	struct AssetBrowserWindowRequest final : public ecs::Event<AssetBrowserWindowRequest> { };
	struct EntityWindowRequest final : public ecs::Event<EntityWindowRequest> { };
	struct FlipbookWindowRequest final : public ecs::Event<FlipbookWindowRequest> { };
	struct InputWindowRequest final : public ecs::Event<InputWindowRequest> { };
	struct SettingsWindowRequest final : public ecs::Event<SettingsWindowRequest> { };
	struct SpriteWindowRequest final : public ecs::Event<SpriteWindowRequest> { };
	struct TableWindowRequest final : public ecs::Event<TableWindowRequest> { };
	struct TextureWindowRequest final : public ecs::Event<TextureWindowRequest> { };
	struct TrajectoryWindowRequest final : public ecs::Event<TrajectoryWindowRequest> { };
}

namespace editor::settings
{
	struct WindowRequest final : public ecs::Event<WindowRequest> { };
}