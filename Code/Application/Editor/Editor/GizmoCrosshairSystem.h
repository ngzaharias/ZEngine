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
	struct LinesSingleton;
	struct TransformComponent;
}

namespace editor::settings
{
	struct LocalSingleton;
}

namespace editor::gizmo
{
	class CrosshairSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::LinesSingleton>
			::Read<
			editor::settings::LocalSingleton,
			eng::CameraComponent,
			eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}
