#pragma once

#include "ECS/Event.h"

namespace dbg
{
	struct ImGuiDemoRequest final : public ecs::Event<ImGuiDemoRequest> { };
}