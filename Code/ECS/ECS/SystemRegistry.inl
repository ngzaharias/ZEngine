#pragma once

namespace detail
{
	template<typename TSystem, typename TWorld>
	using HasInitialiseMethod = decltype(std::declval<TSystem&>().Initialise(std::declval<TWorld&>()));

	template<typename TSystem, typename TWorld>
	using HasShutdownMethod = decltype(std::declval<TSystem&>().Shutdown(std::declval<TWorld&>()));

	template<typename TSystem, typename TWorld>
	using HasUpdateMethod = decltype(std::declval<TSystem&>().Update(std::declval<TWorld&>(), std::declval<const GameTime&>()));
}

template<class TSystem>
bool ecs::SystemRegistry::IsRegistered() const
{
	using NonConst = std::remove_const<TSystem>::type;
	const ecs::SystemId systemId = ToTypeIndex<NonConst, ecs::SystemTag>();
	return m_Entries.Contains(systemId);
}

template<class TSystem, typename... TArgs>
void ecs::SystemRegistry::Register(TArgs&&... args)
{
	using NonConst = std::remove_const<TSystem>::type;
	static_assert(std::is_convertible<NonConst*, ecs::System*>::value, "System must inherit from ecs::System using the [public] keyword!");
	static_assert(std::is_base_of<ecs::System, NonConst>::value, "Type doesn't inherit from ecs::System.");
	Z_PANIC(!IsRegistered<NonConst>(), "System has already been registered!");

	const ecs::SystemId systemId = ToTypeIndex<NonConst, ecs::SystemTag>();
	ecs::SystemEntry& entry = m_Entries.Emplace(systemId);
	entry.m_System = new TSystem(std::forward<TArgs>(args)...);
	entry.m_Priority = systemId;
	entry.m_Name = ToTypeName<NonConst>();

	entry.m_Initialise = &InitialiseFunction<NonConst>;
	entry.m_Shutdown = &ShutdownFunction<NonConst>;
	entry.m_Update = &UpdateFunction<NonConst>;
}

template<class TSystem>
void ecs::SystemRegistry::RegisterPriority(const int32 priority)
{
	using NonConst = std::remove_const<TSystem>::type;
	Z_PANIC(IsRegistered<NonConst>(), "System hasn't been registered!");

	const ecs::SystemId systemId = ToTypeIndex<NonConst, ecs::SystemTag>();
	m_Entries.Get(systemId).m_Priority = priority;
}

template<class TSystem>
TSystem& ecs::SystemRegistry::GetSystem()
{
	using NonConst = std::remove_const<TSystem>::type;
	Z_PANIC(IsRegistered<NonConst>(), "System hasn't been registered!");

	const ecs::SystemId systemId = ToTypeIndex<NonConst, ecs::SystemTag>();
	const ecs::SystemEntry& entry = m_Entries.Get(systemId);
	return *static_cast<TSystem*>(entry.m_System);
}

template<typename TSystem>
void ecs::SystemRegistry::InitialiseFunction(ecs::EntityWorld& entityWorld, ecs::System& system)
{
	using WorldView = TSystem::World;
	if constexpr (core::IsDetected<detail::HasInitialiseMethod, TSystem, WorldView>::value)
	{
		WorldView worldView(entityWorld);

		auto& tSystem = static_cast<TSystem&>(system);
		tSystem.Initialise(worldView);
	}
	else
	{
		system.Initialise();
	}
}

template<typename TSystem>
void ecs::SystemRegistry::ShutdownFunction(ecs::EntityWorld& entityWorld, ecs::System& system)
{
	using WorldView = TSystem::World;
	if constexpr (core::IsDetected<detail::HasShutdownMethod, TSystem, WorldView>::value)
	{
		WorldView worldView(entityWorld);

		auto& tSystem = static_cast<TSystem&>(system);
		tSystem.Shutdown(worldView);
	}
	else
	{
		system.Shutdown();
	}
}

template<typename TSystem>
void ecs::SystemRegistry::UpdateFunction(ecs::EntityWorld& entityWorld, ecs::System& system, const GameTime& gameTime)
{
	using WorldView = TSystem::World;
	if constexpr (core::IsDetected<detail::HasUpdateMethod, TSystem, WorldView>::value)
	{
		WorldView worldView(entityWorld);

		auto& tSystem = static_cast<TSystem&>(system);
		tSystem.Update(worldView, gameTime);
	}
	else
	{
		system.Update(gameTime);
	}
}