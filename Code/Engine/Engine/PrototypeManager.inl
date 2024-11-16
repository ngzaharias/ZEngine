#pragma once

#include "Core/TypeName.h"
#include "ECS/Component.h"
#include "ECS/EntityWorld.h"
#include "Engine/Visitor.h"

#include <type_traits>

template<typename TPrototype>
void eng::PrototypeManager::Register()
{
	const str::Name typeName = str::Name::Create(TypeName<TPrototype>::m_WithNamespace);
	Z_PANIC(!m_EntryMap.Contains(typeName), "Type is already registered!");

	eng::PrototypeEntry& entry = m_EntryMap[typeName];
	entry.m_Save = &SaveFunction<TPrototype>;
	entry.m_Load = &LoadFunction<TPrototype>;
}

template<typename TPrototype>
void eng::PrototypeManager::SaveFunction(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor)
{
	using NonConst = std::remove_const<TPrototype>::type;
	if constexpr (std::is_base_of<ecs::Component<NonConst>, NonConst>::value)
	{
		if (world.HasComponent<NonConst>(entity))
		{
			const auto& component = world.ReadComponent<NonConst>(entity);
			visitor.Write(component);
		}
	}
	else
	{
		static_assert(false, "Unsupported Type!");
	}
}

template<typename TPrototype>
void eng::PrototypeManager::LoadFunction(ecs::EntityWorld& world, const ecs::Entity& entity, eng::Visitor& visitor)
{
	using NonConst = std::remove_const<TPrototype>::type;
	if constexpr (std::is_base_of<ecs::Component<NonConst>, NonConst>::value)
	{
		if (world.IsRegistered<NonConst>())
		{
			auto& component = world.HasComponent<NonConst>(entity)
				? world.WriteComponent<NonConst>(entity)
				: world.AddComponent<NonConst>(entity);
			visitor.Read(component);
		}
	}
	else
	{
		static_assert(false, "Unsupported Type!");
	}
}