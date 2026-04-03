#pragma once

#include <concepts>

namespace ecs
{
	/// \brief Data type that can be added /removed from an entity and be queried.
	struct Component { };

	/// \brief Component that is automatically removed at the end of the next frame.
	struct FrameComponent : public ecs::Component { };

	/// \brief Component that can only have one of each type present at a time.
	struct SoloComponent : public ecs::Component { };

	/// \brief Component that can only have one of each type present at a time and 
	/// is present for the entire lifetime of the world.
	struct StaticComponent : public ecs::Component { };

	/// \brief 
	struct TemplateComponent : public ecs::Component { };

	template<typename TComponent>
	concept IsComponent = std::derived_from<TComponent, ecs::Component>;
	template<typename TComponent>
	concept IsFrameComponent = std::derived_from<TComponent, ecs::FrameComponent>;
	template<typename TComponent>
	concept IsSoloComponent = std::derived_from<TComponent, ecs::SoloComponent>;
	template<typename TComponent>
	concept IsStaticComponent = std::derived_from<TComponent, ecs::StaticComponent>;
	template<typename TComponent>
	concept IsTemplateComponent = std::derived_from<TComponent, ecs::TemplateComponent>;

	template<typename TComponent>
	concept IsSoloOrStaticComponent =
		std::derived_from<TComponent, ecs::SoloComponent> ||
		std::derived_from<TComponent, ecs::StaticComponent>;
}