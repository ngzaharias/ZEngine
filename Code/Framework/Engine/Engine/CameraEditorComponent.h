#pragma once

#include "ECS/Component.h"

namespace eng::camera
{
	struct EditorComponent final : public ecs::Component<EditorComponent> {};
}