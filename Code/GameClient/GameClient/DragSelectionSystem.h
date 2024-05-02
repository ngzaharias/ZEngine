#pragma once

#include <ECS/System.h>

namespace eng
{
	struct CameraComponent;
	struct InputComponent;
	struct LinesComponent;
	struct NameComponent;
	struct PhysicsSceneComponent;
	struct TransformComponent;
}

namespace drag
{
	struct IsSelectableComponent;
	struct SelectionComponent;

	class SelectionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			drag::SelectionComponent,
			eng::NameComponent,
			eng::LinesComponent,
			const drag::IsSelectableComponent,
			const eng::CameraComponent,
			const eng::InputComponent,
			const eng::PhysicsSceneComponent,
			const eng::TransformComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}