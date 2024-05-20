#pragma once

#include <Core/Vector.h>

#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct AmbientLightPrototype final
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct DirectionalLightPrototype final
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct PointLightPrototype final
	{
		float m_Range = 500.f;
		Vector3f m_Colour = Vector3f::One;
	};

	class LightLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::AmbientLightPrototype& prototype) const;
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::DirectionalLightPrototype& prototype) const;
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::PointLightPrototype& prototype) const;

		void Load(eng::AmbientLightPrototype& prototype, eng::Visitor& visitor) const;
		void Load(eng::DirectionalLightPrototype& prototype, eng::Visitor& visitor) const;
		void Load(eng::PointLightPrototype& prototype, eng::Visitor& visitor) const;
	};
}