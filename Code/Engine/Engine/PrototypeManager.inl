#pragma once

template<typename TPrototype, typename TLoader, typename... TArgs>
void eng::PrototypeManager::RegisterPrototype(const str::Name& typeName, TArgs&&... args)
{
	static_assert(std::is_convertible<TLoader*, eng::PrototypeLoader*>::value, "Loader must inherit from eng::PrototypeLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::PrototypeLoader, TLoader>::value, "Loader isn't a base of eng::PrototypeLoader!");

	Z_PANIC(!m_EntryMap.Contains(typeName), "Type is already registered!");

	eng::PrototypeEntry& entry = m_EntryMap[typeName];
	entry.m_Create = &CreateFunction<TPrototype, TLoader>;
	entry.m_Loader = new TLoader(std::forward<TArgs>(args)...);
}

template<typename TPrototype, typename TLoader>
void eng::PrototypeManager::CreateFunction(eng::Visitor& visitor, ecs::EntityWorld& world, const ecs::Entity& entity, const eng::PrototypeLoader& loader)
{
	const TLoader& tLoader = static_cast<const TLoader&>(loader);

	TPrototype prototype;
	tLoader.Load(prototype, visitor);
	tLoader.Add(world, entity, prototype);
}