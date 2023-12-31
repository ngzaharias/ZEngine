#pragma once

#include <Engine/System.h>

#include <vector>
#include <entt/fwd.hpp>
#include <entt/entity/entity.hpp>
#include <entt/signal/sigh.hpp>

namespace physx
{
	class PxMaterial;
	class PxPhysics;
	class PxScene;

	// debug
	class PxPvd;
	class PxPvdTransport;
}

namespace sf
{
	class Time;
}

namespace physics
{
	class PhysicsManager;

	class PhysicsSystem final : public core::System
	{
	public:
		PhysicsSystem(physics::PhysicsManager& physicsManager);
		~PhysicsSystem();

		void Initialize(entt::registry& registry) override;
		void Update(entt::registry& registry, const core::GameTime& gameTime) override;
		void Destroy(entt::registry& registry) override;

	private:
		void OnContact(const entt::entity& entityA, const entt::entity& entityB);
		void OnDestroy_RigidBody(entt::registry& registry, entt::entity entity);

	public:
		entt::sigh<void(const entt::entity&, const entt::entity&)> m_OnCollideSignal;
		entt::sigh<void(const entt::entity&, const entt::entity&)> m_OnOverlapSignal;

	public:
		physics::PhysicsManager& m_PhysicsManager;

		// debug
		physx::PxPvd* m_Debugger = nullptr;
		physx::PxPvdTransport* m_Transport = nullptr;

		float m_DeltaTimeAccumulated = 0.f;
	};
};