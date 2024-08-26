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
	struct Bound2D
	{
		Vector2f m_Max = Vector2f(+KINDA_LARGE_FLOAT);
		Vector2f m_Min = Vector2f(-KINDA_LARGE_FLOAT);
	};

	struct Move2D {};
	struct Move3D {};

	struct Pan3D {};

	struct Zoom2D
	{
		float m_Max = KINDA_LARGE_FLOAT;
		float m_Min = 1.f;
	};

	using BehaviourBounded = Variant<Bound2D>;
	using BehaviourMovement = Variant<Move2D, Move3D>;
	using BehaviourPanning = Variant<Pan3D>;
	using BehaviourZooming = Variant<Zoom2D>;

	struct Prototype final
	{
		Projection m_Projection = Perspective();

		Optional<BehaviourBounded> m_Bounded = std::nullopt;
		Optional<BehaviourMovement> m_Movement = std::nullopt;
		Optional<BehaviourPanning> m_Panning = std::nullopt;
		Optional<BehaviourZooming> m_Zooming = std::nullopt;
	};

	class Loader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const Prototype& prototype) const;
		void Load(Prototype& prototype, eng::Visitor& visitor) const;
	};
}