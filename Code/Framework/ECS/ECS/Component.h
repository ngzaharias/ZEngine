#pragma once

namespace ecs
{
	/// \brief Data type that can be added /removed from an entity and be queried.
	template<typename TComponent>
	struct Component { };

	/// \brief Component that is automatically removed at the end of the next frame.
	template<typename TComponent>
	struct FrameComponent : public ecs::Component<TComponent> { };
}