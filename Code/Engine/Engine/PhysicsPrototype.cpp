#include "EnginePCH.h"
#include "Engine/PhysicsPrototype.h"

#include <Core/Algorithms.h>

#include <ECS/EntityWorld.h>

#include <PhysX/PxPhysics.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxScene.h>
#include <PhysX/PxShape.h>
#include <PhysX/foundation/PxVec3.h>
#include <PhysX/foundation/PxQuat.h>

#include "Engine/AssetManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialAsset.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Visitor.h"

namespace
{
	enum class ERigidbody
	{
		Dynamic,
		Static,
	};

	enum class EShape
	{
		Box,
		Sphere,
	};

	const str::Guid strDefaultMaterial = GUID("a4835493-ae5a-40ba-8083-06deb381c801");

	const str::StringView strChannel = "m_Channel";
	const str::StringView strExtents = "m_Extents";
	const str::StringView strRadius = "m_Radius";
	const str::StringView strRigid = "m_Rigid";
	const str::StringView strShape = "m_Shape";
	const str::StringView strTranslate = "m_Translate";

	const str::StringView strENABLE_CCD = "eENABLE_CCD";
	const str::StringView strKINEMATIC = "eKINEMATIC";
	const str::StringView strLOCK_ANGULAR_X = "eLOCK_ANGULAR_X";
	const str::StringView strLOCK_ANGULAR_Y = "eLOCK_ANGULAR_Y";
	const str::StringView strLOCK_ANGULAR_Z = "eLOCK_ANGULAR_Z";
	const str::StringView strLOCK_LINEAR_X = "eLOCK_LINEAR_X";
	const str::StringView strLOCK_LINEAR_Y = "eLOCK_LINEAR_Y";
	const str::StringView strLOCK_LINEAR_Z = "eLOCK_LINEAR_Z";
}

eng::PhysicsPrototypeLoader::PhysicsPrototypeLoader(eng::PhysicsManager& physicsManager)
	: m_PhysicsManager(physicsManager) 
{ 
}

void eng::PhysicsPrototypeLoader::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const PhysicsPrototype& prototype) const
{
	const auto* asset = m_AssetManager->LoadAsset<eng::PhysicsMaterialAsset>(strDefaultMaterial);

	physx::PxPhysics& physics = m_PhysicsManager.GetPhysics();

	Array<physx::PxShape*> shapes = { };
	if (std::holds_alternative<ShapeBox>(prototype.m_Shape))
	{
		const auto& data = std::get<ShapeBox>(prototype.m_Shape);

		physx::PxBoxGeometry geometry;
		geometry.halfExtents.x = data.m_Extents.x;
		geometry.halfExtents.y = data.m_Extents.y;
		geometry.halfExtents.z = data.m_Extents.z;

		physx::PxFilterData filterData;
		filterData.word0 = data.m_Channel;

		if (physx::PxShape* shape = physics.createShape(geometry, *asset->m_Material))
		{
			shape->setSimulationFilterData(filterData);
			shapes.Append(shape);
		}
	}
	else if (std::holds_alternative<ShapeSphere>(prototype.m_Shape))
	{
		const auto& data = std::get<ShapeSphere>(prototype.m_Shape);

		physx::PxSphereGeometry geometry;
		geometry.radius = data.m_Radius;

		physx::PxFilterData filterData;
		filterData.word0 = data.m_Channel;

		if (physx::PxShape* shape = physics.createShape(geometry, *asset->m_Material))
		{
			shape->setSimulationFilterData(filterData);
			shapes.Append(shape);
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

void eng::PhysicsPrototypeLoader::Load(PhysicsPrototype& prototype, eng::Visitor& visitor) const
{
	// #todo: array of shapes
	// #todo: should become an asset ?
	{
		EShape shape;
		visitor.Visit(strShape, shape, EShape::Box);
		switch (shape)
		{
		case EShape::Box:
		{
			ShapeBox box;
			visitor.Visit(strChannel, box.m_Channel, 0);
			visitor.Visit(strExtents, box.m_Extents, Vector3f::Zero);
			prototype.m_Shape = box;
		} break;

		case EShape::Sphere:
		{
			ShapeSphere sphere;
			visitor.Visit(strChannel, sphere.m_Channel, 0);
			visitor.Visit(strRadius, sphere.m_Radius, 0.f);
			prototype.m_Shape = sphere;
		} break;
		}
	}

	// #todo: should become an asset ?
	{
		ERigidbody rigidbody;
		visitor.Visit(strRigid, rigidbody, ERigidbody::Static);
		switch (rigidbody)
		{
		case ERigidbody::Dynamic:
		{
			RigidDynamic dynamic;
			visitor.Visit(strENABLE_CCD, dynamic.eENABLE_CCD, false);
			visitor.Visit(strKINEMATIC, dynamic.eKINEMATIC, false);
			visitor.Visit(strLOCK_ANGULAR_X, dynamic.eLOCK_ANGULAR_X, false);
			visitor.Visit(strLOCK_ANGULAR_Y, dynamic.eLOCK_ANGULAR_Y, false);
			visitor.Visit(strLOCK_ANGULAR_Z, dynamic.eLOCK_ANGULAR_Z, false);
			visitor.Visit(strLOCK_LINEAR_X, dynamic.eLOCK_LINEAR_X, false);
			visitor.Visit(strLOCK_LINEAR_Y, dynamic.eLOCK_LINEAR_Y, false);
			visitor.Visit(strLOCK_LINEAR_Z, dynamic.eLOCK_LINEAR_Z, false);
			prototype.m_Rigidbody = dynamic;
		} break;

		case ERigidbody::Static:
		{
			prototype.m_Rigidbody = RigidStatic();
		} break;
		}
	}
}