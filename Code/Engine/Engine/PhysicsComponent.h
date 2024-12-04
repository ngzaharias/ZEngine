#pragma once

#include "Core/Array.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

namespace physx
{
	class PxRigidActor;
	class PxShape;
}

namespace eng
{
	struct RigidDynamic
	{
		bool eENABLE_CCD = false;
		bool eKINEMATIC = true;
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

	struct ShapeBox
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;

		Vector3f m_Extents = Vector3f::Zero;
		int32 m_Channel = 0;
	};

	struct ShapeSphere
	{
		Vector3f m_Translate = Vector3f::Zero;

		float m_Radius = 0.f;
		int32 m_Channel = 0;
	};

	using Shape = Variant<ShapeBox, ShapeSphere>;
	using Rigidbody = Variant<RigidDynamic, RigidStatic>;

	struct PhysicsComponent final : ecs::Component<PhysicsComponent>
	{
		Rigidbody m_Rigidbody = { };
		Array<Shape> m_Shapes = { };

		physx::PxRigidActor* m_PxRigidActor = nullptr;
		Array<physx::PxShape*> m_PxShapes = {};
	};
}