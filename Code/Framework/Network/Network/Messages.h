#pragma once

#include "Network/Message.h"
#include "Network/MessageEnum.h"
#include "Network/PeerId.h"

namespace net
{
	struct PeerHandshakeMessage final : public net::Message
	{
		net::PeerId m_PeerId = {};
	};
}