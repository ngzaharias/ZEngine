
//////////////////////////////////////////////////////////////////////////
// Component

template <typename TComponent>
void ecs::TypeRegistry::RegisterComponent()
{
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

	ecs::TypeComponent entry;
	entry.m_Add = &AddComponent<TComponent>;
	entry.m_Remove = &RemoveComponent<TComponent>;
	entry.m_Read = &ReadComponent<TComponent>;
	entry.m_Write = &WriteComponent<TComponent>;

	entry.m_Name = TypeName<TComponent>();
	entry.m_TypeId = ToTypeId<TComponent, ecs::ComponentTag>();

	m_ComponentMap.Insert(entry.m_TypeId, entry);
}

template<typename TComponent>
void ecs::TypeRegistry::AddComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
{
	auto& component = world.AddComponent<TComponent>(entity);
	buffer.Read(component);
}

template<typename TComponent>
void ecs::TypeRegistry::RemoveComponent(ecs::EntityWorld& world, const ecs::Entity& entity)
{
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::TypeRegistry::ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, MemBuffer& buffer)
{
	const auto& component = world.ReadComponent<TComponent>(entity);
	buffer.Write(component);
}

template<typename TComponent>
void ecs::TypeRegistry::WriteComponent(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
{
	auto& component = world.WriteComponent<TComponent>(entity);
	buffer.Read(component);
}

//////////////////////////////////////////////////////////////////////////
// Event

template <typename TEvent>
void ecs::TypeRegistry::RegisterEvent()
{
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");

	ecs::TypeEvent entry;
	entry.m_Add = &AddEvent<TEvent>;
	entry.m_Read = &ReadEvent<TEvent>;

	entry.m_Name = TypeName<TEvent>();
	entry.m_TypeId = ToTypeId<TEvent, ecs::EventTag>();

	m_EventMap.Insert(entry.m_TypeId, entry);
}

template<typename TEvent>
void ecs::TypeRegistry::AddEvent(ecs::EntityWorld& world, const MemBuffer& buffer)
{
}

template<typename TEvent>
void ecs::TypeRegistry::ReadEvent(ecs::EntityWorld& world, MemBuffer& buffer)
{
}

//////////////////////////////////////////////////////////////////////////
// Resource

template <typename TResource>
void ecs::TypeRegistry::RegisterResource()
{
	ecs::TypeResource entry;
	entry.m_Name = TypeName<TResource>();
	entry.m_TypeId = ToTypeId<TResource, ecs::ResourceTag>();

	m_ResourceMap.Insert(entry.m_TypeId, entry);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

template <typename TSingleton>
void ecs::TypeRegistry::RegisterSingleton()
{
	ecs::TypeSingleton entry;
	entry.m_Name = TypeName<TSingleton>();
	entry.m_TypeId = ToTypeId<TSingleton, ecs::SingletonTag>();

	m_SingletonMap.Insert(entry.m_TypeId, entry);
}

template<typename TSingleton>
void ecs::TypeRegistry::ReadSingleton(ecs::EntityWorld& world, MemBuffer& buffer)
{
	auto& singleton = world.ReadSingleton<TSingleton>();
	buffer.Write(singleton);
}

template<typename TSingleton>
void ecs::TypeRegistry::WriteSingleton(ecs::EntityWorld& world, const MemBuffer& buffer)
{
	auto& singleton = world.WriteSingleton<TSingleton>();
	buffer.Read(singleton);
}

//////////////////////////////////////////////////////////////////////////
// System

template <typename TSystem>
void ecs::TypeRegistry::RegisterSystem()
{
	ecs::TypeSystem entry;
	entry.m_Name = TypeName<TSystem>();
	entry.m_TypeId = ToTypeId<TSystem, ecs::SystemTag>();

	m_SystemMap.Insert(entry.m_TypeId, entry);
}