
template <typename TType>
void ecs::EntityWorld::RegisterType()
{
	const TypeId typeId = ToTypeId<TType>();
	ecs::TypeInfo& info = m_TypeMap[typeId];
	info.m_Name = ToTypeName<TType>();

	if constexpr (std::is_base_of<ecs::Component<TType>, TType>::value)
	{
		info.m_Base = ecs::ETypeBase::Component;
	}
	else if constexpr (std::is_base_of<ecs::Event<TType>, TType>::value)
	{
		info.m_Base = ecs::ETypeBase::Event;
	}
	else if constexpr (std::is_base_of<ecs::Singleton<TType>, TType>::value)
	{
		info.m_Base = ecs::ETypeBase::Singleton;
	}
	else if constexpr (std::is_base_of<ecs::System, TType>::value)
	{
		info.m_Base = ecs::ETypeBase::System;
	}
	else
	{
		info.m_Base = ecs::ETypeBase::Resource;
	}
}

template <typename TType>
bool ecs::EntityWorld::IsRegistered() const
{
	static_assert(!std::is_const<TType>::value, "Type cannot be const.");

	const TypeId typeId = ToTypeId<TType>();
	return m_TypeMap.Contains(typeId);
}

template <typename TWorldView>
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

template <typename TComponent>
void ecs::EntityWorld::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(std::is_convertible<TComponent*, ecs::Component<TComponent>*>::value, "Type must inherit using the [public] keyword!");

	Z_PANIC(!IsRegistered<TComponent>(), "Component is already registered!");

	RegisterType<TComponent>();
	m_EntityStorage.RegisterComponent<TComponent>();

	const ecs::ComponentId componentId = ToTypeId<TComponent, ecs::ComponentTag>();
	const TypeId typeId = ToTypeId<TComponent>();
	m_ComponentRemap[componentId] = typeId;
}

template <typename TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");
	
	Z_PANIC(!entity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_EntityStorage.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");

	m_EntityStorage.RemoveComponent<TComponent>(entity);
}

template <typename TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<TComponent>(entity, alive);
}

template <typename TComponent>
auto ecs::EntityWorld::ReadComponent(const ecs::Entity& entity, const bool alive /*= true*/) -> const TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(HasComponent<TComponent>(entity, alive), "Entity doesn't have this component!");
	return m_EntityStorage.GetComponent<TComponent>(entity, alive);
}

template <typename TComponent>
auto ecs::EntityWorld::WriteComponent(const ecs::Entity& entity, const bool alive /*= true*/) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

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

template <typename TEvent>
void ecs::EntityWorld::RegisterEvent()
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TEvent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TEvent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");
	static_assert(std::is_convertible<TEvent*, ecs::Event<TEvent>*>::value, "Type must inherit using the [public] keyword!");

	Z_PANIC(!IsRegistered<TEvent>(), "Event is already registered!");

	RegisterType<TEvent>();
	m_EventStorage.RegisterEvent<TEvent>();
}

template <typename TEvent, typename... TArgs>
auto ecs::EntityWorld::AddEvent(TArgs&&... args) -> TEvent&
{
	static_assert(!std::is_const<TEvent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TEvent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TEvent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");

	Z_PANIC(IsRegistered<TEvent>(), "Event isn't registered!");

	return m_EventStorage.AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

//////////////////////////////////////////////////////////////////////////
// Resource

template <typename TResource>
void ecs::EntityWorld::RegisterResource(TResource& resource)
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");

	Z_PANIC(!IsRegistered<TResource>(), "Resource is already registered!");
	
	RegisterType<TResource>();
	m_ResourceRegistry.Register<TResource>(resource);
}

template <typename TResource>
auto ecs::EntityWorld::ReadResource() -> const TResource&
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TResource>(), "Resource isn't registered!");
	return m_ResourceRegistry.Get<TResource>();
}

