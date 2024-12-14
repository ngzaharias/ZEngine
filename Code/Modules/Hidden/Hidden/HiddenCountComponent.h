#pragma once

#include "ECS/Component.h"

namespace hidden
{
	/// \brief Counts the number of objects that are hidden and revealed.
	struct CountComponent : public ecs::Component<CountComponent>
	{
		int32 m_Objects = 0;
		int32 m_Revealed = 0;
	};
}