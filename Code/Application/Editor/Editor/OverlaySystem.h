#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformSingleton;
}

namespace editor::outliner
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

namespace gamestate
{
	struct EditModeComponent;
}

namespace editor
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			editor::gizmo::TransformSingleton,
			editor::outliner::SelectSingleton,
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			eng::ActiveComponent,
			eng::EditorComponent,
			eng::settings::DebugSingleton,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}