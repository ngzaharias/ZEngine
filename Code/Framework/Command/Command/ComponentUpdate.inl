
template<typename TComponent, typename TValue>
void ComponentUpdate<TComponent, TValue>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.WriteComponent<TComponent>(entity); 
	(&component)->*m_FieldPtr = m_ValueNew;
}

template<typename TComponent, typename TValue>
void ComponentUpdate<TComponent, TValue>::Redo(ecs::EntityWorld& world)
{
	Exec(world);
}

template<typename TComponent, typename TValue>
void ComponentUpdate<TComponent, TValue>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	auto& component = world.WriteComponent<TComponent>(entity);
	(&component)->*m_FieldPtr = m_ValueOld;
}
