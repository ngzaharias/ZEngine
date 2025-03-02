#pragma once

#include "Engine/Window.h"

namespace eng
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		eng::Window* m_Window = nullptr;
	};
}
