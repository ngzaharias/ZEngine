#pragma once

#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	template <typename... TTypes>
	class WorldView;
	struct NameComponent;
}

namespace editor::settings
{
	struct LocalSingleton;
	struct WindowComponent;
	struct WindowRequest;
}

namespace editor::settings
{
	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Components
			ecs::NameComponent,
			editor::settings::WindowComponent,
			// Events
			const editor::settings::WindowRequest,
			// Singletons
			editor::settings::LocalSingleton>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}