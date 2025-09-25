#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		str::Guid m_Group = {};
	};
}