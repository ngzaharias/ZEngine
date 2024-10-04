#pragma once

#include "Core/Array.h"

#include <functional>

template<typename Signature>
class Delegate;

// https://simoncoenen.com/blog/programming/CPP_Delegates
template<typename... Args>
class Delegate<void(Args...)>
{
public:
	void Connect(std::function<void(Args ...)> function)
	{
		m_Callbacks.Emplace(function);
	}

	template<typename Object>
	void Connect(Object& object, void (Object::* method)(Args...))
	{
		Connect([&object, &method](Args... args)
			{
				(object.*method)(args...);
			});
	}

	template<typename Object>
	void Connect(const Object& object, void (Object::* method)(Args...) const)
	{
		Connect([&object, &method](Args... args)
			{
				(object.*method)(args...);
			});
	}

	void Disconnect() {}

	void Publish(Args... args) 
	{
		for (auto& function : m_Callbacks)
			function(args...);
	}

private:
	using Callback = std::function<void(Args...)>;
	Array<Callback> m_Callbacks = {};
};

#include "Core/Delegate.inl"