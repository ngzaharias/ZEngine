#pragma once

#include "ECS/Component.h"

namespace gamestate
{
	struct EditModeComponent final : public ecs::Component<EditModeComponent> { };
};