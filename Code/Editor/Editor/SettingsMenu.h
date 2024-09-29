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
	struct WindowRequestComponent;
}

namespace editor::settings
{
	struct WindowComponent : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};

		LocalComponent m_Local = {};
	};

	class MenuSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			editor::settings::LocalComponent,
			editor::settings::WindowComponent,
			const editor::settings::WindowRequestComponent>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}