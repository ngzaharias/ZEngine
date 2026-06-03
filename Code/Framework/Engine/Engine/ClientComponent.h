#pragma once

#include "ECS/Component.h"

namespace eng
{
	/// \brief Component that is only present on client but registered on both.
	struct ClientComponent final : public ecs::SoloComponent
	{
	};
}