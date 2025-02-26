#pragma once

#include "ECS/Component.h"

namespace eng::application
{
	/// \brief Request to close the application.
	struct CloseRequestComponent : public ecs::EventComponent<CloseRequestComponent>
	{
	}; 
}