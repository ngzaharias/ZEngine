#pragma once

namespace net
{
	enum class EMode
	{
		/// \brief Neither client or server is running.
		Inactive,
		/// \brief The game is running as a local client, server that doesn't accept connections 
		/// from remote clients, and it will run both client-side and server-side logic.
		Standalone,
		/// \brief The game is running as a remote client that is connected to a server that will 
		/// run client-side logic only.
		RemoteClient,
		/// \brief The game is running as a local client, server that does accept connections 
		/// from remote clients, and it will run both client-side and server-side logic.
		ListenServer,
		/// \brief The game is running as a server that does accept connections from remote clients, 
		/// and will run server-side logic only.
		DedicatedServer,
	};
}