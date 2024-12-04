#include "EnginePCH.h"
#include "Engine/PhysicsHelpers.h"

#include "Engine/PhysicsComponent.h"
#include "Math/Quaternion.h"

#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxScene.h>

#include <variant>

physx::PxQuat eng::physics::ToQuaternion(const Quaternion& value)
{
	physx::PxQuat quaternion;
	quaternion.x = value.x;
	quaternion.y = value.y;
	quaternion.z = value.z;
	quaternion.w = value.w;
	return quaternion;
}

physx::PxQuat eng::physics::ToQuaternion(const Rotator& value)
{
	return ToQuaternion(Quaternion::FromRotator(value));
}

physx::PxVec3 eng::physics::ToVector(const Vector3f& value)
{
	physx::PxVec3 vector;
	vector.x = value.x;
	vector.y = value.y;
	vector.z = value.z;
	return vector;
}

physx::PxTransform eng::physics::ToTransform(const Vector3f& translate, const Rotator& rotation)
{
	physx::PxTransform transform;
	transform.p = eng::physics::ToVector(translate);
	transform.q = eng::physics::ToQuaternion(rotation);
	return transform;
}

physx::PxRigidDynamic* eng::physics::CreateRigidbody(physx::PxPhysics& physics, const eng::RigidDynamic& data)
{
	physx::PxRigidDynamic* actor = physics.createRigidDynamic(physx::PxTransform(physx::PxIdentity));
	actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, data.eKINEMATIC);
	actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, data.eENABLE_CCD);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, data.eLOCK_ANGULAR_X);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, data.eLOCK_ANGULAR_Y);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, data.eLOCK_ANGULAR_Z);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, data.eLOCK_LINEAR_X);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, data.eLOCK_LINEAR_Y);
	actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, data.eLOCK_LINEAR_Z);
	return actor;
}

physx::PxRigidStatic* eng::physics::CreateRigidbody(physx::PxPhysics& physics, const eng::RigidStatic& data)
{
	physx::PxRigidStatic* actor = physics.createRigidStatic(physx::PxTransform(physx::PxIdentity));
	return actor;
}

physx::PxShape* eng::physics::CreateShape(physx::PxPhysics& physics, const eng::ShapeBox& data, const physx::PxMaterial& material)
{
	physx::PxBoxGeometry geometry;
	geometry.halfExtents = ToVector(data.m_Extents);

	physx::PxFilterData filterData;
	filterData.word0 = data.m_Channel;

	physx::PxTransform transform;
	transform.p = ToVector(data.m_Translate);
	transform.q = ToQuaternion(data.m_Rotate);

	if (physx::PxShape* shape = physics.createShape(geometry, material, true))
	{
		shape->setLocalPose(transform);
		shape->setSimulationFilterData(filterData);
		return shape;
	}
	return nullptr;
}

physx::PxShape* eng::physics::CreateShape(physx::PxPhysics& physics, const eng::ShapeSphere& data, const physx::PxMaterial& material)
{
	physx::PxSphereGeometry geometry;
	geometry.radius = data.m_Radius;

	physx::PxFilterData filterData;
	filterData.word0 = data.m_Channel;

	physx::PxTransform transform;
	transform.p = ToVector(data.m_Translate);
	transform.q = physx::PxQuat(physx::PxIdentity);

	if (physx::PxShape* shape = physics.createShape(geometry, material, true))
	{
		shape->setSimulationFilterData(filterData);
		shape->setLocalPose(transform);
		return shape;
	}
	return nullptr;
}