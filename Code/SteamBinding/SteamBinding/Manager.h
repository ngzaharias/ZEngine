#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#include <steam_api.h>
#pragma warning(pop)

namespace steam
{
	class Manager final
	{
	public:
		bool Initialise();
		bool Shutdown();

		void Update();

	private:
		bool m_IsConnected = false;
	};
}