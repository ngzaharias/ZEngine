
template<typename TType>
bool ecs::EntityWorld::IsRegistered() const
{
	static_assert(!std::is_const<TType>::value, "Type cannot be const.");

	if constexpr (std::derived_from<TType, ecs::Component>)
	{
		return m_EntityStorage.IsRegistered<TType>();
	}
	else if constexpr (std::derived_from<TType, ecs::Event>)
	{
		return m_EventStorage.IsRegistered<TType>();
	}
	else if constexpr (std::derived_from<TType, ecs::Singleton>)
	{
		return m_SingletonStorage.IsRegistered<TType>();
	}
	else if constexpr (std::derived_from<TType, ecs::System>)
	{
		return m_SystemRegistry.IsRegistered<TType>();
	}
	else
	{
		return m_ResourceRegistry.IsRegistered<TType>();
	}
}

template<typename TWorldView>
TWorldView ecs::EntityWorld::WorldView()
{
	return TWorldView(*this);
}

//////////////////////////////////////////////////////////////////////////
// Entity

inline bool ecs::EntityWorld::IsAlive(const ecs::Entity& entity) const
{
	return m_EntityStorage.IsAlive(entity);
}

inline auto ecs::EntityWorld::CreateEntity() -> ecs::Entity
{
	return m_EntityStorage.CreateEntity();
}

inline void ecs::EntityWorld::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityStorage.DestroyEntity(entity);
}

//////////////////////////////////////////////////////////////////////////
// Component

template<typename TComponent>
void ecs::EntityWorld::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(!IsRegistered<TComponent>(), "Component is already registered!");

	m_TypeRegistry.RegisterComponent<TComponent>();
	m_EntityStorage.RegisterComponent<TComponent>();
}

template<typename TComponent, typename... TArgs>
requires ecs::IsSoloOrStaticComponent<TComponent>
auto ecs::EntityWorld::AddComponent(TArgs&&... args) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_EntityStorage.AddComponent<TComponent>(m_EntityStorage.m_Entity, std::forward<TArgs>(args)...);
}

template<typename TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(!entity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_EntityStorage.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
void ecs::EntityWorld::RemoveComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(HasComponent<TComponent>(m_EntityStorage.m_Entity), "Entity doesn't have this component!");

	m_EntityStorage.RemoveComponent<TComponent>(m_EntityStorage.m_Entity);
}

template<typename TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");

	m_EntityStorage.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
bool ecs::EntityWorld::HasComponent(const bool alive /*= true*/) const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<TComponent>(m_EntityStorage.m_Entity, alive);
}

template<typename TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<TComponent>(entity, alive);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
auto ecs::EntityWorld::ReadComponent(const bool alive /*= true*/) -> const TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(HasComponent<TComponent>(m_EntityStorage.m_Entity, alive), "Entity doesn't have this component!");
	return m_EntityStorage.GetComponent<TComponent>(m_EntityStorage.m_Entity, alive);
}

template<typename TComponent>
auto ecs::EntityWorld::ReadComponent(const ecs::Entity& entity, const bool alive /*= true*/) -> const TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(HasComponent<TComponent>(entity, alive), "Entity doesn't have this component!");
	return m_EntityStorage.GetComponent<TComponent>(entity, alive);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
auto ecs::EntityWorld::WriteComponent(const bool alive /*= true*/) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(HasComponent<TComponent>(m_EntityStorage.m_Entity, alive), "Entity doesn't have this component!");

	if (alive)
	{
		ecs::EntityBuffer& buffer = m_EntityStorage.GetEntityBuffer();
		buffer.UpdateComponent<TComponent>(m_EntityStorage.m_Entity);
	}
	return m_EntityStorage.GetComponent<TComponent>(m_EntityStorage.m_Entity, alive);
}

template<typename TComponent>
auto ecs::EntityWorld::WriteComponent(const ecs::Entity& entity, const bool alive /*= true*/) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(HasComponent<TComponent>(entity, alive), "Entity doesn't have this component!");

	if (alive)
	{
		ecs::EntityBuffer& buffer = m_EntityStorage.GetEntityBuffer();
		buffer.UpdateComponent<TComponent>(entity);
	}
	return m_EntityStorage.GetComponent<TComponent>(entity, alive);
}

//////////////////////////////////////////////////////////////////////////
// Event

template<typename TEvent>
void ecs::EntityWorld::RegisterEvent()
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TEvent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TEvent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TEvent, ecs::Event>, "Type doesn't inherit from ecs::Event.");

	Z_PANIC(!IsRegistered<TEvent>(), "Event is already registered!");

	m_TypeRegistry.RegisterEvent<TEvent>();
	m_EventStorage.RegisterEvent<TEvent>();
}

template<typename TEvent, typename... TArgs>
auto ecs::EntityWorld::AddEvent(TArgs&&... args) -> TEvent&
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TEvent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TEvent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TEvent, ecs::Event>, "Type doesn't inherit from ecs::Event.");

	Z_PANIC(IsRegistered<TEvent>(), "Event isn't registered!");

	return m_EventStorage.AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

