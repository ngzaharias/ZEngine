#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::loading
{
	struct CloseEvent final : public ecs::Event { };
}
