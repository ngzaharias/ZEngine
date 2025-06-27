#pragma once

#include "ECS/Component.h"

namespace gui::settings
{
	struct CloseRequestComponent final : public ecs::EventComponent<CloseRequestComponent> { };
	struct OpenRequestComponent final : public ecs::EventComponent<OpenRequestComponent> { };

	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
	};
}