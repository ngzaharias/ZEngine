#pragma once

#include "ECS/Event.h"

namespace editor
{
	struct TextureWindowRequest final : public ecs::Event<TextureWindowRequest> { };
}