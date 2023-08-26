#pragma once

#include <Core/Vector.h>
#include <Core/String.h>

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct AmbientLightPrototype final : public eng::Prototype
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct DirectionalLightPrototype final : public eng::Prototype
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct PointLightPrototype final : public eng::Prototype
	{
		float m_Range = 500.f;
		Vector3f m_Colour = Vector3f::One;
	};

	class LightPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const AmbientLightPrototype& prototype) const;
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const DirectionalLightPrototype& prototype) const;
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const PointLightPrototype& prototype) const;

		void Load(AmbientLightPrototype& prototype, eng::Visitor& visitor) const;
		void Load(DirectionalLightPrototype& prototype, eng::Visitor& visitor) const;
		void Load(PointLightPrototype& prototype, eng::Visitor& visitor) const;
	};
}