#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct CameraComponent;
	struct CameraTemplate;
}

namespace eng
{
	/// \brief
	class CameraSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::CameraComponent>
			::Read<
			eng::CameraTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
}