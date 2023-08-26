#pragma once

#include <ECS/Manager.h>

namespace physx
{
	class PxPhysics;
	class PxPvd;
	class PxPvdTransport;
}

namespace eng
{
	class PhysicsManager final : public ecs::Manager
	{
	public:
		void Initialise() override;
		void Shutdown() override;

		physx::PxPhysics& GetPhysics() const { return *m_Physics; }

	public:
		physx::PxPhysics* m_Physics = nullptr;

		// debug
		physx::PxPvd* m_Debugger = nullptr;
		physx::PxPvdTransport* m_Transport = nullptr;
	};
}