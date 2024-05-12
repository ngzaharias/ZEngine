#pragma once

#include <Core/Rotator.h>
#include <Core/Vector.h>

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>
#include <Engine/TomlTypes.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct TransformPrototype final : public eng::Prototype
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate     = Rotator::Zero;
		Vector3f m_Scale     = Vector3f::One;
	};

	class TransformPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const TransformPrototype& prototype) const;
		void Load(eng::TransformPrototype& prototype, eng::Visitor& visitor) const;
	};
}