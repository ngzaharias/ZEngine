#pragma once

#include "ECS/Component.h"

namespace debug
{
	struct FrameBufferWindowComponent final : public ecs::Component<FrameBufferWindowComponent> { };
};