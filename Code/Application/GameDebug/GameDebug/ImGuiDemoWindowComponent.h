#pragma once

#include "ECS/Component.h"

namespace debug
{
	struct ImGuiDemoWindowComponent final : public ecs::Component<ImGuiDemoWindowComponent> { };
};