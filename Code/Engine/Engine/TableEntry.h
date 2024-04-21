#pragma once

#include <Core/Path.h>
#include <Core/String.h>

using int32 = int32_t;

namespace eng
{
	class ITableManager;

	using TableId = int32;

	struct TableEntry
	{
		str::String m_Name = {};
		str::String m_Filename = {};
		ITableManager* m_Manager = nullptr;
	};

	struct TableTag {};
}