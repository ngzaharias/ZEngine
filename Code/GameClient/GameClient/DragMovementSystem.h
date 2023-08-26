#pragma once

#include <ECS/System.h>

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct NameComponent;
	struct TransformComponent;
}

namespace drag
{
	struct SelectionComponent;

	class MovementSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::LinesComponent,
			eng::TransformComponent,
			const drag::SelectionComponent,
			const eng::CameraComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}