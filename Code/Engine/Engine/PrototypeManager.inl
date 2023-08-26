#pragma once

template<typename TPrototype, typename TLoader, typename... TArgs>
void eng::PrototypeManager::RegisterPrototype(const str::Name& type, TArgs&&... args)
{
	static_assert(std::is_convertible<TPrototype*, eng::Prototype*>::value, "Prototype must inherit from eng::Prototype using the [public] keyword!");
	static_assert(std::is_base_of<eng::Prototype, TPrototype>::value, "Prototype isn't a base of eng::Prototype!");

	static_assert(std::is_convertible<TLoader*, eng::PrototypeLoader*>::value, "Loader must inherit from eng::PrototypeLoader using the [public] keyword!");
	static_assert(std::is_base_of<eng::PrototypeLoader, TLoader>::value, "Loader isn't a base of eng::PrototypeLoader!");

	constexpr TypeId typeId = ToTypeId<TPrototype>();
	Z_ASSERT_CRASH(!core::Contains(m_EntryMap, typeId), "Type is already registered!");

	eng::PrototypeEntry& entry = m_EntryMap[typeId];
	entry.m_Type = type;
	entry.m_Loader = new TLoader(std::forward<TArgs>(args)...);
	entry.m_Loader->m_AssetManager = &m_AssetManager;
	entry.m_Loader->m_PrototypeManager = this;

	entry.m_Add = &AddFunction<TPrototype, TLoader>;
	entry.m_Load = &LoadFunction<TPrototype, TLoader>;
	entry.m_New = &NewFunction<TPrototype>;

	Z_ASSERT_CRASH(!core::Contains(m_TypeMap, type), "Type is already registered!");
	m_TypeMap[type] = typeId;
}

template<typename TPrototype>
eng::Prototype* eng::PrototypeManager::NewFunction()
{
	return new TPrototype();
}

template<typename TPrototype, typename TLoader>
void eng::PrototypeManager::LoadFunction(eng::Prototype* prototype, const eng::PrototypeLoader* loader, eng::Visitor& visitor)
{
	const TLoader* tLoader = dynamic_cast<const TLoader*>(loader);
	TPrototype* tPrototype = dynamic_cast<TPrototype*>(prototype);
	tLoader->Load(*tPrototype, visitor);
}

template<typename TPrototype, typename TLoader>
void eng::PrototypeManager::AddFunction(ecs::EntityWorld& world, const ecs::Entity& entity, const eng::Prototype* prototype, const eng::PrototypeLoader* loader)
{
	const TLoader* tLoader = dynamic_cast<const TLoader*>(loader);
	const TPrototype* tPrototype = dynamic_cast<const TPrototype*>(prototype);
	tLoader->Add(world, entity, *tPrototype);
}