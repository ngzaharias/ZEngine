#pragma once

template<class TType>
bool ecs::EntityWorld::IsRegistered() const
{
	using NonConst = std::remove_const<TType>::type;

	constexpr bool isComponent = std::is_base_of<ecs::Component<NonConst>, NonConst>::value;
	constexpr bool isSystem = std::is_base_of<ecs::System, NonConst>::value;

	if constexpr (isComponent)
	{
		return m_EntityStorage.IsRegistered<NonConst>();
	}
	else if constexpr (isSystem)
	{
		return m_SystemRegistry.IsRegistered<NonConst>();
	}

	static_assert(isComponent || isSystem, "Type doesn't inherit from ecs::Component, ecs::Manager or ecs::System!");
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

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddComponent(const ecs::Entity& entity, TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::EventComponent<NonConst>, NonConst>::value, "Type inherits from ecs::EventComponent, use AddEventComponent instead.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use AddSingleton instead.");
	Z_PANIC(!entity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");

	return m_FrameBuffer.AddComponent<NonConst>(entity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityWorld::RemoveComponent(const ecs::Entity& entity)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	Z_PANIC(IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(HasComponent<NonConst>(entity), "Entity doesn't have this component!");

	m_FrameBuffer.RemoveComponent<NonConst>(entity);
}

template<class TComponent>
bool ecs::EntityWorld::HasComponent(const ecs::Entity& entity, const bool alive /*= true*/) const
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use HasSingleton instead.");

	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<NonConst>(entity, alive);
}

template<class TComponent>
bool ecs::EntityWorld::HasSingleton(const bool alive /*= true*/) const
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	return m_EntityStorage.HasComponent<NonConst>(m_SingletonEntity, alive);
}

template<class TComponent>
auto ecs::EntityWorld::ReadComponent(const ecs::Entity& entity, const bool alive /*= true*/)->const TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use GetSingleton instead.");

	Z_PANIC(alive == IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	Z_PANIC(HasComponent<NonConst>(entity, alive), "Entity doesn't have this component!");

	return m_EntityStorage.GetComponent< NonConst>(entity, alive);
}

template<class TComponent>
auto ecs::EntityWorld::WriteComponent(const ecs::Entity& entity, const bool alive /*= true*/)->TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::Component<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(!std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type inherits from ecs::SingletonComponent, use GetSingleton instead.");

	Z_PANIC(alive == IsAlive(entity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	Z_PANIC(HasComponent<NonConst>(entity, alive), "Entity doesn't have this component!");

	m_FrameBuffer.UpdateComponent<NonConst>(entity);
	return m_EntityStorage.GetComponent<NonConst>(entity, alive);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddEventComponent(TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::EventComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::EventComponent.");

	const ecs::Entity entity = CreateEntity();
	m_EventEntities.Append(entity);

	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	return m_FrameBuffer.AddComponent<NonConst>(entity, std::forward<TArgs>(args)...);
}

template<class TComponent, typename... TArgs>
auto ecs::EntityWorld::AddSingleton(TArgs&&... args)->decltype(auto)
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(!m_SingletonEntity.IsUnassigned(), "Entity is unassigned!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	Z_PANIC(!HasSingleton<NonConst>(), "Singleton already exists!");
	return m_FrameBuffer.AddComponent<NonConst>(m_SingletonEntity, std::forward<TArgs>(args)...);
}

template<class TComponent>
void ecs::EntityWorld::RemoveSingleton()
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(IsAlive(m_SingletonEntity), "Entity isn't alive!");
	Z_PANIC(HasSingleton<NonConst>(), "Entity doesn't have this component!");
	m_FrameBuffer.RemoveComponent<NonConst>(m_SingletonEntity);
}

template<class TComponent>
auto ecs::EntityWorld::ReadSingleton(const bool alive /*= true*/)->const TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(alive == IsAlive(m_SingletonEntity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	Z_PANIC(HasSingleton<NonConst>(), "Component hasn't been added!");

	return m_EntityStorage.GetComponent<NonConst>(m_SingletonEntity, alive);
}

template<class TComponent>
auto ecs::EntityWorld::WriteSingleton(const bool alive /*= true*/)->TComponent&
{
	using NonConst = typename std::remove_const<TComponent>::type;
	static_assert(std::is_base_of<ecs::SingletonComponent<NonConst>, NonConst>::value, "Type doesn't inherit from ecs::SingletonComponent.");

	Z_PANIC(alive == IsAlive(m_SingletonEntity), "Entity isn't alive!");
	Z_PANIC(IsRegistered<NonConst>(), "Component isn't registered!");
	Z_PANIC(HasSingleton<NonConst>(), "Component hasn't been added!");

	m_FrameBuffer.UpdateComponent<NonConst>(m_SingletonEntity);
	return m_EntityStorage.GetComponent<NonConst>(m_SingletonEntity, alive);
}

template<class TResource>
void ecs::EntityWorld::RegisterResource(TResource& resource)
{
	return m_ResourceRegistry.Register<TResource>(resource);
}

template<class TResource>
auto ecs::EntityWorld::ReadResource() -> const TResource&
{
	return m_ResourceRegistry.Get<TResource>();
}

template<class TResource>
auto ecs::EntityWorld::WriteResource() -> TResource&
{
	return m_ResourceRegistry.Get<TResource>();
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
auto ecs::EntityWorld::GetSystem()->TSystem&
{
	return m_SystemRegistry.GetSystem<TSystem>();
}