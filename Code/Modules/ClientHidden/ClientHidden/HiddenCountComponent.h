#pragma once

#include "ECS/Component.h"

namespace client::hidden
{
	/// \brief Counts the number of objects that are hidden and revealed.
	struct CountComponent final : public ecs::Component<CountComponent>
	{
		int32 m_Objects = 0;
		int32 m_Revealed = 0;
	};
}