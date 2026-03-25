#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct ActiveComponent;
	struct CameraComponent;
	struct LinesComponent;
	struct TransformComponent;
}

namespace editor::settings
{
	struct LocalComponent;
}

namespace editor::gizmo
{
	class CrosshairSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesComponent>
			::Read<
			editor::settings::LocalComponent,
			eng::ActiveComponent,
			eng::CameraComponent,
			eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
