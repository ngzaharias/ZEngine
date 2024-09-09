#include "EnginePCH.h"
#include "Engine/PhysicsPrototype.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialAsset.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Visitor.h"
#include "Math/Quaternion.h"

#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxScene.h>
#include <PhysX/PxShape.h>
#include <PhysX/foundation/PxVec3.h>
#include <PhysX/foundation/PxQuat.h>

namespace
{
	const str::Guid strDefaultMaterial = GUID("a4835493-ae5a-40ba-8083-06deb381c801");

	const str::StringView strChannel = "m_Channel";
	const str::StringView strExtents = "m_Extents";
	const str::StringView strRadius = "m_Radius";
	const str::StringView strRigid = "m_Rigid";
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strShape = "m_Shape";
	const str::StringView strShapes = "m_Shapes";
	const str::StringView strTranslate = "m_Translate";

	const str::StringView strENABLE_CCD = "eENABLE_CCD";
	const str::StringView strKINEMATIC = "eKINEMATIC";
	const str::StringView strLOCK_ANGULAR_X = "eLOCK_ANGULAR_X";
	const str::StringView strLOCK_ANGULAR_Y = "eLOCK_ANGULAR_Y";
	const str::StringView strLOCK_ANGULAR_Z = "eLOCK_ANGULAR_Z";
	const str::StringView strLOCK_LINEAR_X = "eLOCK_LINEAR_X";
	const str::StringView strLOCK_LINEAR_Y = "eLOCK_LINEAR_Y";
	const str::StringView strLOCK_LINEAR_Z = "eLOCK_LINEAR_Z";

	physx::PxTransform ToTransform(const Vector3f& translate, const Rotator& rotate)
	{
		physx::PxTransform transform;
		transform.p.x = translate.x;
		transform.p.y = translate.y;
		transform.p.z = translate.z;

		const Quaternion quat = Quaternion::FromRotator(rotate);
		transform.q.x = quat.x;
		transform.q.y = quat.y;
		transform.q.z = quat.z;
		transform.q.w = quat.w;

		return transform;
	}
}

template<>
void eng::Visitor::ReadCustom(eng::RigidDynamic& value) const
{
	Read(strENABLE_CCD, value.eENABLE_CCD, value.eENABLE_CCD);
	Read(strKINEMATIC, value.eKINEMATIC, value.eKINEMATIC);
	Read(strLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X);
	Read(strLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y);
	Read(strLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z);
	Read(strLOCK_LINEAR_X, value.eLOCK_LINEAR_X, value.eLOCK_LINEAR_X);
	Read(strLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y);
	Read(strLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z);
}

template<>
void eng::Visitor::ReadCustom(eng::RigidStatic& value) const
{
}

template<>
void eng::Visitor::ReadCustom(eng::ShapeBox& value) const
{
	Read(strExtents, value.m_Extents, value.m_Extents);
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strRotate, value.m_Rotate, value.m_Rotate);
	Read(strChannel, value.m_Channel, value.m_Channel);
}

template<>
void eng::Visitor::ReadCustom(eng::ShapeSphere& value) const
{
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strChannel, value.m_Channel, value.m_Channel);
	Read(strRadius, value.m_Radius, value.m_Radius);
}

template<>
void eng::Visitor::WriteCustom(const eng::RigidDynamic& value)
{
	Write(strENABLE_CCD, value.eENABLE_CCD);
	Write(strKINEMATIC, value.eKINEMATIC);
	Write(strLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X);
	Write(strLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y);
	Write(strLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z);
	Write(strLOCK_LINEAR_X, value.eLOCK_LINEAR_X);
	Write(strLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y);
	Write(strLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z);
}

template<>
void eng::Visitor::WriteCustom(const eng::PhysicsPrototype& value)
{
	Write(strRigid, value.m_Rigidbody);
	Write(strShapes, value.m_Shapes);
}

template<>
void eng::Visitor::WriteCustom(const eng::RigidStatic& value)
{
}

template<>
void eng::Visitor::WriteCustom(const eng::ShapeBox& value)
{
	Write(strExtents, value.m_Extents);
	Write(strTranslate, value.m_Translate);
	Write(strRotate, value.m_Rotate);
	Write(strChannel, value.m_Channel);
}

template<>
void eng::Visitor::WriteCustom(const eng::ShapeSphere& value)
{
	Write(strTranslate, value.m_Translate);
	Write(strChannel, value.m_Channel);
	Write(strRadius, value.m_Radius);
}

