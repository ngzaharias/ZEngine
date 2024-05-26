#pragma once

#include <Core/Guid.h>
#include <Core/Vector.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>

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