#pragma once

#include "Core/EnumHelpers.h"

template<typename TMessage>
TMessage* net::Peer::CreateMessage(const EMessage type)
{
	return reinterpret_cast<TMessage*>(m_Client->CreateMessage(EnumToValue(type)));
}