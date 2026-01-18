
template<class TEvent>
void ecs::EventStorage::RegisterEvent()
{
	m_BufferLocalCurr.RegisterEvent<TEvent>();
	m_BufferLocalNext.RegisterEvent<TEvent>();
	if constexpr (std::is_base_of<ecs::IsReplicated, TEvent>::value)
		m_BufferRemoteCurr.RegisterEvent<TEvent>();
}

template <typename TEvent, typename... TArgs>
auto ecs::EventStorage::AddEvent(TArgs&&... args) -> TEvent&
{
	return m_BufferLocalNext.AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

template<class TEvent>
bool ecs::EventStorage::HasEvents() const
{
	using Container = ecs::EventContainer<TEvent>;
	const Container& local = m_BufferLocalCurr.GetAt<TEvent>();
	const Container* remote = m_BufferRemoteCurr.TryAt<TEvent>();
	return local.GetCount() > 0 || (remote && remote->GetCount() > 0);
}

template<class TEvent>
auto ecs::EventStorage::GetEvents() const -> decltype(auto)
{
	using Container = const ecs::EventContainer<TEvent>;

	struct Iterator
	{
		auto operator*() -> const TEvent&
		{
			const int32 localIndex = m_Index - m_Stride;
			return localIndex >= 0
				? m_Local.m_Data[localIndex]
				: m_Remote->m_Data[m_Index];
		}

		auto operator++() -> Iterator&
		{
			m_Index++;
			return *this;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return m_Index != rhs.m_Index;
		}

		Container& m_Local;
		Container* m_Remote;

		int32 m_Index = 0;
		int32 m_Offset = 0;
		int32 m_Stride = 0;
	};

	struct Wrapper
	{
		auto begin() -> Iterator
		{
			const int32 stride = m_Remote ? m_Remote->GetCount() : 0;
			return Iterator{
				.m_Local = m_Local, 
				.m_Remote = m_Remote, 
				.m_Stride = stride };
		}

		auto end() -> Iterator
		{
			const int32 local = m_Local.GetCount();
			const int32 remote = m_Remote ? m_Remote->GetCount() : 0;
			return Iterator{ 
				.m_Local = m_Local, 
				.m_Index = local + remote };
		}

		Container& m_Local;
		Container* m_Remote;
	};

	return Wrapper{
		.m_Local = m_BufferLocalCurr.GetAt<TEvent>(),
		.m_Remote = m_BufferRemoteCurr.TryAt<TEvent>() };
}