
template<typename TType>
bool ecs::EntityWorld::IsRegistered() const
{
	static_assert(!std::is_const<TType>::value, "Type cannot be const.");

	if constexpr (std::derived_from<TType, ecs::Component>)
	{
		return m_EntityBuffer.IsRegistered<TType>();
	}
	else if constexpr (std::derived_from<TType, ecs::Event>)
	{
		return m_EventStorage.IsRegistered<TType>();
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
	return m_EntityStorage2.IsAlive(entity);
}

inline auto ecs::EntityWorld::CreateEntity() -> ecs::Entity
{
	return m_EntityBuffer.CreateEntity();
}

inline void ecs::EntityWorld::DestroyEntity(const ecs::Entity& entity)
{
	m_EntityBuffer.DestroyEntity(entity);
}

//////////////////////////////////////////////////////////////////////////
// Component

template<typename TComponent>
void ecs::EntityWorld::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(!IsRegistered<TComponent>(), "Component is already registered!");

	m_EntityBuffer.RegisterComponent<TComponent>();
	m_TypeRegistry.RegisterComponent<TComponent>();

	if constexpr (std::derived_from<TComponent, ecs::StaticComponent>)
		m_EntityBuffer.AddComponent<TComponent>(m_StaticEntity);
}

template<typename TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");
	static_assert(!ecs::IsSoloComponent<TComponent>, "Type can't inherit from ecs::SoloComponent.");
	static_assert(!ecs::IsStaticComponent<TComponent>, "Type can't inherit from ecs::StaticComponent.");

	Z_PANIC(!entity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_EntityBuffer.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");
	static_assert(!ecs::IsSoloComponent<TComponent>, "Type can't inherit from ecs::SoloComponent.");
	static_assert(!ecs::IsStaticComponent<TComponent>, "Type can't inherit from ecs::StaticComponent.");

	Z_PANIC(IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");

	m_EntityBuffer.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity) const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage2.HasComponent<TComponent>(entity);
}

template<typename TComponent>
auto ecs::EntityWorld::ReadComponent(const ecs::Entity& entity) -> const TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");
	static_assert(!ecs::IsSoloComponent<TComponent>, "Type can't inherit from ecs::SoloComponent.");
	static_assert(!ecs::IsStaticComponent<TComponent>, "Type can't inherit from ecs::StaticComponent.");

	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");
	return m_EntityStorage2.GetComponent<TComponent>(entity);
}

template<typename TComponent>
auto ecs::EntityWorld::WriteComponent(const ecs::Entity& entity) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(ecs::IsComponent<TComponent>, "Type doesn't inherit from ecs::Component.");
	static_assert(!ecs::IsSoloComponent<TComponent>, "Type can't inherit from ecs::SoloComponent.");
	static_assert(!ecs::IsStaticComponent<TComponent>, "Type can't inherit from ecs::StaticComponent.");

	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");

	m_EntityBuffer.UpdateComponent<TComponent>(entity);
	return m_EntityStorage2.GetComponent<TComponent>(entity);
}

//////////////////////////////////////////////////////////////////////////
// Solo/Static Component

template<typename TComponent, typename... TArgs>
requires ecs::IsSoloComponent<TComponent>
auto ecs::EntityWorld::AddComponent(TArgs&&... args) -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_EntityBuffer.AddComponent<TComponent>(m_StaticEntity, std::forward<TArgs>(args)...);
}

template<typename TComponent>
requires ecs::IsSoloComponent<TComponent>
void ecs::EntityWorld::RemoveComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(!std::derived_from<TComponent, ecs::FrameComponent>, "Type cannot be a FrameComponent.");

	Z_PANIC(HasComponent<TComponent>(m_StaticEntity), "Entity doesn't have this component!");

	m_EntityBuffer.RemoveComponent<TComponent>(m_StaticEntity);
}

template<typename TComponent>
requires ecs::IsSoloComponent<TComponent>
bool ecs::EntityWorld::HasComponent() const
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage2.HasComponent<TComponent>(m_StaticEntity);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
auto ecs::EntityWorld::ReadComponent() -> const TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(HasComponent<TComponent>(m_StaticEntity), "Entity doesn't have this component!");
	return m_EntityStorage2.GetComponent<TComponent>(m_StaticEntity);
}

template<typename TComponent>
requires ecs::IsSoloOrStaticComponent<TComponent>
auto ecs::EntityWorld::WriteComponent() -> TComponent&
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");

	Z_PANIC(HasComponent<TComponent>(m_StaticEntity), "Entity doesn't have this component!");

	m_EntityBuffer.UpdateComponent<TComponent>(m_StaticEntity);
	return m_EntityStorage2.GetComponent<TComponent>(m_StaticEntity);
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
	static_assert(!std::derived_from<TResource, ecs::StaticComponent>, "Type cannot be a StaticComponent.");
	static_assert(!std::derived_from<TResource, ecs::System>, "Type cannot be a System.");

	Z_PANIC(IsRegistered<TResource>(), "Resource isn't registered!");
	return m_ResourceRegistry.Get<TResource>();
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
	return &m_EntityStorage2.GetComponent<NonConst>(entity);
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
	return m_EntityStorage2.TryComponent<NonConst>(entity);
}

template<typename...TComponents>
auto ecs::EntityWorld::TryComponentsForView(const ecs::Entity& entity) const -> std::tuple<TComponents*...>
{
	std::tuple<TComponents*...> components;
	((std::get<TComponents*>(components) = TryComponentForView<TComponents>(entity)), ...);
	return components;
}