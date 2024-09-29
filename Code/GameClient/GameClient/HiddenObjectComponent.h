#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		str::Guid m_Sprite = {};
		Vector2u m_Size = Vector2u::Zero;

		str::Guid m_Group = {};
	};
}