
template<class TEvent>
void ecs::EventStorage::RegisterEvent()
{
	m_Buffer.RegisterEvent<TEvent>();
}

template<class TEvent>
auto ecs::EventStorage::GetEvents() const -> const Array<TEvent>&
{
	const ecs::EventContainer<TEvent>* container = m_Buffer.GetAt<TEvent>();
	return container->GetValues();
}