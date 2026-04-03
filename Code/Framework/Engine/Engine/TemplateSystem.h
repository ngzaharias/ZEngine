#pragma once

#include "ECS/System.h"
#include "ECS/WorldView.h"

namespace eng
{
	struct TemplateComponent;
	struct TemplateMapComponent;
}

namespace eng
{
	class TemplateSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView
			::Write<
			eng::TemplateMapComponent>
			::Read<
			eng::TemplateComponent>;

		void Update(World& world, const GameTime& gameTime);
	};
}