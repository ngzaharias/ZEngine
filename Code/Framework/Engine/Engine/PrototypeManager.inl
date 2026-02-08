#pragma once

#include "Core/TypeName.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "Engine/Visitor.h"

#include <type_traits>

template<typename TComponent>
void eng::PrototypeManager::RegisterComponent()
{
	static_assert(!std::is_const<TComponent>::value, "Type cannot be const.");
	static_assert(!std::is_reference_v<TComponent>, "Type cannot be a reference.");
	static_assert(!std::is_pointer_v<TComponent>, "Type cannot be a pointer.");
	static_assert(std::is_base_of<ecs::Component<TComponent>, TComponent>::value, "Type doesn't inherit from ecs::Component.");
	static_assert(std::is_base_of<ecs::IsPrototype, TComponent>::value, "Type doesn't inherit from ecs::IsPrototype.");

	const str::Name typeName = str::Name::Create(TypeName<TComponent>::m_WithNamespace);
	Z_PANIC(!m_EntryMap.Contains(typeName), "Type is already registered!");

	eng::PrototypeEntry& entry = m_EntryMap[typeName];
	entry.m_Save = &SaveComponent<TComponent>;
	entry.m_Load = &LoadComponent<TComponent>;
	entry.m_Inspect = &InspectComponent<TComponent>;
}

template<typename TComponent>
void eng::PrototypeManager::SaveComponent(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor)
{
	if (world.HasComponent<TComponent>(entity))
	{
		static const str::StringView key = TypeName<TComponent>::m_WithNamespace;
		const auto& component = world.ReadComponent<TComponent>(entity);
		visitor.Write(key, component);
	}
}

template<typename TComponent>
void eng::PrototypeManager::LoadComponent(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor)
{
	if (!world.IsRegistered<TComponent>())
		return;

	auto& component = world.HasComponent<TComponent>(entity)
		? world.WriteComponent<TComponent>(entity)
		: world.AddComponent<TComponent>(entity);
	visitor.Read(component);
}

template<typename TComponent>
void eng::PrototypeManager::InspectComponent(ecs::EntityWorld& world, const ecs::Entity& entity, imgui::Inspector& inspector)
{
	if (world.HasComponent<TComponent>(entity))
	{
		static const str::String label = str::String(TypeName<TComponent>::m_WithNamespace);
		inspector.Write(label.c_str(), world.WriteComponent<TComponent>(entity));
	}
}