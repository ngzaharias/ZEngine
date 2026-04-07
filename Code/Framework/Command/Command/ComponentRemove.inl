
template<typename TComponent>
void ComponentRemove<TComponent>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void ComponentRemove<TComponent>::Redo(ecs::EntityWorld& world)
{
	Exec(world);
}

template<typename TComponent>
void ComponentRemove<TComponent>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.AddComponent<TComponent>(entity);
	component = m_Data;
}
