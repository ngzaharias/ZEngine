#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace hidden
{
	/// \brief Group of hidden objects.
	struct GroupComponent : public ecs::Component<GroupComponent>
	{
		str::Guid m_Sprite = {};
		Vector2u m_Size = Vector2u::Zero;

		Array<str::Guid> m_Objects = {};
		Set<str::Guid> m_Revealed = {};
	};
}