template <typename TResource>
auto ecs::EntityWorld::WriteResource() -> TResource&
{
	static_assert(!std::is_const<TResource>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TResource>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TResource>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TResource>(), "Resource isn't registered!");
	return m_ResourceRegistry.Get<TResource>();
}

//////////////////////////////////////////////////////////////////////////
// Singleton

template <typename TSingleton, typename... TArgs>
void ecs::EntityWorld::RegisterSingleton(TArgs&&... args)
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Singleton<TSingleton>, TSingleton>::value, "Type doesn't inherit from ecs::Singleton.");
	static_assert(std::is_convertible<TSingleton*, ecs::Singleton<TSingleton>*>::value, "Type must inherit using the [public] keyword!");

	Z_PANIC(!IsRegistered<TSingleton>(), "Singleton is already registered!");

	RegisterType<TSingleton>();
	m_SingletonStorage.RegisterSingleton<TSingleton>(std::forward<TArgs>(args)...);
}

template <typename TSingleton>
auto ecs::EntityWorld::ReadSingleton() -> const TSingleton&
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Singleton<TSingleton>, TSingleton>::value, "Type doesn't inherit from ecs::Singleton.");

	Z_PANIC(IsRegistered<TSingleton>(), "Singleton isn't registered!");
	return m_SingletonStorage.GetSingleton<TSingleton>();
}

template <typename TSingleton>
auto ecs::EntityWorld::WriteSingleton() -> TSingleton&
{
	static_assert(!std::is_const<TSingleton>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSingleton>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSingleton>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Singleton<TSingleton>, TSingleton>::value, "Type doesn't inherit from ecs::Singleton.");

	Z_PANIC(IsRegistered<TSingleton>(), "Singleton isn't registered!");

	m_SingletonStorage.UpdateSingleton<TSingleton>();
	return m_SingletonStorage.GetSingleton<TSingleton>();
}

//////////////////////////////////////////////////////////////////////////
// System

template <typename TSystem, typename... TArgs>
void ecs::EntityWorld::RegisterSystem(TArgs&&... args)
{
	static_assert(!std::is_const<TSystem>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSystem>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSystem>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::System, TSystem>::value, "Type doesn't inherit from ecs::System.");
	static_assert(std::is_convertible<TSystem*, ecs::System*>::value, "Type must inherit using the [public] keyword!");

	Z_PANIC(!IsRegistered<TSystem>(), "System is already registered!");

	RegisterType<TSystem>();
	m_SystemRegistry.Register<TSystem>(std::forward<TArgs>(args)...);
}

template <typename TSystem>
auto ecs::EntityWorld::GetSystem() -> TSystem&
{
	static_assert(!std::is_const<TSystem>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TSystem>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TSystem>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TSystem>(), "System isn't registered!");
	return m_SystemRegistry.GetSystem<TSystem>();
}

//////////////////////////////////////////////////////////////////////////
// EntityView

template <typename TComponent>
auto ecs::EntityWorld::GetComponentForView(const ecs::Entity& entity) const -> TComponent*
{
	using NonConst = std::remove_const<TComponent>::type;
	return &m_EntityStorage.GetComponent<NonConst>(entity, true);
}

template <typename...TComponents>
auto ecs::EntityWorld::GetComponentsForView(const ecs::Entity& entity) const -> std::tuple<TComponents*...>
{
	std::tuple<TComponents*...> components;
	((std::get<TComponents*>(components) = GetComponentForView<TComponents>(entity)), ...);
	return components;
}

template <typename TComponent>
auto ecs::EntityWorld::TryComponentForView(const ecs::Entity& entity) const -> TComponent*
{
	using NonConst = std::remove_const<TComponent>::type;
	return m_EntityStorage.TryComponent<NonConst>(entity, true);
}

template <typename...TComponents>
auto ecs::EntityWorld::TryComponentsForView(const ecs::Entity& entity) const -> std::tuple<TComponents*...>
{
	std::tuple<TComponents*...> components;
	((std::get<TComponents*>(components) = TryComponentForView<TComponents>(entity)), ...);
	return components;
}