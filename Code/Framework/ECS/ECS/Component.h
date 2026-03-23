#pragma once

namespace ecs
{
	/// \brief Data type that can be added /removed from an entity and be queried.
	struct Component { };

	/// \brief Component that is automatically removed at the end of the next frame.
	struct FrameComponent : public ecs::Component { };

	/// \brief Component that can only be added to a single entity.
	struct SoloComponent : public ecs::Component { };
}