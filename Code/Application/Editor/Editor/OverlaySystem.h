#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformSingleton;
}

namespace editor::entity
{
	struct SelectSingleton;
}

namespace eng
{
	class WindowManager;
	struct ActiveComponent;
	struct CameraComponent;
	struct EditorComponent;
	struct TransformComponent;
}

namespace eng::settings
{
	struct DebugSingleton;
}

namespace settings
{
	struct EditorComponent;
}

namespace editor
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::gizmo::TransformSingleton,
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			eng::ActiveComponent,
			eng::EditorComponent,
			eng::settings::DebugSingleton,
			eng::WindowManager,
			::settings::EditorComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}