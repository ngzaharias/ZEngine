#pragma once

#include "Core/Array.h"
#include "Core/Variant.h"
#include "Engine/PrototypeLoader.h"
#include "Math/Vector.h"

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class Visitor;

	struct ShapeBox
	{
		Vector3f m_Extents = Vector3f::Zero;
		int32 m_Channel = 0;
	};

	struct ShapeSphere
	{
		float m_Radius = 0.f;
		int32 m_Channel = 0;
	};

	struct RigidDynamic
	{
		bool eENABLE_CCD = false;
		bool eKINEMATIC = false;
		bool eLOCK_ANGULAR_X = false;
		bool eLOCK_ANGULAR_Y = false;
		bool eLOCK_ANGULAR_Z = false;
		bool eLOCK_LINEAR_X = false;
		bool eLOCK_LINEAR_Y = false;
		bool eLOCK_LINEAR_Z = false;
	};

	struct RigidStatic
	{
	};

	using Shape = Variant<ShapeBox, ShapeSphere>;
	using Rigidbody = Variant<RigidDynamic, RigidStatic>;

	struct PhysicsPrototype final
	{
		Rigidbody m_Rigidbody = { };
		Array<Shape> m_Shapes = { };
	};

	class PhysicsLoader final : public eng::PrototypeLoader
	{
	public:
		void Add(ecs::EntityWorld& entityWorld, const ecs::Entity& entity, const eng::PhysicsPrototype& prototype) const;
		void Load(eng::PhysicsPrototype& prototype, eng::Visitor& visitor) const;
	};
}