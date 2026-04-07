
template<typename TComponent, typename... TArgs>
void editor::entity::Commands::AddComponent(const str::Guid& guid, TArgs&&... args)
{
	m_ExecStack.Append(new ComponentAdd<TComponent, TArgs...>(guid, args...));
}

template<typename TComponent>
void editor::entity::Commands::RemoveComponent(const str::Guid& guid)
{
	m_ExecStack.Append(new ComponentRemove<TComponent>(guid, {}));
}

template <typename TComponent, typename TValue>
void editor::entity::Commands::UpdateComponent(
	TValue TComponent::* fieldPtr,
	const str::Guid& entity,
	const TValue& valueOld, 
	const TValue& valueNew)
{
	m_ExecStack.Append(new ComponentUpdate<TComponent, TValue>(fieldPtr, entity, valueOld, valueNew));
}