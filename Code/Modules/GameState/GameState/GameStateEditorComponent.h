#pragma once

#include "ECS/Component.h"

namespace gamestate
{
	struct EditorComponent final : public ecs::Component<EditorComponent> { };
};