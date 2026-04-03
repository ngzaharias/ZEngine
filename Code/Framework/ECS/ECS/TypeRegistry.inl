
//////////////////////////////////////////////////////////////////////////
// Component

template<typename TComponent>
void ecs::TypeRegistry::RegisterComponent()
{
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	constexpr bool isPrototype = std::derived_from<TComponent, ecs::IsPrototype>;
	constexpr bool isReplicated = std::derived_from<TComponent, ecs::IsReplicated>;
	constexpr bool isTemplate = std::derived_from<TComponent, ecs::TemplateComponent>;

	const TypeId globalId = ToTypeId<TComponent>();
	const TypeId localId = ToTypeId<TComponent, ecs::ComponentTag>();

	ecs::TypeInfo& info = m_TypeMap[globalId];
	info.m_Name = TypeName<TComponent>::m_WithNamespace;
	info.m_Base = ecs::ETypeBase::Component;
	info.m_LocalId = localId;

	ecs::TypeComponent& entry = m_ComponentMap[localId];
	entry.m_GlobalId = globalId;
	entry.m_LocalId = localId;

	entry.m_AddedId = ecs::QueryProxy<ecs::query::Added<TComponent>>::Id();
	entry.m_UpdatedId = ecs::QueryProxy<ecs::query::Updated<TComponent>>::Id();
	entry.m_RemovedId = ecs::QueryProxy<ecs::query::Removed<TComponent>>::Id();
	entry.m_IncludeId = ecs::QueryProxy<ecs::query::Include<TComponent>>::Id();

	entry.m_IsPrototype = isPrototype;
	entry.m_IsReplicated = isReplicated;
	entry.m_IsTemplate = isTemplate;

	entry.m_HasSolo = &HasComponentSolo<TComponent>;
	entry.m_AddSolo = &AddComponentSolo<TComponent>;
	entry.m_UpdateSolo = &UpdateComponentSolo<TComponent>;
	entry.m_RemoveSolo = &RemoveComponentSolo<TComponent>;

	if constexpr (isReplicated)
	{
		entry.m_AddData = &AddComponentData<TComponent>;
		entry.m_UpdateData = &UpdateComponentData<TComponent>;
		entry.m_ReadData = &ReadComponentData<TComponent>;
		entry.m_WriteData = &WriteComponentData<TComponent>;
	}
}

template<typename TComponent>
bool ecs::TypeRegistry::HasComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	return storage.HasComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::TypeRegistry::AddComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	auto& component = buffer.AddComponent<TComponent>(entity);
	data.Read(component);
}

template<typename TComponent>
void ecs::TypeRegistry::AddComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	buffer.AddComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::TypeRegistry::UpdateComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
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
void ecs::TypeRegistry::UpdateComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	// only mark for update if it's already in storage
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	if (!buffer.HasComponent<TComponent>(entity))
	{
		buffer.UpdateComponent<TComponent>(entity);
	}
}

template<typename TComponent>
void ecs::TypeRegistry::RemoveComponentSolo(ecs::EntityStorage& storage, const ecs::Entity& entity)
{
	ecs::EntityBuffer& buffer = storage.GetEntityBuffer();
	buffer.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void ecs::TypeRegistry::ReadComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, MemBuffer& data)
{
	const auto& component = storage.GetComponent<TComponent>(entity);
	data.Write(component);
}

template<typename TComponent>
void ecs::TypeRegistry::WriteComponentData(ecs::EntityStorage& storage, const ecs::Entity& entity, const MemBuffer& data)
{
	auto& component = storage.GetComponent<TComponent>(entity);
	data.Read(component);
}

//////////////////////////////////////////////////////////////////////////
// Event

template<typename TEvent>
void ecs::TypeRegistry::RegisterEvent()
{
	static_assert(std::derived_from<TEvent, ecs::Event>, "Type doesn't inherit from ecs::Event.");

	const TypeId globalId = ToTypeId<TEvent>();
	const TypeId localId = ToTypeId<TEvent, ecs::EventTag>();

	ecs::TypeInfo& info = m_TypeMap[globalId];
	info.m_Name = ToTypeName<TEvent>();
	info.m_Base = ecs::ETypeBase::Event;
	info.m_LocalId = localId;

	ecs::TypeEvent& entry = m_EventMap[localId];
	entry.m_GlobalId = globalId;
	entry.m_LocalId = localId;
	entry.m_IsReplicated = std::derived_from<TEvent, ecs::IsReplicated>;

	if constexpr (std::derived_from<TEvent, ecs::IsReplicated>)
	{
		entry.m_Add = &AddEventMethod<TEvent>;
	}
}

template<typename TEvent>
void ecs::TypeRegistry::AddEventMethod(ecs::EventBuffer& buffer, const MemBuffer& data)
{
	TEvent& event = buffer.AddEvent<TEvent>();
	data.Read(event);
}

//////////////////////////////////////////////////////////////////////////
// Resource

template<typename TResource>
void ecs::TypeRegistry::RegisterResource()
{
	const TypeId globalId = ToTypeId<TResource>();
	const TypeId localId = ToTypeId<TResource, ecs::ResourceTag>();

	ecs::TypeInfo& info = m_TypeMap[globalId];
	info.m_Name = ToTypeName<TResource>();
	info.m_Base = ecs::ETypeBase::Resource;
	info.m_LocalId = localId;

	ecs::TypeResource& entry = m_ResourceMap[localId];
	entry.m_GlobalId = globalId;
	entry.m_LocalId = localId;
}

//////////////////////////////////////////////////////////////////////////
// System

template<typename TSystem>
void ecs::TypeRegistry::RegisterSystem()
{
	const TypeId globalId = ToTypeId<TSystem>();
	const TypeId localId = ToTypeId<TSystem, ecs::SystemTag>();

	ecs::TypeInfo& info = m_TypeMap[globalId];
	info.m_Name = ToTypeName<TSystem>();
	info.m_Base = ecs::ETypeBase::System;
	info.m_LocalId = localId;

	ecs::TypeSystem& entry = m_SystemMap[localId];
	entry.m_GlobalId = globalId;
	entry.m_LocalId = localId;
}