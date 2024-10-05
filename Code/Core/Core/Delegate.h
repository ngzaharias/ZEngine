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
	int32 Connect(std::function<void(Args ...)> function);
	template<typename Object>
	int32 Connect(Object& object, void (Object::* method)(Args...));
	template<typename Object>
	int32 Connect(const Object& object, void (Object::* method)(Args...) const);

	bool Disconnect(const int32 handle);

	void Publish(Args... args);

private:
	struct Callback
	{
		std::function<void(Args...)> m_Function;
		int32 m_Handle = -1;
	};

	Array<Callback> m_Callbacks = {};
	int32 m_HandleNext = 0;
};

#include "Core/Delegate.inl"