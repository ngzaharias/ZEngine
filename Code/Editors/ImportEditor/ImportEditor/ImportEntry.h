#pragma once


namespace editor::importer
{
	class Importer;
}

namespace editor::importer
{
	struct Entry
	{
		Importer* m_Importer = nullptr;
		TypeId m_TypeId = -1;
	};
}