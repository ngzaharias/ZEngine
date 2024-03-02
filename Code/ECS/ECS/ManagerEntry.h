#pragma once

#include <Core/String.h>

namespace ecs
{
	class Manager;
	using ManagerId = int32;

	struct ManagerEntry
	{
		ecs::Manager* m_Manager = nullptr;
		str::String m_Name = { };
	};
}