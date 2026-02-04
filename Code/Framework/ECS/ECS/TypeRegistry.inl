
//////////////////////////////////////////////////////////////////////////
// Component

template <typename TComponent>
void ecs::TypeRegistry::RegisterComponent()
{
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");

	ecs::TypeComponent entry;
	if constexpr (std::is_base_of<ecs::IsReplicated, TComponent>::value)
	{
		entry.m_Add = &AddComponentMethod<TComponent>;
		entry.m_Update = &UpdateComponentMethod<TComponent>;
		entry.m_Remove = &RemoveComponentMethod<TComponent>;
		entry.m_Read = &ReadComponentMethod<TComponent>;
		entry.m_Write = &WriteComponentMethod<TComponent>;
	}

	entry.m_Name = ToTypeName<TComponent>();
	entry.m_TypeId = ToTypeId<TComponent, ecs::ComponentTag>();

	entry.m_AddedId = ecs::QueryProxy<ecs::query::Added<TComponent>>::Id();
	entry.m_UpdatedId = ecs::QueryProxy<ecs::query::Updated<TComponent>>::Id();
	entry.m_RemovedId = ecs::QueryProxy<ecs::query::Removed<TComponent>>::Id();
	entry.m_IncludeId = ecs::QueryProxy<ecs::query::Include<TComponent>>::Id();
	entry.m_IsReplicated = std::is_base_of<ecs::IsReplicated, TComponent>::value;

	m_ComponentMap.Insert(entry.m_TypeId, entry);
}

template<typename TComponent>
void ecs::TypeRegistry::AddComponentMethod(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	auto& component = buffer.AddComponent<TComponent>(entity);
	data.Read(component);
}

template<typename TComponent>
void ecs::TypeRegistry::UpdateComponentMethod(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	// if the component was already added, we need to fetch it from the buffer
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	if (buffer.HasComponent<TComponent>(entity))
	{
		auto& component = buffer.WriteComponent<TComponent>(entity);
		data.Read(component);
	}
	// otherwise fetch it from the storage
	else
	{
		// only mark for update if it's already in storage
		buffer.UpdateComponent<TComponent>(entity);

		auto& component = storage.GetComponent<TComponent>(entity);
		data.Read(component);
	}
}

template<typename TComponent>
void ecs::TypeRegistry::RemoveComponentMethod(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	buffer.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::TypeRegistry::ReadComponentMethod(ecs::EntityStorage& storage, const ecs::Entity& entity, MemBuffer& data)
{
	const auto& component = storage.GetComponent<TComponent>(entity);
	data.Write(component);
}

template<typename TComponent>
void ecs::TypeRegistry::WriteComponentMethod(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	auto& component = storage.GetComponent<TComponent>(entity);
	data.Read(component);
}

//////////////////////////////////////////////////////////////////////////
// Event

template <typename TEvent>
void ecs::TypeRegistry::RegisterEvent()
{
	static_assert(std::is_base_of<ecs::Event<TEvent>, TEvent>::value, "Type doesn't inherit from ecs::Event.");

	ecs::TypeEvent entry;
	if constexpr (std::is_base_of<ecs::IsReplicated, TEvent>::value)
	{
		entry.m_Add = &AddEventMethod<TEvent>;
	}

	entry.m_Name = ToTypeName<TEvent>();
	entry.m_TypeId = ToTypeId<TEvent, ecs::EventTag>();

	m_EventMap.Insert(entry.m_TypeId, entry);
}

template<typename TEvent>
void ecs::TypeRegistry::AddEventMethod(ecs::EventBuffer& buffer, const MemBuffer& data)
{
	TEvent& event = buffer.AddEvent<TEvent>();
	data.Read(event);
}

//////////////////////////////////////////////////////////////////////////
// Resource

template <typename TResource>
void ecs::TypeRegistry::RegisterResource()
{
	ecs::TypeResource entry;
	entry.m_Name = ToTypeName<TResource>();
	entry.m_TypeId = ToTypeId<TResource, ecs::ResourceTag>();

	m_ResourceMap.Insert(entry.m_TypeId, entry);
}

//////////////////////////////////////////////////////////////////////////
// Singleton

template <typename TSingleton>
void ecs::TypeRegistry::RegisterSingleton()
{
	ecs::TypeSingleton entry;
	entry.m_Name = ToTypeName<TSingleton>();
	entry.m_TypeId = ToTypeId<TSingleton, ecs::SingletonTag>();
	m_SingletonMap.Insert(entry.m_TypeId, entry);
}

//////////////////////////////////////////////////////////////////////////
// System

template <typename TSystem>
void ecs::TypeRegistry::RegisterSystem()
{
	ecs::TypeSystem entry;
	entry.m_Name = ToTypeName<TSystem>();
	entry.m_TypeId = ToTypeId<TSystem, ecs::SystemTag>();

	m_SystemMap.Insert(entry.m_TypeId, entry);
}