#pragma once

#include <ECS/System.h>

#include <entt/signal/sigh.hpp>

#include <PhysX/PxSimulationEventCallback.h>

namespace ecs
{
	struct Entity;
}

namespace physx
{
	class PxActor;
	class PxScene;
	class PxTransform;

	struct PxConstraintInfo;
	struct PxContactPair;
	struct PxContactPairHeader;
	struct PxTriggerPair;
}

namespace eng
{
	class PhysicsManager;

	struct PhysicsSceneComponent;
	struct RigidDynamicComponent;
	struct RigidStaticComponent;
	struct TransformComponent;

	class PhysicsSystem final : public ecs::System, public physx::PxSimulationEventCallback
	{
	public:
		using World = ecs::WorldView<
			eng::PhysicsManager,
			eng::PhysicsSceneComponent,
			eng::RigidDynamicComponent,
			eng::RigidStaticComponent,
			eng::TransformComponent>;

		void Initialise(World& world);
		void Shutdown(World& world);

		void Update(World& world, const GameTime& gameTime);

	private:
		void OnContact(const ecs::Entity& entityA, const ecs::Entity& entityB);

		void ProcessAdded(World& world);
		void ProcessUpdated(World& world);
		void ProcessRemoved(World& world);

	private:
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override { }
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
		void onWake(physx::PxActor** actors, physx::PxU32 count) override;

	public:
		entt::sigh<void(const ecs::Entity&, const ecs::Entity&)> m_OnCollideSignal;
		entt::sigh<void(const ecs::Entity&, const ecs::Entity&)> m_OnOverlapSignal;

	public:
		float m_DeltaTimeAccumulated = 0.f;
	};
};