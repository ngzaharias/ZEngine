#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct AssetBrowserWindowRequest final : public ecs::Event<AssetBrowserWindowRequest> { };
}