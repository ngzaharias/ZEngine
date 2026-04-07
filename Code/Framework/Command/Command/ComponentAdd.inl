
template<typename TComponent, typename... TArgs>
void ComponentAdd<TComponent, TArgs...>::Exec(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	std::apply([&](auto&&... args)
		{
			world.AddComponent<TComponent>(entity, std::forward<decltype(args)>(args)...);
		}, m_Args);
}

template<typename TComponent, typename... TArgs>
void ComponentAdd<TComponent, TArgs...>::Redo(ecs::EntityWorld& world)
{
	Exec(world);
}

template<typename TComponent, typename... TArgs>
void ComponentAdd<TComponent, TArgs...>::Undo(ecs::EntityWorld& world)
{
	const ecs::Entity entity = eng::ToEntity(world, m_Guid);
	world.RemoveComponent<TComponent>(entity);
}
