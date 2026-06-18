#pragma once

#include "Core/Path.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"
#include "ImportEditor/ImportEntry.h"

namespace editor::importer
{
	struct Tag {};

	struct Manager final
	{
	public:
		Manager();
		~Manager();

		template<class TImporter>
		bool IsRegistered() const;

		template<class TImporter, typename... TArgs>
		void Register(TArgs&&... args);

		template<class TImporter>
		TImporter& Get();

	private:
		SparseArray<TypeId, Entry> m_Entries = { };
	};
}

#include "ImportManager.inl"