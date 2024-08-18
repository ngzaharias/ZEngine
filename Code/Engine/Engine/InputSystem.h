#pragma once

#include "Core/Input.h"
#include "Core/Set.h"
#include "ECS/System.h"

namespace glfw
{
	class Window;
}

namespace ecs
{
	struct NameComponent;
}

namespace eng
{
	struct InputComponent;

	class InputSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			ecs::NameComponent,
			eng::InputComponent,
			const glfw::Window>;

		void Initialise(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		Set<input::EKeyboard> m_KeyboardPrevious;
		Set<input::EKeyboard> m_KeyboardCurrent;
		Set<input::EMouse> m_MousePrevious;
		Set<input::EMouse> m_MouseCurrent;
	};
}