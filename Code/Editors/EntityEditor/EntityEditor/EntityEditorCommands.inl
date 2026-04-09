
template<typename TComponent, typename... TArgs>
void editor::entity::ComponentAdd<TComponent, TArgs...>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	std::apply([&](auto&&... args)
		{
			world.AddComponent<TComponent>(entity, std::forward<decltype(args)>(args)...);
		}, m_Args);
}

template<typename TComponent, typename... TArgs>
void editor::entity::ComponentAdd<TComponent, TArgs...>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void editor::entity::ComponentRemove<TComponent>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void editor::entity::ComponentRemove<TComponent>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.AddComponent<TComponent>(entity);
	component = m_Data;
}

template<typename TComponent, typename TValue>
void editor::entity::ComponentUpdate<TComponent, TValue>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.WriteComponent<TComponent>(entity);
	(&component)->*m_FieldPtr = m_ValueNew;
}

template<typename TComponent, typename TValue>
void editor::entity::ComponentUpdate<TComponent, TValue>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.WriteComponent<TComponent>(entity);
	(&component)->*m_FieldPtr = m_ValueOld;
}