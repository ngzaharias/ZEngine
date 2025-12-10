#pragma once

#include "ECS/Event.h"

namespace debug
{
	struct ImGuiDemoRequest final : public ecs::Event<ImGuiDemoRequest> { };
}