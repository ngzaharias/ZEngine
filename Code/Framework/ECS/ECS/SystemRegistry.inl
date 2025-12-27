#pragma once

template<class TSystem>
bool ecs::SystemRegistry::IsRegistered() const
{
	const TypeId typeId = ToTypeId<TSystem>();
	return m_Entries.Contains(typeId);
}

template<class TSystem, typename... TArgs>
void ecs::SystemRegistry::Register(TArgs&&... args)
{
	const TypeId systemId = ToTypeId<TSystem>();
	
	ecs::SystemEntry& entry = m_Entries.Emplace(systemId);
	entry.m_System = new TSystem(std::forward<TArgs>(args)...);
	entry.m_Initialise = &InitialiseFunction<TSystem>;
	entry.m_Shutdown = &ShutdownFunction<TSystem>;
	entry.m_Update = &UpdateFunction<TSystem>;
	entry.m_TypeId = systemId;

	ToTypeId(entry.m_Read, typename TSystem::World::TReadList{});
	ToTypeId(entry.m_Write, typename TSystem::World::TWriteList{});
	
	for (const TypeId typeId : entry.m_Read)
		m_Reads[typeId].Add(systemId);
	for (const TypeId typeId : entry.m_Write)
		m_Writes[typeId].Add(systemId);
}

template<class TSystem>
TSystem& ecs::SystemRegistry::GetSystem()
{
	const TypeId typeId = ToTypeId<TSystem>();
	const ecs::SystemEntry& entry = m_Entries.Get(typeId);
	return *static_cast<TSystem*>(entry.m_System);
}

template<typename TSystem>
void ecs::SystemRegistry::InitialiseFunction(ecs::EntityWorld& entityWorld, ecs::System& system)
{
	using WorldView = TSystem::World;
	WorldView worldView(entityWorld);
	
	auto& tSystem = static_cast<TSystem&>(system);
	if constexpr (requires { tSystem.Initialise(worldView); })
	{
		tSystem.Initialise(worldView);
	}
	else
	{
		tSystem.Initialise();
	}
}

template<typename TSystem>
void ecs::SystemRegistry::ShutdownFunction(ecs::EntityWorld& entityWorld, ecs::System& system)
{
	using WorldView = TSystem::World;
	WorldView worldView(entityWorld);
	
	auto& tSystem = static_cast<TSystem&>(system);
	if constexpr (requires { tSystem.Shutdown(worldView); })
	{
		tSystem.Shutdown(worldView);
	}
	else
	{
		tSystem.Shutdown();
	}
}

template<typename TSystem>
void ecs::SystemRegistry::UpdateFunction(ecs::EntityWorld& entityWorld, ecs::System& system, const GameTime& gameTime)
{
	using WorldView = TSystem::World;
	WorldView worldView(entityWorld);
	
	auto& tSystem = static_cast<TSystem&>(system);
	if constexpr (requires{ tSystem.Update(worldView, gameTime); })
	{
		tSystem.Update(worldView, gameTime);
	}
	else
	{
		tSystem.Update(gameTime);
	}
}