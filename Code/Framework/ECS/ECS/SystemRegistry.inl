#pragma once

template<class TSystem>
bool ecs::SystemRegistry::IsRegistered() const
{
	const ecs::SystemId systemId = ToTypeId<TSystem, ecs::SystemTag>();
	return m_Entries.Contains(systemId);
}

template<class TSystem, typename... TArgs>
void ecs::SystemRegistry::Register(TArgs&&... args)
{
	const ecs::SystemId systemId = ToTypeId<TSystem, ecs::SystemTag>();
	ecs::SystemEntry& entry = m_Entries.Emplace(systemId);
	entry.m_System = new TSystem(std::forward<TArgs>(args)...);
	entry.m_Priority = systemId;
	entry.m_Name = ToTypeName<TSystem>();

	ToTypeId(entry.m_DependencyRead, typename TSystem::World::TReadList{});
	ToTypeId(entry.m_DependencyWrite, typename TSystem::World::TWriteList{});
	for (const TypeId typeId : entry.m_DependencyRead)
		m_Edges[typeId].Add(systemId);

	entry.m_Initialise = &InitialiseFunction<TSystem>;
	entry.m_Shutdown = &ShutdownFunction<TSystem>;
	entry.m_Update = &UpdateFunction<TSystem>;
}

template<class TSystem>
void ecs::SystemRegistry::RegisterPriority(const int32 priority)
{
	const ecs::SystemId systemId = ToTypeId<TSystem, ecs::SystemTag>();
	m_Entries.Get(systemId).m_Priority = priority;
}

template<class TSystem>
TSystem& ecs::SystemRegistry::GetSystem()
{
	const ecs::SystemId systemId = ToTypeId<TSystem, ecs::SystemTag>();
	const ecs::SystemEntry& entry = m_Entries.Get(systemId);
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