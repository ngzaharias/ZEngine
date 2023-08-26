#pragma once

#include <Core/String.h>

namespace ecs
{
	class Manager;
	using ManagerId = int32;

	struct ManagerEntry
	{
		ecs::Manager* m_Manager = nullptr;

		ecs::ManagerId m_ManagerId = INT32_MAX;
		str::String m_DebugName = { };
		bool m_IsExternal = false;
	};
}