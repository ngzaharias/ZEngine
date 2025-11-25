#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::loading
{
	struct CloseRequest final : public ecs::Event<CloseRequest> { };
}
