#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct AssetBrowserWindowEvent final : public ecs::Event<AssetBrowserWindowEvent> { };
}