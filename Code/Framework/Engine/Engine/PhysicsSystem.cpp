#include "EnginePCH.h"
#include "Engine/PhysicsSystem.h"

#include "Core/GameTime.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/PhysicsComponent.h"
#include "Engine/PhysicsSceneComponent.h"
#include "Engine/PhysicsHelpers.h"
#include "Engine/PhysicsManager.h"
#include "Engine/PhysicsMaterialAsset.h"
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
	const str::Guid strDefaultMaterial = GUID("a4835493ae5a40ba808306deb381c801");

	constexpr float s_Gravity = -981.f;
	constexpr int32 s_SimulationSubsteps = 1;
	constexpr float s_SimulationTimestep = 1.f / 60.f;

	physx::PxRigidActor* CreateActor(physx::PxPhysics& physics, const eng::Rigidbody& data)
	{
		if (std::holds_alternative<eng::RigidDynamic>(data))
		{
			const auto& rigidbody = std::get<eng::RigidDynamic>(data);
			return eng::physics::CreateRigidbody(physics, rigidbody);
		}
		else if (std::holds_alternative<eng::RigidStatic>(data))
		{
			const auto& rigidbody = std::get<eng::RigidStatic>(data);
			return eng::physics::CreateRigidbody(physics, rigidbody);
		}
		return nullptr;
	}

	physx::PxShape* CreateShape(physx::PxPhysics& physics, const eng::Shape& data, const physx::PxMaterial& material)
	{
		if (std::holds_alternative<eng::ShapeBox>(data))
		{
			const auto& shape = std::get<eng::ShapeBox>(data);
			return eng::physics::CreateShape(physics, shape, material);
		}
		else if (std::holds_alternative<eng::ShapeSphere>(data))
		{
			const auto& shape = std::get<eng::ShapeSphere>(data);
			return eng::physics::CreateShape(physics, shape, material);
		}
		return nullptr;
	}
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

	auto& sceneComponent = world.WriteSingleton<eng::PhysicsSceneSingleton>();
	sceneComponent.m_PhysicsScene = physics.createScene(sceneDesc);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strDefaultMaterial);
}

void eng::PhysicsSystem::Shutdown(World& world)
{
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strDefaultMaterial);
}

void eng::PhysicsSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	auto& sceneComponent = world.WriteSingleton<eng::PhysicsSceneSingleton>();

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

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* materialAsset = assetManager.ReadAsset<eng::PhysicsMaterialAsset>(strDefaultMaterial);
	if (!materialAsset)
		return;

	Set<ecs::Entity> entities = {};
	entities.Add(world.Query<ecs::query::Added<eng::PhysicsComponent>>());
	entities.Add(world.Query<ecs::query::Updated<eng::PhysicsComponent>>());
	for (const ecs::Entity& entity : entities)
	{
		auto& physicsManager = world.WriteResource<eng::PhysicsManager>();

		// #hack: dirty hack to not mark the component as updated which causes an infinite loop
		auto& physicsComponent = const_cast<eng::PhysicsComponent&>(world.ReadComponent<eng::PhysicsComponent>(entity));
		auto& sceneComponent = world.WriteSingleton<eng::PhysicsSceneSingleton>();

		physx::PxPhysics& physics = physicsManager.GetPhysics();
		for (physx::PxShape* shape : physicsComponent.m_PxShapes)
		{
			physicsComponent.m_PxRigidActor->detachShape(*shape);
			shape->release();
		}
		physicsComponent.m_PxShapes.RemoveAll();

		if (physicsComponent.m_PxRigidActor)
		{
			physicsComponent.m_PxRigidActor->release();
			physicsComponent.m_PxRigidActor = nullptr;
		}

		physx::PxTransform transform(physx::PxIdentity);
		if (world.HasComponent<eng::TransformComponent>(entity))
		{
			const auto& transformComponent = world.ReadComponent<eng::TransformComponent>(entity);
			transform = eng::physics::ToTransform(transformComponent.m_Translate, transformComponent.m_Rotate);
		}

		physicsComponent.m_PxRigidActor = CreateActor(physics, physicsComponent.m_Rigidbody);
		physicsComponent.m_PxRigidActor->userData = reinterpret_cast<void*>(entity.m_Value);
		physicsComponent.m_PxRigidActor->setGlobalPose(transform);

		for (const eng::Shape& data : physicsComponent.m_Shapes)
		{
			if (physx::PxShape* shape = CreateShape(physics, data, *materialAsset->m_Material))
			{
				physicsComponent.m_PxRigidActor->attachShape(*shape);
				physicsComponent.m_PxShapes.Append(shape);
			}
		}

		sceneComponent.m_PhysicsScene->addActor(*physicsComponent.m_PxRigidActor);
	}
}

void eng::PhysicsSystem::ProcessUpdated(World& world)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Include<eng::TransformComponent, const eng::PhysicsComponent>;
	for (auto&& view : world.Query<Query>())
	{
		const auto& physicsComponent = view.ReadRequired<eng::PhysicsComponent>();
		if (!std::holds_alternative<eng::RigidDynamic>(physicsComponent.m_Rigidbody))
			continue;

		const auto& rigidDynamic = std::get<eng::RigidDynamic>(physicsComponent.m_Rigidbody);
		if (!rigidDynamic.eKINEMATIC)
		{
			auto& transformComponent = view.WriteRequired<eng::TransformComponent>();
			const physx::PxVec3 translate = physicsComponent.m_PxRigidActor->getGlobalPose().p;
			transformComponent.m_Translate = Vector3f(translate.x, translate.y, translate.z);
		}
		else
		{
			const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();
			const physx::PxTransform transform = eng::physics::ToTransform(
				transformComponent.m_Translate, 
				transformComponent.m_Rotate);

			physicsComponent.m_PxRigidActor->setGlobalPose(transform);
		}
	}
}

void eng::PhysicsSystem::ProcessRemoved(World& world)
{
	for (auto&& view : world.Query<ecs::query::Removed<eng::PhysicsComponent>>())
	{
		auto& component = world.WriteComponent<eng::PhysicsComponent>(view, false);
		if (!component.m_PxRigidActor)
			continue;

		for (physx::PxShape* shape : component.m_PxShapes)
		{
			component.m_PxRigidActor->detachShape(*shape);
			shape->release();
		}
		component.m_PxRigidActor->release();
	}
}

void eng::PhysicsSystem::OnContact(const ecs::Entity& entityA, const ecs::Entity& entityB)
{
	m_OnCollideSignal.Publish(entityA, entityB);
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
