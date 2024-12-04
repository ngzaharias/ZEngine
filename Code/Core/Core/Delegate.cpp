#include "Core/Delegate.h"

DelegateCollection::DelegateCollection()
{
}

DelegateCollection::DelegateCollection(DelegateCollection&& rhs) noexcept
	: m_Disconnects(std::move(rhs.m_Disconnects))
{
}

auto DelegateCollection::operator=(DelegateCollection&& rhs) noexcept->DelegateCollection&
{
	m_Disconnects = std::move(rhs.m_Disconnects);
	return *this;
}

DelegateCollection::~DelegateCollection()
{
	Disconnect();
}

void DelegateCollection::Disconnect()
{
	for (auto& disconnect : m_Disconnects)
		disconnect();
	m_Disconnects.RemoveAll();
}