void eng::PhysicsLoader::Add(ecs::EntityWorld& entityWorld, const ecs::Entity& entity, const eng::PhysicsPrototype& prototype) const
{
	using World = ecs::WorldView<
		eng::AssetManager,
		eng::PhysicsManager,
		eng::RigidDynamicComponent,
		eng::RigidStaticComponent>;
	World world = entityWorld.GetWorldView<World>();

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	auto& physicsManager = world.WriteResource<eng::PhysicsManager>();
	const auto* asset = assetManager.LoadAsset<eng::PhysicsMaterialAsset>(strDefaultMaterial);

	physx::PxPhysics& physics = physicsManager.GetPhysics();

	Array<physx::PxShape*> shapes = { };
	for (const eng::Shape& shape : prototype.m_Shapes)
	{
		if (std::holds_alternative<ShapeBox>(shape))
		{
			const auto& data = std::get<ShapeBox>(shape);

			physx::PxBoxGeometry geometry;
			geometry.halfExtents.x = data.m_Extents.x;
			geometry.halfExtents.y = data.m_Extents.y;
			geometry.halfExtents.z = data.m_Extents.z;

			physx::PxFilterData filterData;
			filterData.word0 = data.m_Channel;

			if (physx::PxShape* pShape = physics.createShape(geometry, *asset->m_Material, true))
			{
				const physx::PxTransform transform = ToTransform(data.m_Translate, data.m_Rotate);

				pShape->setLocalPose(transform);
				pShape->setSimulationFilterData(filterData);
				shapes.Append(pShape);
			}
		}
		else if (std::holds_alternative<ShapeSphere>(shape))
		{
			const auto& data = std::get<ShapeSphere>(shape);

			physx::PxSphereGeometry geometry;
			geometry.radius = data.m_Radius;

			physx::PxFilterData filterData;
			filterData.word0 = data.m_Channel;

			if (physx::PxShape* pShape = physics.createShape(geometry, *asset->m_Material, true))
			{
				const physx::PxTransform transform = ToTransform(data.m_Translate, Rotator::Zero);

				pShape->setLocalPose(transform);
				pShape->setSimulationFilterData(filterData);
				shapes.Append(pShape);
			}
		}
	}

	if (std::holds_alternative<RigidDynamic>(prototype.m_Rigidbody))
	{
		const auto& data = std::get<RigidDynamic>(prototype.m_Rigidbody);

		auto& component = world.AddComponent<eng::RigidDynamicComponent>(entity);
		component.m_Actor = physics.createRigidDynamic({ physx::PxVec3(0.f), physx::PxQuat(physx::PxIdentity) });
		component.m_Actor->userData = reinterpret_cast<void*>(entity.m_Value);
		component.m_Actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, data.eKINEMATIC);
		component.m_Actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, data.eENABLE_CCD);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, data.eLOCK_ANGULAR_X);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, data.eLOCK_ANGULAR_Y);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, data.eLOCK_ANGULAR_Z);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, data.eLOCK_LINEAR_X);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, data.eLOCK_LINEAR_Y);
		component.m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, data.eLOCK_LINEAR_Z);
		component.m_IsKinematic = data.eKINEMATIC;

		for (auto* shape : shapes)
		{
			component.m_Actor->attachShape(*shape);
			component.m_Shapes.Append(shape);
		}
	}
	else if (std::holds_alternative<RigidStatic>(prototype.m_Rigidbody))
	{
		const auto& data = std::get<RigidStatic>(prototype.m_Rigidbody);

		auto& component = world.AddComponent<eng::RigidStaticComponent>(entity);
		component.m_Actor = physics.createRigidStatic({ physx::PxVec3(0.f), physx::PxQuat(physx::PxIdentity) });
		component.m_Actor->userData = reinterpret_cast<void*>(entity.m_Value);

		for (auto* shape : shapes)
		{
			component.m_Actor->attachShape(*shape);
			component.m_Shapes.Append(shape);
		}
	}
}

void eng::PhysicsLoader::Load(eng::PhysicsPrototype& prototype, eng::Visitor& visitor) const
{
	visitor.Read(strRigid, prototype.m_Rigidbody, prototype.m_Rigidbody);
	visitor.Read(strShapes, prototype.m_Shapes, prototype.m_Shapes);
}