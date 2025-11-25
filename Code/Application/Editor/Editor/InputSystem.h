#pragma once

#include "ECS/Component.h"
#include "ECS/System.h"
#include "imgui/Identifier.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class InputManager;
}

namespace editor
{
	struct InputWindowRequest;

	struct InputWindowComponent : public ecs::Component<InputWindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};
	};

	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Managers
			eng::InputManager,
			// Components
			ecs::NameComponent,
			editor::InputWindowComponent,
			const editor::InputWindowRequest>;

		void Update(World& world, const GameTime& gameTime);

	private:
		imgui::Identifier m_WindowIds = {};
	};
}