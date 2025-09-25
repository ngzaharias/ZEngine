#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"

namespace eng::settings
{
	/// \brief 
	struct GameplayComponent : public ecs::SingletonComponent<GameplayComponent>
	{
		str::Guid m_Theme = GUID("605947d5630f4508921b83738d2b6869");
	};
}