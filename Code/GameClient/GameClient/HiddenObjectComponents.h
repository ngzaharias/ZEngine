#pragma once

#include "Core/Guid.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace hidden
{
	/// \brief Group of hidden objects that are all based off the same riddle.
	struct GroupComponent : public ecs::Component<GroupComponent>
	{
		str::Guid m_Sprite = {};
		Vector2u m_Size = Vector2u::Zero;

		Set<str::Guid> m_Objects = {};
		Set<str::Guid> m_Revealed = {};
	};

	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		Vector2u m_Size = Vector2u::Zero;
		str::Guid m_Group = {};
		str::Guid m_Sprite = {};
	};

	/// \brief Added to a hidden object entity when it has been revealed.
	struct RevealedComponent : public ecs::Component<RevealedComponent> { };
}