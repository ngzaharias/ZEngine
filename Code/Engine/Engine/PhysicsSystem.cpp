#include "EnginePCH.h"
#include "Engine/PhysicsSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PhysicsManager.h"
#include "Engine/RigidDynamicComponent.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/TransformComponent.h"
#include "Math/Quaternion.h"

#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxScene.h>

namespace
{
	constexpr float s_Gravity = -981.f;
	constexpr int32 s_SimulationSubsteps = 1;
	constexpr float s_SimulationTimestep = 1.f / 60.f;
}

namespace physx
{
	PxFilterFlags contactReportFilterShader
	(
		PxFilterObjectAttributes attributes0
		, PxFilterData filterData0
		, PxFilterObjectAttributes attributes1
		, PxFilterData filterData1
		, PxPairFlags& pairFlags
		, const void* constantBlock
		, PxU32 constantBlockSize
	)
	{
		PX_UNUSED(attributes0);
		PX_UNUSED(attributes1);
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);
		PX_UNUSED(constantBlockSize);
		PX_UNUSED(constantBlock);

		// ignore kinematic <-> kinematic
		const bool isKinematic0 = physx::PxFilterObjectIsKinematic(attributes0);
		const bool isKinematic1 = physx::PxFilterObjectIsKinematic(attributes1);
		if (isKinematic0 && isKinematic1)
			return PxFilterFlag::eSUPPRESS;

		// ignore kinematic <-> rigid static
		const bool isRigidStatic0 = physx::PxGetFilterObjectType(attributes0) == PxFilterObjectType::eRIGID_STATIC;
		const bool isRigidStatic1 = physx::PxGetFilterObjectType(attributes1) == PxFilterObjectType::eRIGID_STATIC;
		if ((isKinematic0 && isRigidStatic1) || (isKinematic1 && isRigidStatic0))
			return PxFilterFlag::eSUPPRESS;

		const bool hasCollisionWord0 = filterData0.word0 & filterData1.word0;
		const bool hasCollisionWord1 = filterData0.word1 & filterData1.word1;
		const bool hasCollisionWord2 = filterData0.word2 & filterData1.word2;
		const bool hasCollisionWord3 = filterData0.word3 & filterData1.word3;

		const bool isBlocking = hasCollisionWord0;
		if (!isBlocking)
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
			//return PxFilterFlag::eSUPPRESS;
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// #todo: enable continuous collision detection per entity
		const bool hasCCD0 = true;
		const bool hasCCD1 = true;
		if (hasCCD0 || hasCCD1)
			pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;

		// #todo: enable contact modifying per entity
		const bool hasContactModify0 = false;
		const bool hasContactModify1 = false;
		if (hasContactModify0 || hasContactModify1)
			pairFlags |= PxPairFlag::eMODIFY_CONTACTS;

		// #todo: enable contact notify per entity
		const bool hasContactNotify0 = true;
		const bool hasContactNotify1 = true;
		if (hasContactNotify0 || hasContactNotify1)
		{
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		}

		return PxFilterFlag::eDEFAULT;
	}
}

void eng::PhysicsSystem::Initialise(World& world)
{
	auto& physicsManager = world.WriteResource<eng::PhysicsManager>();
	physx::PxPhysics& physics = physicsManager.GetPhysics();

	physx::PxSceneDesc sceneDesc(physics.getTolerancesScale());
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.filterShader = physx::contactReportFilterShader;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.gravity = physx::PxVec3(0.f, s_Gravity, 0.f);
	sceneDesc.simulationEventCallback = this;

	auto& sceneComponent = world.AddSingleton<eng::PhysicsSceneComponent>();
	sceneComponent.m_PhysicsScene = physics.createScene(sceneDesc);
}

void eng::PhysicsSystem::Shutdown(World& world)
{
}

void eng::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& sceneComponent = world.WriteSingleton<eng::PhysicsSceneComponent>();

	// must be done before the simulation is run
	ProcessAdded(world);

	{
		PROFILE_CUSTOM("eng::PhysicsSystem::Simulate");

		m_DeltaTimeAccumulated += gameTime.m_DeltaTime;
		for (; m_DeltaTimeAccumulated >= s_SimulationTimestep; m_DeltaTimeAccumulated -= s_SimulationTimestep)
		{
			constexpr float dt = s_SimulationTimestep / s_SimulationSubsteps;
			for (int32 i = 0; i < s_SimulationSubsteps; ++i)
			{
				sceneComponent.m_PhysicsScene->simulate(dt);
				sceneComponent.m_PhysicsScene->fetchResults(true);
			}
		}
	}

	ProcessUpdated(world);
	ProcessRemoved(world);
}

