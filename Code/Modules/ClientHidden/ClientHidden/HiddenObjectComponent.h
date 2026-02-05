#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Vector.h"

namespace client::hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent final : public ecs::Component<ObjectComponent>
		, ecs::IsPrototype
	{
		str::Guid m_Group = {};
	};
}