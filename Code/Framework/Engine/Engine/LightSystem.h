#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng::light
{
	struct AmbientComponent;
	struct AmbientTemplate;
	struct DirectionalComponent;
	struct DirectionalTemplate;
	struct PointComponent;
	struct PointTemplate;
}

namespace eng
{
	class LightSystem : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::light::AmbientComponent,
			eng::light::DirectionalComponent,
			eng::light::PointComponent>
			::Read<
			eng::light::AmbientTemplate,
			eng::light::DirectionalTemplate,
			eng::light::PointTemplate>;

		void Update(World& world, const GameTime& gameTime);
	};
};