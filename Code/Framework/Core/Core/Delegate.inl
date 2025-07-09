#include "Core/Algorithms.h"

// https://simoncoenen.com/blog/programming/CPP_Delegates

template<typename... Args>
void Delegate<void(Args...)>::Publish(Args... args) const
{
	for (auto& connection : m_Connections)
		connection.m_Function(args...);
}

template<typename... Args>
auto Delegate<void(Args...)>::Connect(std::function<void(Args ...)> function)->DelegateHandle<void(Args...)>
{
	const DelegateHandleId handleId = m_HandleIdNext++;
	m_Connections.Emplace(handleId, function);
	return DelegateHandle<void(Args...)>(handleId, *this);
}

template<typename... Args>
template<typename Object>
auto Delegate<void(Args...)>::Connect(Object& object, void (Object::* method)(Args...))->DelegateHandle<void(Args...)>
{
	return Connect([&object, method](Args... args)
		{
			(object.*method)(args...);
		});
}

template<typename... Args>
template<typename Object>
auto Delegate<void(Args...)>::Connect(const Object& object, void (Object::* method)(Args...) const)->DelegateHandle<void(Args...)>
{
	return Connect([&object, method](Args... args)
		{
			(object.*method)(args...);
		});
}

template<typename... Args>
void Delegate<void(Args...)>::Disconnect(const DelegateHandleId handleId)
{
	const auto begin = m_Connections.begin();
	const auto end = m_Connections.end();
	const auto find = std::find_if(begin, end,
		[handleId](const Connection& rhs)
		{
			return handleId == rhs.m_HandleId;
		});

	if (find != end)
		m_Connections.RemoveAt(find);
}

template<typename Signature>
DelegateHandle<Signature>::DelegateHandle(DelegateHandleId handleId, Delegate<Signature>& delegate)
	: m_HandleId(handleId)
	, m_Delegate(delegate)
{
}

template<typename Signature>
void DelegateHandle<Signature>::Disconnect() const
{
	// #note: handles are unique to each delegate, so we don't need to clear it after disconnecting
	m_Delegate.Disconnect(m_HandleId);
}

template <typename... Handles>
DelegateCollection::DelegateCollection(const Handles&... handles)
{
	m_Disconnects =
	{
		([=]() { handles.Disconnect(); })...
	};
}