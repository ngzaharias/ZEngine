#pragma once

#include "Core/Array.h"

#include <functional>

template<typename Signature>
class Delegate;

template<typename Signature>
class DelegateHandle;

using DelegateHandleId = uint32;

/// \brief An object that holds a list of connections that can be evoked by the owner without having to know about the receivers.
/// 
/// Neither a delegate or its receivers can be moved in memory at any time.
/// There is no guarantee on the order that the receivers are called.
template<typename... Args>
class Delegate<void(Args...)>
{
	using DelegateFunction = std::function<void(Args...)>;

	struct Connection
	{
		DelegateHandleId m_HandleId;
		DelegateFunction m_Function;
	};

public:
	void Publish(Args... args) const;

	auto Connect(std::function<void(Args ...)> function)->DelegateHandle<void(Args...)>;
	template<typename Object>
	auto Connect(Object& object, void (Object::* method)(Args...))->DelegateHandle<void(Args...)>;
	template<typename Object>
	auto Connect(const Object& object, void (Object::* method)(Args...) const)->DelegateHandle<void(Args...)>;

	void Disconnect(const DelegateHandleId handleId);

private:
	DelegateHandleId m_HandleIdNext = 0;
	Array<Connection> m_Connections = {};
};

/// \brief Holds a connection to a delegate that can be later disconnected without interacting with the owner.
template<typename Signature>
class DelegateHandle
{
public:
	DelegateHandle(DelegateHandleId handleId, Delegate<Signature>& delegate);

	void Disconnect() const;

private:
	DelegateHandleId m_HandleId;
	Delegate<Signature>& m_Delegate;
};

/// \brief Holds a collection of connections that will be automatically disconnected when its lifetime expires.
class DelegateCollection
{
	using Function = std::function<void()>;

public:
	DelegateCollection();
	DelegateCollection(DelegateCollection&& rhs) noexcept;
	template <typename... Handles>
	DelegateCollection(const Handles&... handles);
	DelegateCollection(const DelegateCollection&) = delete;
	~DelegateCollection();

	auto operator=(DelegateCollection&& other) noexcept->DelegateCollection&;
	auto operator=(const DelegateCollection& other)->DelegateCollection & = delete;

	// \brief Disconnect all connections.
	void Disconnect();

private:
	Array<Function> m_Disconnects;
};

#include "Core/Delegate.inl"