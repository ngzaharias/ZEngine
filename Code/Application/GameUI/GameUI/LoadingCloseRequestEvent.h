#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::loading
{
	struct CloseRequestEvent final : public ecs::Event<CloseRequestEvent> { };
}
