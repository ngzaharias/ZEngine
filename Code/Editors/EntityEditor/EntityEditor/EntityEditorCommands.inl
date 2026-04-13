
template<typename TComponent, typename... TArgs>
const char* editor::entity::ComponentAdd<TComponent, TArgs...>::ToString() const
{
	static const str::StringView name = TypeName<TComponent>::m_WithNamespace;
	static const str::String string = std::format("ComponentAdd: {}", name);
	return string.c_str();
}

template<typename TComponent, typename... TArgs>
void editor::entity::ComponentAdd<TComponent, TArgs...>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	std::apply([&](auto&&... args)
		{
			world.AddComponent<TComponent>(entity, std::forward<decltype(args)>(args)...);
		}, m_Args);
}

template<typename TComponent, typename... TArgs>
void editor::entity::ComponentAdd<TComponent, TArgs...>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.RemoveComponent<TComponent>(entity);
}

//////////////////////////////////////////////////////////////////////////

template<typename TComponent>
const char* editor::entity::ComponentRemove<TComponent>::ToString() const
{
	static const str::StringView name = TypeName<TComponent>::m_WithNamespace;
	static const str::String string = std::format("ComponentRemove: {}", name);
	return string.c_str();
}

template<typename TComponent>
void editor::entity::ComponentRemove<TComponent>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void editor::entity::ComponentRemove<TComponent>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	auto& component = world.AddComponent<TComponent>(entity);
	component = m_Data;
}

//////////////////////////////////////////////////////////////////////////

template<typename TComponent, typename TValue>
const char* editor::entity::ComponentField<TComponent, TValue>::ToString() const
{
	static const str::StringView name = TypeName<TComponent>::m_WithNamespace;
	static const str::String string = std::format("ComponentField: {}", name);
	return string.c_str();
}

template<typename TComponent, typename TValue>
void editor::entity::ComponentField<TComponent, TValue>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	auto& component = world.WriteComponent<TComponent>(entity);
	(&component)->*m_FieldPtr = m_ValueNew;
}

template<typename TComponent, typename TValue>
void editor::entity::ComponentField<TComponent, TValue>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	auto& component = world.WriteComponent<TComponent>(entity);
	(&component)->*m_FieldPtr = m_ValueOld;
}

template<typename TComponent, typename TValue>
bool editor::entity::ComponentField<TComponent, TValue>::Merge(Command& command)
{
	if (m_Command != command.m_Command)
		return false;
	if (m_TypeId != command.m_TypeId)
		return false;

	auto& cast = static_cast<ComponentField<TComponent, TValue>&>(command);
	if (m_UUID != cast.m_UUID)
		return false;
	if (m_FieldPtr != cast.m_FieldPtr)
		return false;

	m_ValueNew = cast.m_ValueNew;
	return true;
}

//////////////////////////////////////////////////////////////////////////

template<typename TComponent>
const char* editor::entity::ComponentUpdate<TComponent>::ToString() const
{
	static const str::StringView name = TypeName<TComponent>::m_WithNamespace;
	static const str::String string = std::format("ComponentUpdate: {}", name);
	return string.c_str();
}

template<typename TComponent>
void editor::entity::ComponentUpdate<TComponent>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	auto& component = world.WriteComponent<TComponent>(entity);
	component = m_ValueNew;
}

template<typename TComponent>
void editor::entity::ComponentUpdate<TComponent>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_UUID);
	auto& component = world.WriteComponent<TComponent>(entity);
	component = m_ValueOld;
}

template<typename TComponent>
bool editor::entity::ComponentUpdate<TComponent>::Merge(Command& command)
{
	if (m_Command != command.m_Command)
		return false;
	if (m_TypeId != command.m_TypeId)
		return false;

	auto& cast = static_cast<ComponentUpdate<TComponent>&>(command);
	if (m_UUID != cast.m_UUID)
		return false;

	m_ValueNew = cast.m_ValueNew;
	return true;
}