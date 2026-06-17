#pragma once

#include "Core/Path.h"
#include "Core/SparseArray.h"
#include "Core/TypeInfo.h"

namespace editor::importer
{
	using ImporterId = TypeId;
	struct ImporterTag {};

	class Importer
	{
		virtual ~Importer() {};
	};

	struct ImportEntry
	{
		Importer* m_Importer = nullptr;
		TypeId m_TypeId = -1;
	};

	struct ImportManager final
	{
	public:
		void Initialise();
		void Shutdown();

		template<class TImporter>
		bool IsRegistered() const;

		template<class TImporter, typename... TArgs>
		void Register(TArgs&&... args);

		template<class TImporter>
		TImporter& Get();

	private:
		SparseArray<ImporterId, ImportEntry> m_Entries = { };
	};
}

#include "ImportManager.inl"