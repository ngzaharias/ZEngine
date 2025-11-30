#pragma once

#include "ECS/EntityWorld.h"
#include "ECS/System.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace editor::settings
{
	struct WindowRequest;
}

namespace editor::settings
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		LocalSingleton m_Local = {};
	};

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