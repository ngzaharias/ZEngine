#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Sprite = {};
	};

	/// \brief Added to a hidden object entity when it has been revealed.
	struct RevealedComponent : public ecs::Component<RevealedComponent>	{ };
}