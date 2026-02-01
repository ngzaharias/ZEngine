#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace client::hidden
{
	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent final : public ecs::Component<ObjectComponent>
	{
		str::Guid m_Group = {};
	};
}