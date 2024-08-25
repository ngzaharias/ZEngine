#pragma once

#include "Core/Optional.h"
#include "Engine/CameraTypes.h"
#include "Engine/PrototypeLoader.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;
}

namespace eng::camera
{
	struct Prototype final
	{
		Projection m_Projection = Perspective();

		Optional<BehaviourBound> m_Bound = std::nullopt;
		Optional<BehaviourMove> m_Move = std::nullopt;
		Optional<BehaviourZoom> m_Zoom = std::nullopt;
	};

	class Loader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const Prototype& prototype) const;
		void Load(Prototype& prototype, eng::Visitor& visitor) const;
	};
}