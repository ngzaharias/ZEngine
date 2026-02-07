#pragma once

#include "Core/Colour.h"
#include "Core/Guid.h"
#include "Core/Optional.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace eng
{
	// \brief 
	struct SpriteComponent final : public ecs::Component<SpriteComponent>
		, ecs::IsPrototype
	{
		str::Guid m_Sprite = {};
		
		Optional<Colour> m_Colour = {};
		Optional<Vector2u> m_Size = {};
	};
}