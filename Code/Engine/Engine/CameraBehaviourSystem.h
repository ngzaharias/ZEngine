#pragma once

#include "ECS/System.h"

namespace eng
{
	struct InputComponent;
	struct TransformComponent;
}

namespace eng::camera
{
	struct BehaviourComponent;
	struct ProjectionComponent;

	class BehaviourSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::TransformComponent,
			const eng::camera::BehaviourComponent,
			const eng::camera::ProjectionComponent,
			const eng::InputComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		void BehaviourFree2D(World& world, const GameTime& gameTime, const ecs::Entity& entity);
		void BehaviourFree3D(World& world, const GameTime& gameTime, const ecs::Entity& entity);
		void BehaviourStatic(World& world, const GameTime& gameTime, const ecs::Entity& entity);
	};
}