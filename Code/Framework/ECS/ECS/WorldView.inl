#pragma once

template<typename... TWrite, typename... TRead>
ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::WorldView_t(ecs::EntityWorld& entityWorld)
	: m_EntityWorld(entityWorld)
	, m_QueryRegistry(entityWorld.m_QueryRegistry)
{
}

template<typename... TWrite, typename... TRead>
template<typename... TOthers>
ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::WorldView_t(const WorldView_t<TOthers...>& rhs)
	: m_EntityWorld(rhs.m_EntityWorld)
	, m_QueryRegistry(rhs.m_QueryRegistry)
{
}

//template<typename... TWrite, typename... TRead>
//template<typename... TOthers>
//ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::operator WorldView_t<TOthers...>() const
//{
//	return WorldView_t<TOthers...>(*this);
//}

template<typename... TWrite, typename... TRead>
template<class TEntityView>
TEntityView ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::EntityView(const ecs::Entity& entity)
{
	return TEntityView(m_EntityWorld, entity);
}

template<typename... TWrite, typename... TRead>
bool ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::IsAlive(const ecs::Entity& entity) const
{
	return m_EntityWorld.IsAlive(entity);
}

template<typename... TWrite, typename... TRead>
inline auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::CreateEntity() -> ecs::Entity
{
	return m_EntityWorld.CreateEntity();
}

template<typename... TWrite, typename... TRead>
inline void ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityWorld.DestroyEntity(entity);
}

template<typename... TWrite, typename... TRead>
template<typename TComponent, typename... TArgs>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
{
	static_assert(core::Contains<TComponent, TWrite...>(), "WorldView doesn't have Write access to Component.");

	return m_EntityWorld.template AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<typename... TWrite, typename... TRead>
template<class TComponent>
void ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::RemoveComponent(const ecs::Entity& entity)
{
	static_assert(core::Contains<TComponent, TWrite...>(), "WorldView doesn't have Write access to Component.");

	m_EntityWorld.template RemoveComponent<TComponent>(entity);
}

template<typename... TWrite, typename... TRead>
template<class TComponent>
bool ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	constexpr bool hasWriteAccess = core::Contains<TComponent, TWrite...>();
	constexpr bool hasReadAccess = core::Contains<TComponent, TRead...>();
	static_assert(hasWriteAccess || hasReadAccess, "WorldView doesn't have Write/Read access to Component.");

	return m_EntityWorld.template HasComponent<TComponent>(entity, alive);
}

template<typename... TWrite, typename... TRead>
template<class TComponent>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::ReadComponent(const ecs::Entity& entity, const bool alive /*= true*/)->const TComponent&
{
	constexpr bool hasWriteAccess = core::Contains<TComponent, TWrite...>();
	constexpr bool hasReadAccess = core::Contains<TComponent, TRead...>();
	static_assert(hasWriteAccess || hasReadAccess, "WorldView doesn't have Write/Read access to Component.");

	return m_EntityWorld.template ReadComponent<TComponent>(entity, alive);
}

template<typename... TWrite, typename... TRead>
template<class TComponent>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::WriteComponent(const ecs::Entity& entity, const bool alive /*= true*/)->TComponent&
{
	static_assert(core::Contains<TComponent, TWrite...>(), "WorldView doesn't have Write access to Component.");

	return m_EntityWorld.template WriteComponent<TComponent>(entity, alive);
}

