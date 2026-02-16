#pragma once

#include "ECS/Component.h"

namespace settings
{
	struct EditorComponent final : public ecs::Component<EditorComponent>
	{
	};
};