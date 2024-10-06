#include "Core/Algorithms.h"

template<typename... Args>
int32 Delegate<void(Args...)>::Connect(std::function<void(Args ...)> function)
{
	Callback& callback = m_Callbacks.Emplace(function);
	callback.m_Handle = m_HandleNext++;
	return callback.m_Handle;
}

template<typename... Args>
template<typename Object>
int32 Delegate<void(Args...)>::Connect(Object& object, void (Object::* method)(Args...))
{
	return Connect([&object, method](Args... args)
		{
			(object.*method)(args...);
		});
}

template<typename... Args>
template<typename Object>
int32 Delegate<void(Args...)>::Connect(const Object& object, void (Object::* method)(Args...) const)
{
	return Connect([&object, method](Args... args)
		{
			(object.*method)(args...);
		});
}

template<typename... Args>
bool Delegate<void(Args...)>::Disconnect(const int32 handle)
{
	const auto begin = m_Callbacks.begin();
	const auto end = m_Callbacks.end();
	const auto find = std::find_if(begin, end,
		[handle](const Callback& rhs)
		{
			return handle == rhs.m_Handle;
		});

	if (find == end)
		return false;

	m_Callbacks.RemoveOrderedAt(find);
	return true;
}

template<typename... Args>
void Delegate<void(Args...)>::Publish(Args... args)
{
	for (auto& callback : m_Callbacks)
		callback.m_Function(args...);
}