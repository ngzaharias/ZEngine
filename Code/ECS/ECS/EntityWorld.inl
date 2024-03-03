#pragma once

template<class TType>
bool ecs::EntityWorld::IsRegistered() const
{
	using NonConst = std::remove_const<TType>::type;

	constexpr bool isComponent = std::is_base_of<ecs::Component<NonConst>, NonConst>::value;
	constexpr bool isManager = std::is_base_of<ecs::Manager, NonConst>::value;
	constexpr bool isSystem = std::is_base_of<ecs::System, NonConst>::value;

	if constexpr (isComponent)
	{
		return m_EntityStorage.IsRegistered<NonConst>();
	}
	else if constexpr (isManager)
	{
		return m_ManagerRegistry.IsRegistered<NonConst>();
	}
	else if constexpr (isSystem)
	{
		return m_SystemRegistry.IsRegistered<NonConst>();
	}

	static_assert(isComponent || isManager || isSystem, "Type doesn't inherit from ecs::Component, ecs::Manager or ecs::System!");
	return false;
}

template<class TWorldView>
TWorldView ecs::EntityWorld::GetWorldView()
{
	return TWorldView(*this);
}

inline bool ecs::EntityWorld::IsAlive(const ecs::Entity& entity) const
{
	return m_EntityStorage.IsAlive(entity);
}

inline auto ecs::EntityWorld::CreateEntity()->ecs::Entity
{
	return m_FrameBuffer.CreateEntity();
}

inline void ecs::EntityWorld::DestroyEntity(const ecs::Entity& entity)
{
	m_FrameBuffer.DestroyEntity(entity);
}

template<class TComponent>
void ecs::EntityWorld::RegisterComponent()
{
	m_ComponentRegistry.Register<TComponent>();
	m_EntityStorage.RegisterComponent<TComponent>();
	m_FrameBuffer.RegisterComponent<TComponent>();
}

template<class TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use HasSingleton instead.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<TComponent>(entity, alive);
}

template<class TComponent>
bool ecs::EntityWorld::HasSingleton(const bool alive /*= true*/) const
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<TComponent>(m_SingletonEntity, alive);
}

template<class TComponent>
auto ecs::EntityWorld::GetComponent(const ecs::Entity& entity, const bool alive /*= true*/)->TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use GetSingleton instead.");

	Z_PANIC(!alive || IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	Z_PANIC(HasComponent<TComponent>(entity, alive), "Entity doesn't have this component!");

	if constexpr (!std::is_const<TComponent>::value)
		m_FrameBuffer.UpdateComponent<TComponent>(entity);

	return m_EntityStorage.GetComponent<TComponent>(entity, alive);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::EventComponent<NonConst>, NonConst>::value, "Type inherits from ecs::EventComponent, use AddEventComponent instead.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use AddSingleton instead.");
	Z_PANIC(!entity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");

	return m_FrameBuffer.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddEventComponent(TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::EventComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::EventComponent.");

	const ecs::Entity entity = CreateEntity();
	m_EventEntities.Append(entity);

	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	return m_FrameBuffer.AddComponent<TComponent>(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	Z_PANIC(IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(HasComponent<TComponent>(entity), "Entity doesn't have this component!");

	m_FrameBuffer.RemoveComponent<TComponent>(entity);
}

template<class TComponent>
auto ecs::EntityWorld::GetSingleton(const bool alive /*= true*/)->TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(!alive || IsAlive(m_SingletonEntity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	Z_PANIC(HasSingleton<TComponent>(), "Component hasn't been added!");

	if constexpr (!std::is_const<TComponent>::value)
		m_FrameBuffer.UpdateComponent<TComponent>(m_SingletonEntity);

	return m_EntityStorage.GetComponent<TComponent>(m_SingletonEntity, alive);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddSingleton(TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(!m_SingletonEntity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<TComponent>(), "Component isn't registered!");
	Z_PANIC(!HasSingleton<TComponent>(), "Singleton already exists!");
	return m_FrameBuffer.AddComponent<TComponent>(m_SingletonEntity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityWorld::RemoveSingleton()
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(IsAlive(m_SingletonEntity), "Entity isn't alive!");
	Z_PANIC(HasSingleton<TComponent>(), "Entity doesn't have this component!");
	m_FrameBuffer.RemoveComponent<TComponent>(m_SingletonEntity);
}

template<class TManager, typename... TArgs>
void ecs::EntityWorld::RegisterManager(TArgs&&... args)
{
	m_ManagerRegistry.Register<TManager>(std::forward<TArgs>(args)...);
}

template<class TManager>
TManager& ecs::EntityWorld::GetManager()
{
	Z_PANIC(IsRegistered<TManager>(), "Manager isn't registered!");
	return m_ManagerRegistry.GetManager<TManager>();
}

template<class TResource>
bool ecs::EntityWorld::HasResource()
{
	return m_ResourceRegistry.Has<TResource>();
}

template<class TResource>
TResource& ecs::EntityWorld::GetResource()
{
	Z_PANIC(HasResource<TResource>(), "Resource hasn't been added!");
	return m_ResourceRegistry.Get<TResource>();
}

template<class TResource>
void ecs::EntityWorld::AddResource(TResource& resource)
{
	Z_PANIC(!HasResource<TResource>(), "Resource hasn't been added!");
	return m_ResourceRegistry.Add<TResource>(resource);
}

template<class TResource>
void ecs::EntityWorld::RemoveResource()
{
	Z_PANIC(HasResource<TResource>(), "Resource hasn't been added!");
	return m_ResourceRegistry.Remove<TResource>();
}

template<class TSystem, typename... TArgs>
void ecs::EntityWorld::RegisterSystem(TArgs&&... args)
{
	m_SystemRegistry.Register<TSystem>(std::forward<TArgs>(args)...);
}

template<class TSystem>
void ecs::EntityWorld::RegisterSystemPriority(const int32 priority)
{
	m_SystemRegistry.RegisterPriority<TSystem>(priority);
}

template<class TSystem>
TSystem& ecs::EntityWorld::GetSystem()
{
	Z_PANIC(IsRegistered<TSystem>(), "System isn't registered!");
	return m_SystemRegistry.GetSystem<TSystem>();
}