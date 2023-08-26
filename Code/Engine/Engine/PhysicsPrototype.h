#pragma once

#include <Core/Variant.h>
#include <Core/Vector.h>

#include <Engine/Prototype.h>
#include <Engine/PrototypeLoader.h>

namespace ecs
{
	class EntityWorld;
	struct Entity;
}

namespace eng
{
	class PhysicsManager;
	class Visitor;

	struct ShapeBox
	{
		Vector3f m_Extents = Vector3f(0.f);
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

	struct PhysicsPrototype final : public eng::Prototype
	{
		Shape m_Shape = { };
		Rigidbody m_Rigidbody = { };
	};

	class PhysicsPrototypeLoader final : public eng::PrototypeLoader
	{
	public:
		PhysicsPrototypeLoader(eng::PhysicsManager& physicsManager);

		void Add(ecs::EntityWorld& world, const ecs::Entity& entity, const PhysicsPrototype& prototype) const;
		void Load(PhysicsPrototype& prototype, eng::Visitor& visitor) const;

	private:
		eng::PhysicsManager& m_PhysicsManager;
	};
}