#pragma once

#include "ECS/Component.h"

namespace hidden
{
	/// \brief Added to a hidden object entity when it has been revealed.
	struct RevealComponent : public ecs::Component<RevealComponent> { };
}