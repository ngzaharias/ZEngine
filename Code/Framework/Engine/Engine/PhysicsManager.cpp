#include "EnginePCH.h"
#include "Engine/PhysicsManager.h"

#include "Core/Log.h"

#include <PhysX/PxPhysicsAPI.h>

namespace physx
{
	class UserAllocatorCallback final : public physx::PxAllocatorCallback
	{
	public:
		void* allocate(size_t size, const char*, const char*, int) override
		{
			void* ptr = platformAlignedAlloc(size);
			PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
			return ptr;
		}

		void deallocate(void* ptr) override
		{
			platformAlignedFree(ptr);
		}
	};

	class UserErrorCallback final : public physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			Z_LOG(ELog::Assert, "{}({}) : PhysX Error [{}]: {}", file, line, static_cast<int32>(code), message);
		}
	};

	static UserAllocatorCallback gAllocator;
	static UserErrorCallback gError;
	static PxFoundation* gFoundation = nullptr;
}

void eng::PhysicsManager::Initialise()
{
	PROFILE_FUNCTION();

	physx::gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, physx::gAllocator, physx::gError);

	m_Debugger = physx::PxCreatePvd(*physx::gFoundation);
	m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Debugger->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eDEBUG);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *physx::gFoundation, physx::PxTolerancesScale(), false, m_Debugger);
}

void eng::PhysicsManager::Shutdown()
{
	PROFILE_FUNCTION();

	m_Debugger->disconnect();

	m_Physics->release();

	m_Transport->release();
	m_Debugger->release();
	physx::gFoundation->release();
}