#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformComponent;
}

namespace editor::outliner
{
	struct SelectComponent;
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
	struct DebugComponent;
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
			editor::gizmo::TransformComponent,
			editor::outliner::SelectComponent,
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			eng::ActiveComponent,
			eng::EditorComponent,
			eng::settings::DebugComponent,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}