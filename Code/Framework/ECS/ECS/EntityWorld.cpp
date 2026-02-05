#include "ECS/EntityWorld.h"

#include "Core/Profiler.h"
#include "ECS/NameComponent.h"
#include "ECS/System.h"

ecs::EntityWorld::EntityWorld(ecs::TypeRegistry& typeRegistry)
	: m_TypeRegistry(typeRegistry)
	, m_EntityStorage(m_QueryRegistry)
	, m_EventStorage()
	, m_SingletonStorage()
	, m_QueryRegistry()
	, m_ResourceRegistry()
	, m_SystemRegistry()
{
	RegisterComponent<ecs::NameComponent>();
}

void ecs::EntityWorld::Initialise()
{
	PROFILE_FUNCTION();

	m_QueryRegistry.Initialise();
	m_SystemRegistry.Initialise(*this);
	//Z_LOG(ELog::Debug, "{}", LogUpdateOrder());

	// flush the initialise
	m_EntityStorage.FlushChanges();
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

	m_EntityStorage.FlushChanges();
	m_EventStorage.FlushChanges();
	m_SingletonStorage.FlushChanges();
}

auto ecs::EntityWorld::GetComponentMask(const ecs::Entity& entity) -> const ecs::ComponentMask&
{
	return m_EntityStorage.m_AliveEntities.Get(entity);
}

str::String ecs::EntityWorld::LogDependencies() const
{
	const ecs::SystemEntries& entries = m_SystemRegistry.GetEntries();
	const auto& typeMap = m_TypeRegistry.GetTypeMap();

	str::String output;
	output.reserve(static_cast<size_t>(entries.GetCount()) * 256);
	for (auto&& [systemId, entry] : entries)
	{
		const ecs::TypeInfo& typeInfo = typeMap.Get(systemId);
		output += std::format("{}\n", typeInfo.m_Name);
		
		output += std::format("\tWrite:\n");
		for (const TypeId typeId : entry.m_Write)
		{
			const ecs::TypeInfo& typeInfo = typeMap.Get(typeId);
			output += std::format("\t\t{}\n", typeInfo.m_Name);
		}
		
		output += std::format("\tRead:\n");
		for (const TypeId typeId : entry.m_Read)
		{
			const ecs::TypeInfo& typeInfo = typeMap.Get(typeId);
			output += std::format("\t\t{}\n", typeInfo.m_Name);
		}
	}
	return output;
}

str::String ecs::EntityWorld::LogUpdateOrder() const
{
	const ecs::SystemOrder& order = m_SystemRegistry.GetOrder();
	const auto& typeMap = m_TypeRegistry.GetTypeMap();

	str::String output;
	output.reserve(static_cast<size_t>(order.GetCount()) * 256);
	for (const ecs::SystemEntry* entry : order)
	{
		const ecs::TypeInfo& typeInfo = typeMap.Get(entry->m_TypeId);
		output += std::format("{}\n", typeInfo.m_Name);
	}
	return output;
}