//////////////////////////////////////////////////////////////////////////
// Resource

template<typename TResource>
void ecs::EntityWorld::RegisterResource(TResource& resource)
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");

	Z_PANIC(!IsRegistered<TResource>(), "Resource is already registered!");
	
	m_TypeRegistry.RegisterResource<TResource>();
	m_ResourceRegistry.Register<TResource>(resource);
}

template<typename TResource>
auto ecs::EntityWorld::ReadResource() -> const TResource&
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TResource>(), "Resource isn't registered!");
	return m_ResourceRegistry.Get<TResource>();
}

template<typename TResource>
auto ecs::EntityWorld::WriteResource() -> TResource&
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");
	
	static_assert(!std::derived_from<TResource, ecs::Component>, "Type cannot be a Component.");
	static_assert(!std::derived_from<TResource, ecs::Event>, "Type cannot be an Event.");
	static_assert(!std::derived_from<TResource, ecs::Singleton>, "Type cannot be a Singleton.");
	static_assert(!std::derived_from<TResource, ecs::System>, "Type cannot be a System.");

	Z_PANIC(IsRegistered<TResource>(), "Resource isn't registered!");
	return m_ResourceRegistry.Get<TResource>();
}

//////////////////////////////////////////////////////////////////////////
// Singleton

template<typename TSingleton, typename... TArgs>
void ecs::EntityWorld::RegisterSingleton(TArgs&&... args)
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TSingleton, ecs::Singleton>, "Type doesn't inherit from ecs::Singleton.");

	Z_PANIC(!IsRegistered<TSingleton>(), "Singleton is already registered!");

	m_TypeRegistry.RegisterSingleton<TSingleton>();
	m_SingletonStorage.RegisterSingleton<TSingleton>(std::forward<TArgs>(args)...);
}

template<typename TSingleton>
auto ecs::EntityWorld::ReadSingleton() -> const TSingleton&
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TSingleton, ecs::Singleton>, "Type doesn't inherit from ecs::Singleton.");

	Z_PANIC(IsRegistered<TSingleton>(), "Singleton isn't registered!");
	return m_SingletonStorage.GetSingleton<TSingleton>();
}

template<typename TSingleton>
auto ecs::EntityWorld::WriteSingleton() -> TSingleton&
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TSingleton, ecs::Singleton>, "Type doesn't inherit from ecs::Singleton.");

	Z_PANIC(IsRegistered<TSingleton>(), "Singleton isn't registered!");

	m_SingletonStorage.UpdateSingleton<TSingleton>();
	return m_SingletonStorage.GetSingleton<TSingleton>();
}

//////////////////////////////////////////////////////////////////////////
// System

template<typename TSystem, typename... TArgs>
void ecs::EntityWorld::RegisterSystem(TArgs&&... args)
{
	static_assert(!std::is_const<TSystem>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSystem>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSystem>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TSystem, ecs::System>, "Type doesn't inherit from ecs::System.");

	Z_PANIC(!IsRegistered<TSystem>(), "System is already registered!");

	m_TypeRegistry.RegisterSystem<TSystem>();
	m_SystemRegistry.RegisterSystem<TSystem>(std::forward<TArgs>(args)...);
}

template<typename TSystem>
auto ecs::EntityWorld::GetSystem() -> TSystem&
{
	static_assert(!std::is_const<TSystem>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSystem>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSystem>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TSystem, ecs::System>, "Type doesn't inherit from ecs::System.");

	Z_PANIC(IsRegistered<TSystem>(), "System isn't registered!");
	return m_SystemRegistry.GetSystem<TSystem>();
}

//////////////////////////////////////////////////////////////////////////
// EntityView

template<typename TComponent>
auto ecs::EntityWorld::GetComponentForView(const ecs::Entity& entity) const -> TComponent*
{
	using NonConst = std::remove_const<TComponent>::type;
	return &m_EntityStorage.GetComponent<NonConst>(entity, true);
}

template<typename...TComponents>
auto ecs::EntityWorld::GetComponentsForView(const ecs::Entity& entity) const -> std::tuple<TComponents*...>
{
	std::tuple<TComponents*...> components;
	((std::get<TComponents*>(components) = GetComponentForView<TComponents>(entity)), ...);
	return components;
}

template<typename TComponent>
auto ecs::EntityWorld::TryComponentForView(const ecs::Entity& entity) const -> TComponent*
{
	using NonConst = std::remove_const<TComponent>::type;
	return m_EntityStorage.TryComponent<NonConst>(entity, true);
}

template<typename...TComponents>
auto ecs::EntityWorld::TryComponentsForView(const ecs::Entity& entity) const -> std::tuple<TComponents*...>
{
	std::tuple<TComponents*...> components;
	((std::get<TComponents*>(components) = TryComponentForView<TComponents>(entity)), ...);
	return components;
}