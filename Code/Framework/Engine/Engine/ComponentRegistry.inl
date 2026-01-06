
template<typename TComponent>
void net::ComponentRegistry::Register()
{
	const TypeId localId = ToTypeId<TComponent, ecs::ComponentTag>();
	const TypeId worldId = ToTypeId<TComponent>();
	m_LocalToWorld[localId] = worldId;
	m_WorldToLocal[worldId] = localId;

	net::ComponentEntry& entry = m_Entries[localId];
	entry.m_ComponentId = localId;
	entry.m_AddedId = ecs::QueryProxy<ecs::query::Added<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_RemovedId = ecs::QueryProxy<ecs::query::Removed<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_UpdatedId = ecs::QueryProxy<ecs::query::Updated<TComponent>::Include<net::ReplicationComponent>>::Id();
	entry.m_Add = &Add<TComponent>;
	entry.m_Remove = &Remove<TComponent>;
	entry.m_Read = &Read<TComponent>;
	entry.m_Write = &Write<TComponent>;
}

template<typename TComponent>
void net::ComponentRegistry::Add(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
{
	world.AddComponent<TComponent>(entity);
}

template<typename TComponent>
void net::ComponentRegistry::Remove(ecs::EntityWorld& world, const ecs::Entity& entity)
{
	world.RemoveComponent<TComponent>(entity);
}

template<typename TComponent>
void net::ComponentRegistry::Read(ecs::EntityWorld& world, const ecs::Entity& entity, MemBuffer& buffer)
{
	world.ReadComponent<TComponent>(entity);
}

template<typename TComponent>
void net::ComponentRegistry::Write(ecs::EntityWorld& world, const ecs::Entity& entity, const MemBuffer& buffer)
{
	world.WriteComponent<TComponent>(entity);
}