#pragma once

#include "Core/Guid.h"
#include "ECS/Singleton.h"

namespace eng::settings
{
	/// \brief 
	struct GameplaySingleton final : public ecs::Singleton<GameplaySingleton>
	{
		str::Guid m_Theme = GUID("605947d5630f4508921b83738d2b6869");
	};
}