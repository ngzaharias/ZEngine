#pragma once

#include <Core/Guid.h>
#include <Core/Vector.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>

namespace hidden
{
	struct ObjectComponent : public ecs::Component<ObjectComponent> 
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = {};
	};
}