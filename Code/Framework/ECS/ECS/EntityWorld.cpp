#include "ECS/EntityWorld.h"

#include "Core/Profiler.h"
#include "ECS/NameComponent.h"
#include "ECS/System.h"

ecs::EntityWorld::EntityWorld()
	: m_FrameBuffer()
	, m_EntityStorage()
	, m_QueryRegistry()
	, m_SystemRegistry()
	, m_TypeMap()
{
	RegisterComponent<ecs::NameComponent>();
}

void ecs::EntityWorld::Initialise()
{
	PROFILE_FUNCTION();

	m_QueryRegistry.Initialise();
	m_SystemRegistry.Initialise(*this);

	// flush the initialise
	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}

void ecs::EntityWorld::Shutdown()
{
	PROFILE_FUNCTION();

	m_SystemRegistry.Shutdown(*this);
}

void ecs::EntityWorld::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	m_SystemRegistry.Update(*this, gameTime);

	m_EntityStorage.FlushChanges(m_FrameBuffer, m_QueryRegistry);
}

str::String ecs::EntityWorld::LogDependencies() const
{
	const ecs::SystemEntries& entries = m_SystemRegistry.GetEntries();

	str::String output;
	output.reserve(entries.GetCount() * 256);
	for (auto&& [systemId, entry] : entries)
	{
		const ecs::TypeInfo& systemInfo = m_TypeMap.Get(systemId);

		output += std::format("\n\n{}", systemInfo.m_Name);
		
		output += std::format("\n\tWrite:");
		for (const TypeId typeId : entry.m_DependencyWrite)
		{
			const ecs::TypeInfo& typeInfo = m_TypeMap.Get(typeId);
			output += std::format("\n\t\t{}", typeInfo.m_Name);
		}
		
		output += std::format("\n\tRead:");
		for (const TypeId typeId : entry.m_DependencyRead)
		{
			const ecs::TypeInfo& typeInfo = m_TypeMap.Get(typeId);
			output += std::format("\n\t\t{}", typeInfo.m_Name);
		}
	}
	return output;
}