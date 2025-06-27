#pragma once

class Quaternion;
class Rotator;
class Vector3f;

namespace eng
{
	struct RigidDynamic;
	struct RigidStatic;
	struct ShapeBox;
	struct ShapeSphere;
}

namespace physx
{
	class PxMaterial;
	class PxPhysics;
	class PxQuat;
	class PxRigidBody;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxShape;
	class PxTransform;
	class PxVec3;
}

namespace eng::physics
{
	physx::PxQuat ToQuaternion(const Quaternion& value);
	physx::PxQuat ToQuaternion(const Rotator& value);
	physx::PxVec3 ToVector(const Vector3f& value);
	physx::PxTransform ToTransform(const Vector3f& translate, const Rotator& rotation);

	physx::PxRigidDynamic* CreateRigidbody(physx::PxPhysics& physics, const eng::RigidDynamic& data);
	physx::PxRigidStatic* CreateRigidbody(physx::PxPhysics& physics, const eng::RigidStatic& data);

	physx::PxShape* CreateShape(physx::PxPhysics& physics, const eng::ShapeBox& data, const physx::PxMaterial& material);
	physx::PxShape* CreateShape(physx::PxPhysics& physics, const eng::ShapeSphere& data, const physx::PxMaterial& material);
}