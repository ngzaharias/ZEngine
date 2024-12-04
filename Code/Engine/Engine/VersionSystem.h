#pragma once

#include "ECS/System.h"

namespace eng
{
	struct VersionComponent;

	class VersionSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<eng::VersionComponent>;

		void Initialise(World& world);
	};
}