template<typename... TWrite, typename... TRead>
template<typename TEvent, typename... TArgs>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::AddEvent(TArgs&&... args)->decltype(auto)
{
	static_assert(core::Contains<TEvent, TWrite...>(), "WorldView doesn't have Write access to Event.");

	return m_EntityWorld.template AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

template<typename... TWrite, typename... TRead>
template<class TResource>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::ReadResource() -> const TResource&
{
	constexpr bool hasWriteAccess = core::Contains<TResource, TWrite...>();
	constexpr bool hasReadAccess = core::Contains<TResource, TRead...>();
	static_assert(hasWriteAccess || hasReadAccess, "WorldView doesn't have Write/Read access to Resource.");

	return m_EntityWorld.template ReadResource<TResource>();
}

template<typename... TWrite, typename... TRead>
template<class TResource>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::WriteResource() -> TResource&
{
	static_assert(core::Contains<TResource, TWrite...>(), "WorldView doesn't have Write access to Resource.");

	return m_EntityWorld.template WriteResource<TResource>();
}

template<typename... TWrite, typename... TRead>
template<class TSingleton>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::ReadSingleton() -> const TSingleton&
{
	constexpr bool hasWriteAccess = core::Contains<TSingleton, TWrite...>();
	constexpr bool hasReadAccess = core::Contains<TSingleton, TRead...>();
	static_assert(hasWriteAccess || hasReadAccess, "WorldView doesn't have Write/Read access to Singleton.");

	return m_EntityWorld.template ReadSingleton<TSingleton>();
}

template<typename... TWrite, typename... TRead>
template<class TSingleton>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::WriteSingleton() -> TSingleton&
{
	static_assert(core::Contains<TSingleton, TWrite...>(), "WorldView doesn't have Write access to Singleton.");

	return m_EntityWorld.template WriteSingleton<TSingleton>();
}

template<typename... TWrite, typename... TRead>
template<class TType>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::Count() -> int32
{
	if constexpr (std::derived_from<TType, ecs::Event>)
	{
		return m_EntityWorld.m_EntityStorage.GetEvents<TType>().GetCount();
	}
	else if constexpr (std::derived_from<TType, ecs::Singleton>)
	{
		static_assert(false, "Unsupported.");
	}
	else
	{
		static const ecs::QueryId queryId = ecs::QueryProxy<TType>::Id();
		const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
		return queryGroup.GetCount();
	}
}

template<typename... TWrite, typename... TRead>
template<class TType>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::HasAny() -> bool
{
	if constexpr (std::derived_from<TType, ecs::Event>)
	{
		return m_EntityWorld.m_EventStorage.HasEvents<TType>();
	}
	else if constexpr (std::derived_from<TType, ecs::Singleton>)
	{
		return m_EntityWorld.m_SingletonStorage.WasUpdated<TType>();
	}
	else
	{
		static const ecs::QueryId queryId = ecs::QueryProxy<TType>::Id();
		const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
		return !queryGroup.IsEmpty();
	}
}

template<typename... TWrite, typename... TRead>
template<class TEvent>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::Events() -> decltype(auto)
{
	return m_EntityWorld.m_EventStorage.GetEvents<TEvent>();
}

template<typename... TWrite, typename... TRead>
template<class TQuery>
auto ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::Query() -> ecs::QueryRange<TQuery>
{
#ifndef Z_RELEASE
	ValidateAccess<TQuery>();
#endif

	static const ecs::QueryId queryId = ecs::QueryProxy<TQuery>::Id();
	const ecs::QueryGroup& queryGroup = m_QueryRegistry.GetGroup(queryId);
	return ecs::QueryRange<TQuery>{ m_EntityWorld, queryGroup };
}


template<typename... TWrite, typename... TRead>
template<class TQuery>
void ecs::WorldView_t<TypeList<TWrite...>, TypeList<TRead...>>::ValidateAccess()
{
	using TAccessList = TypeList<TWrite..., TRead...>;

	using TIncludeWrite = TypeNonConst<ecs::query::IncludeAccess<TQuery>>::type;
	constexpr bool hasIncludeWrite = core::ContainsAll(TIncludeWrite{}, TWriteList{});
	static_assert(hasIncludeWrite, "WorldView doesn't have Write access to Component in Include Query.");

	using TIncludeRead = TypeConst<ecs::query::IncludeAccess<TQuery>>::type;
	constexpr bool hasIncludeRead = core::ContainsAll(TIncludeRead{}, TAccessList{});
	static_assert(hasIncludeRead, "WorldView doesn't have Read (or Write) access to Component in Include Query.");

	using TOptionalWrite = TypeNonConst<ecs::query::OptionalAccess<TQuery>>::type;
	constexpr bool hasOptionalWrite = core::ContainsAll(TOptionalWrite{}, TWriteList{});
	static_assert(hasOptionalWrite, "WorldView doesn't have Write access to Component in Optional Query.");

	using TOptionalRead = TypeConst<ecs::query::OptionalAccess<TQuery>>::type;
	constexpr bool hasOptionalRead = core::ContainsAll(TOptionalRead{}, TAccessList{});
	static_assert(hasOptionalRead, "WorldView doesn't have Read (or Write) access to Component in Optional Query.");
}