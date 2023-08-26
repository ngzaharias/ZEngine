#include "EnginePCH.h"
#include "Engine/SinkCollection.h"

SinkCollection::SinkCollection(Array<Connection>&& connections)
	: m_Connections(std::move(connections))
{
}

SinkCollection::SinkCollection(std::initializer_list<Connection>&& connections)
	: m_Connections(std::move(connections))
{
}

SinkCollection::~SinkCollection()
{
	Disconnect();
}

SinkCollection& SinkCollection::operator=(Array<Connection>&& connections) noexcept
{
	Disconnect();
	m_Connections = std::move(connections);
	return *this;
}

void SinkCollection::Disconnect()
{
	for (Connection& connection : m_Connections)
		connection.release();
	m_Connections.RemoveAll();
}