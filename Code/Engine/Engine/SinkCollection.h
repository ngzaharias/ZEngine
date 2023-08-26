#pragma once

#include <Core/Array.h>

#include <entt/signal/sigh.hpp>

using Connection = entt::connection;

class SinkCollection
{
public:
	SinkCollection() = default;
	SinkCollection(Array<Connection>&&);
	SinkCollection(std::initializer_list<Connection>&&);
	~SinkCollection();

	SinkCollection& operator=(Array<Connection>&&) noexcept;

	void Disconnect();

private:
	Array<Connection> m_Connections = { };
};
