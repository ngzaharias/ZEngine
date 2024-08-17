#pragma once

#include "ECS/System.h"

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct TransformComponent;

	class CameraSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			const eng::CameraComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}