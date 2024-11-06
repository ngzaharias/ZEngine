#pragma once

#include "Core/Guid.h"
#include "Core/Optional.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace eng
{
	struct SpriteComponent : public ecs::Component<SpriteComponent>
	{
		str::Guid m_Sprite = {};
		Vector3f m_Colour = Vector3f::One;
		Vector2u m_Size = Vector2u::Zero;
	};

	struct SpriteRequestComponent : public ecs::EventComponent<SpriteRequestComponent>
	{
		ecs::Entity m_Entity = {};
		Optional<str::Guid> m_Sprite = {};
		Optional<Vector3f> m_Colour = {};
		Optional<Vector2u> m_Size = {};
	};
}