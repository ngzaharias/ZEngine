#pragma once

#include "ECS/Component.h"

namespace dbg::level
{
	struct OpenWindowComponent final : public ecs::Component<OpenWindowComponent> { };
}