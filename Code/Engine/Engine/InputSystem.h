#pragma once

#include "Core/Input.h"
#include "Core/Set.h"
#include "ECS/System.h"

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	class WindowManager;
	struct InputComponent;
}

namespace eng
{
	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			// Resources
			const eng::WindowManager,
			// Components
			ecs::NameComponent,
			eng::InputComponent>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		Set<input::EKeyboard> m_KeyboardPrevious;
		Set<input::EKeyboard> m_KeyboardCurrent;
		Set<input::EMouse> m_MousePrevious;
		Set<input::EMouse> m_MouseCurrent;
	};
}