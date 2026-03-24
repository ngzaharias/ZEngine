#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace editor::gizmo
{
	struct TransformStaticComponent;
}

namespace editor::outliner
{
	struct SelectStaticComponent;
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
	struct DebugStaticComponent;
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
			editor::gizmo::TransformStaticComponent,
			editor::outliner::SelectStaticComponent,
			eng::CameraComponent,
			eng::TransformComponent>
			::Read<
			eng::ActiveComponent,
			eng::EditorComponent,
			eng::settings::DebugStaticComponent,
			eng::WindowManager,
			gamestate::EditModeComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}