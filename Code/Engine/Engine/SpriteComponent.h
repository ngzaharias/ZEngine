#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng
{
	struct SpriteComponent : public ecs::Component<SpriteComponent>
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = { };
	};
}