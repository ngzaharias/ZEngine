#pragma once

#include <Core/Rotator.h>
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

	struct TransformPrototype final
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate     = Rotator::Zero;
		Vector3f m_Scale     = Vector3f::One;
	};

	class TransformLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::TransformPrototype& prototype) const;
		void Load(eng::TransformPrototype& prototype, eng::Visitor& visitor) const;
	};
}