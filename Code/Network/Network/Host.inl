#pragma once

#include "Core/EnumHelpers.h"

template<typename TMessage>
TMessage* net::Host::CreateMessage(const PeerId& peerId, const EMessage type)
{
	return reinterpret_cast<TMessage*>(m_Server->CreateMessage(peerId.m_Value, EnumToValue(type)));
}