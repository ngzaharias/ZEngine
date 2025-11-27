#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "GameDebug/FPSCounter.h"

namespace eng
{
	class WindowManager;
	struct VersionSingleton;
}

namespace dbg
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
		dbg::FPSCounter m_FPSCounter;
	};
}