#pragma once

namespace physx
{
	class PxPhysics;
	class PxPvd;
	class PxPvdTransport;
}

namespace eng
{
	class PhysicsManager final
	{
	public:
		void Initialise();
		void Shutdown();

		physx::PxPhysics& GetPhysics() const { return *m_Physics; }

	public:
		physx::PxPhysics* m_Physics = nullptr;

		// debug
		physx::PxPvd* m_Debugger = nullptr;
		physx::PxPvdTransport* m_Transport = nullptr;
	};
}