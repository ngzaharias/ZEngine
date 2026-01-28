
template<class TEvent>
void ecs::EventStorage::RegisterEvent()
{
	m_MainBuffer.RegisterEvent<TEvent>();
	m_NextBuffer.RegisterEvent<TEvent>();
	if constexpr (std::is_base_of<ecs::IsReplicated, TEvent>::value)
		m_SyncBuffer.RegisterEvent<TEvent>();
}

template <typename TEvent, typename... TArgs>
auto ecs::EventStorage::AddEvent(TArgs&&... args) -> TEvent&
{
	return m_NextBuffer.AddEvent<TEvent>(std::forward<TArgs>(args)...);
}

template<class TEvent>
bool ecs::EventStorage::HasEvents() const
{
	using Container = ecs::EventContainer<TEvent>;
	const Container& main = m_MainBuffer.GetAt<TEvent>();
	const Container* sync = m_SyncBuffer.TryAt<TEvent>();
	return main.GetCount() > 0 || (sync && sync->GetCount() > 0);
}

template<class TEvent>
auto ecs::EventStorage::GetEvents() const -> decltype(auto)
{
	using Container = const ecs::EventContainer<TEvent>;

	struct Iterator
	{
		auto operator*() -> const TEvent&
		{
			const int32 syncIndex = m_Index - m_Stride;
			return syncIndex < 0
				? m_Main.m_Data[m_Index]
				: m_Sync->m_Data[syncIndex];
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

		Container& m_Main;
		Container* m_Sync;

		int32 m_Index = 0;
		int32 m_Offset = 0;
		int32 m_Stride = 0;
	};

	struct Wrapper
	{
		auto begin() -> Iterator
		{
			return Iterator{
				.m_Main = m_Main,
				.m_Sync = m_Sync,
				.m_Stride = m_Main.GetCount() };
		}

		auto end() -> Iterator
		{
			const int32 main = m_Main.GetCount();
			const int32 sync = m_Sync ? m_Sync->GetCount() : 0;
			return Iterator{
				.m_Main = m_Main,
				.m_Index = main + sync };
		}

		Container& m_Main;
		Container* m_Sync;
	};

	return Wrapper{
		.m_Main = m_MainBuffer.GetAt<TEvent>(),
		.m_Sync = m_SyncBuffer.TryAt<TEvent>() };
}