
template<typename TComponent, typename... TArgs>
void editor::entity::CommandManager::ComponentAdd(const str::Guid& guid, TArgs&&... args)
{
	m_ExecStack.Append(new editor::entity::ComponentAdd<TComponent, TArgs...>(guid, args...));
}

template<typename TComponent>
void editor::entity::CommandManager::ComponentRemove(const str::Guid& guid)
{
	m_ExecStack.Append(new editor::entity::ComponentRemove<TComponent>(guid, {}));
}

template <typename TComponent, typename TValue>
void editor::entity::CommandManager::ComponentField(
	TValue TComponent::* fieldPtr,
	const str::Guid& entity,
	const TValue& valueOld,
	const TValue& valueNew)
{
	m_ExecStack.Append(new editor::entity::ComponentField<TComponent, TValue>(fieldPtr, entity, valueOld, valueNew));
}

template <typename TComponent>
void editor::entity::CommandManager::ComponentUpdate(
	const str::Guid& entity,
	const TComponent& valueOld,
	const TComponent& valueNew)
{
	m_ExecStack.Append(new editor::entity::ComponentUpdate<TComponent>(entity, valueOld, valueNew));
}