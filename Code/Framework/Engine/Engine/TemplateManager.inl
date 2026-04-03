#pragma once

template<typename TComponent>
void eng::TemplateManager::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::derived_from<TComponent, ecs::TemplateComponent>, "Type doesn't inherit from ecs::TemplateComponent.");

	const str::Name typeName = NAME(TypeName<TComponent>::m_WithNamespace);
	Z_PANIC(!m_EntryMap.Contains(typeName), "Type is already registered!");

	eng::TemplateEntry& entry = m_EntryMap[typeName];
	entry.m_Load = &LoadComponent<TComponent>;
	entry.m_Read = &ReadComponent<TComponent>;
	entry.m_Sync = &SyncComponent<TComponent>;
	entry.m_Inspect = &InspectComponent<TComponent>;
}

template<typename TComponent>
void eng::TemplateManager::LoadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor)
{
	auto& component = world.HasComponent<TComponent>(entity)
		? world.WriteComponent<TComponent>(entity)
		: world.AddComponent<TComponent>(entity);
	visitor.Read(component);
}

template<typename TComponent>
void eng::TemplateManager::ReadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor)
{
	static const str::Name typeName = NAME(TypeName<TComponent>::m_WithNamespace);
	if (world.HasComponent<TComponent>(entity))
	{
		const auto& component = world.ReadComponent<TComponent>(entity);
		visitor.Write(typeName, component);
	}
}

template<typename TComponent>
void eng::TemplateManager::SyncComponent(ecs::EntityWorld& world, const ecs::Entity& entity, Visitor& visitor)
{
	static const str::Name typeName = NAME(TypeName<TComponent>::m_WithNamespace);
	if (visitor.Has(typeName))
	{
		auto& component = world.HasComponent<TComponent>(entity)
			? world.WriteComponent<TComponent>(entity)
			: world.AddComponent<TComponent>(entity);
		visitor.Read(component);
	}
	else if (world.HasComponent<TComponent>(entity))
	{
		world.RemoveComponent<TComponent>(entity);
	}
}

template<typename TComponent>
bool eng::TemplateManager::InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& inspector)
{
	if (world.HasComponent<TComponent>(entity))
	{
		static const str::String label = str::String(TypeName<TComponent>::m_WithNamespace);
		return inspector.Write(label.c_str(), world.WriteComponent<TComponent>(entity));
	}
	return false;
}