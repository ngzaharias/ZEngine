
//////////////////////////////////////////////////////////////////////////
// Component

template<typename TComponent>
void ecs::TypeRegistry::RegisterComponent()
{
	static_assert(std::derived_from<TComponent, ecs::Component>, "Type doesn't inherit from ecs::Component.");

	constexpr bool isReplicated = std::derived_from<TComponent, ecs::IsReplicated>;
	constexpr bool isTemplate = std::derived_from<TComponent, ecs::TemplateComponent>;

	const TypeId globalId = ToTypeId<TComponent>();
	const TypeId localId = ToTypeId<TComponent, ecs::ComponentTag>();

	ecs::TypeInfo& info = m_TypeMap[globalId];
	info.m_Name = TypeName<TComponent>::m_WithNamespace;
	info.m_Base = ecs::ETypeBase::Component;
	info.m_LocalId = localId;

	ecs::TypeComponent& entry = m_ComponentMap[localId];
	entry.m_Name = NAME(info.m_Name);
	entry.m_Bytes = sizeof(TComponent);
	entry.m_GlobalId = globalId;
	entry.m_LocalId = localId;

	entry.m_AddedId = ecs::QueryProxy<ecs::query::Added<TComponent>>::Id();
	entry.m_UpdatedId = ecs::QueryProxy<ecs::query::Updated<TComponent>>::Id();
	entry.m_RemovedId = ecs::QueryProxy<ecs::query::Removed<TComponent>>::Id();
	entry.m_IncludeId = ecs::QueryProxy<ecs::query::Include<TComponent>>::Id();

	entry.m_IsReplicated = isReplicated;
	entry.m_IsTemplate = isTemplate;

	entry.m_ConstructComponent = &ecs::ConstructComponent<TComponent>;
	entry.m_CopystructComponent = &ecs::CopystructComponent<TComponent>;
	entry.m_DestructComponent = &ecs::DestructComponent<TComponent>;

	entry.m_HasComponent = &ecs::HasComponent<TComponent>;
	entry.m_AddComponent = &ecs::AddComponent<TComponent>;
	entry.m_RemoveComponent = &ecs::RemoveComponent<TComponent>;
	
	if constexpr (isReplicated)
	{
		entry.m_AddComponentData = &ecs::AddComponent<TComponent>;
		entry.m_ReadComponentData = &ecs::ReadComponent<TComponent>;
		entry.m_WriteComponentData = &ecs::WriteComponent<TComponent>;
	}
}

//////////////////////////////////////////////////////////////////////////
// Event

template<typename TEvent>
void ecs::TypeRegistry::RegisterEvent()
{
	static_assert(std::derived_from<TEvent, ecs::Event>, "Type doesn't inherit from ecs::Event.");

	constexpr bool isReplicated = std::derived_from<TEvent, ecs::IsReplicated>;

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

	if constexpr (isReplicated)
	{
		entry.m_AddEvent = &ecs::AddEvent<TEvent>;
	}
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