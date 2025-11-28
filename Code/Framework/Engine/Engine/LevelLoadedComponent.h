#pragma once

#include "Core/Name.h"
#include "Core/Path.h"
#include "ECS/Component.h"

namespace eng::level
{
	/// \brief There exists one of these components for every level that is loaded.
	struct LoadedComponent final : public ecs::Component<LoadedComponent>
	{
		str::Name m_Name = {};
		str::Path m_Path = {};
	};
}