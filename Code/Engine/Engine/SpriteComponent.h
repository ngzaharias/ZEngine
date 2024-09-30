#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng
{
	struct SpriteComponent : public ecs::Component<SpriteComponent>
	{
		str::Guid m_Sprite = { };
		Vector3f m_Colour = Vector3f::One;
		Vector2u m_Size = Vector2u::Zero;
	};
}