void eng::PhysicsSystem::ProcessAdded(World& world)
{
	PROFILE_FUNCTION();

	auto& sceneComponent = world.WriteSingleton<eng::PhysicsSceneComponent>();

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::RigidDynamicComponent>>())
	{
		auto& rigidComponent = world.WriteComponent<eng::RigidDynamicComponent>(entity);
		const auto& transformComponent = world.ReadComponent< eng::TransformComponent>(entity);

		const Quaternion quaternion = Quaternion::FromRotator(transformComponent.m_Rotate);

		physx::PxVec3 translate;
		translate.x = transformComponent.m_Translate.x;
		translate.y = transformComponent.m_Translate.y;
		translate.z = transformComponent.m_Translate.z;

		physx::PxQuat rotate;
		rotate.x = quaternion.x;
		rotate.y = quaternion.y;
		rotate.z = quaternion.z;
		rotate.w = quaternion.w;

		rigidComponent.m_Actor->setGlobalPose({ translate, rotate }, false);
		sceneComponent.m_PhysicsScene->addActor(*rigidComponent.m_Actor);
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<eng::RigidStaticComponent>>())
	{
		auto& rigidComponent = world.WriteComponent<eng::RigidStaticComponent>(entity);
		const auto& transformComponent = world.ReadComponent< eng::TransformComponent>(entity);

		const Quaternion quaternion = Quaternion::FromRotator(transformComponent.m_Rotate);

		physx::PxVec3 translate;
		translate.x = transformComponent.m_Translate.x;
		translate.y = transformComponent.m_Translate.y;
		translate.z = transformComponent.m_Translate.z;

		physx::PxQuat rotate;
		rotate.x = quaternion.x;
		rotate.y = quaternion.y;
		rotate.z = quaternion.z;
		rotate.w = quaternion.w;

		rigidComponent.m_Actor->setGlobalPose({ translate, rotate }, false);
		sceneComponent.m_PhysicsScene->addActor(*rigidComponent.m_Actor);
	}
}

void eng::PhysicsSystem::ProcessUpdated(World& world)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Include<eng::TransformComponent, const eng::RigidDynamicComponent>;
	for (const ecs::Entity& entity : world.Query<Query>())
	{
		const auto& rigidComponent = world.ReadComponent< eng::RigidDynamicComponent>(entity);
		if (!rigidComponent.m_IsKinematic)
		{
			auto& transformComponent = world.WriteComponent<eng::TransformComponent>(entity);
			const physx::PxVec3 translate = rigidComponent.m_Actor->getGlobalPose().p;
			transformComponent.m_Translate = Vector3f(translate.x, translate.y, translate.z);
		}
		else
		{
			const auto& transformComponent = world.WriteComponent<eng::TransformComponent>(entity);
			const Quaternion quaternion = Quaternion::FromRotator(transformComponent.m_Rotate);

			physx::PxVec3 translate;
			translate.x = transformComponent.m_Translate.x;
			translate.y = transformComponent.m_Translate.y;
			translate.z = transformComponent.m_Translate.z;

			physx::PxQuat rotate;
			rotate.x = quaternion.x;
			rotate.y = quaternion.y;
			rotate.z = quaternion.z;
			rotate.w = quaternion.w;

			const physx::PxTransform transform = { translate, rotate };
			rigidComponent.m_Actor->setGlobalPose(transform);
		}
	}
}

void eng::PhysicsSystem::ProcessRemoved(World& world)
{
	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::RigidDynamicComponent>>())
	{
		auto& rigidComponent = world.WriteComponent<eng::RigidDynamicComponent>(entity, false);
		for (auto* shape : rigidComponent.m_Shapes)
		{
			rigidComponent.m_Actor->detachShape(*shape);
			shape->release();
		}

		rigidComponent.m_Actor->release();
	}

	for (const ecs::Entity& entity : world.Query<ecs::query::Removed<eng::RigidStaticComponent>>())
	{
		auto& rigidComponent = world.WriteComponent<eng::RigidStaticComponent>(entity, false);
		for (auto* shape : rigidComponent.m_Shapes)
		{
			rigidComponent.m_Actor->detachShape(*shape);
			shape->release();
		}

		rigidComponent.m_Actor->release();
	}
}

void eng::PhysicsSystem::OnContact(const ecs::Entity& entityA, const ecs::Entity& entityB)
{
	m_OnCollideSignal.publish(entityA, entityB);
}

void eng::PhysicsSystem::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void eng::PhysicsSystem::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	ecs::Entity entity0 = ecs::Entity(size_t(pairHeader.actors[0]->userData));
	ecs::Entity entity1 = ecs::Entity(size_t(pairHeader.actors[1]->userData));

	OnContact(entity0, entity1);
}

void eng::PhysicsSystem::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void eng::PhysicsSystem::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
}

void eng::PhysicsSystem::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}
