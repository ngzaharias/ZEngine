#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "GameDebug/FPSCounter.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
}

namespace eng
{
	class WindowManager;
	struct VersionSingleton;
}

namespace debug
{
	class OverlaySystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Singletons
			const eng::VersionSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		debug::FPSCounter m_FPSCounter;
	};
}