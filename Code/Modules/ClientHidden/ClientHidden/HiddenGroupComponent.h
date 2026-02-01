#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace client::hidden
{
	/// \brief Group of hidden objects.
	struct GroupComponent final : public ecs::Component<GroupComponent>
	{
		Array<str::Guid> m_Objects = {};
		Array<str::Guid> m_Revealed = {};
	};
}