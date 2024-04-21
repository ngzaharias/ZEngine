#pragma once

#include <Core/SparseArray.h>

#include <Engine/TableEntry.h>

namespace str
{
	class Name;
}

namespace eng
{
	/// \brief 
	class TableHeadmaster final
	{
		static constexpr const char* s_Extension = ".table";
		using Entries = SparseArray<eng::TableId, eng::TableEntry>;

	public:
		/// \brief 
		void Initialise(const str::Path& folderPath);
		/// \brief 
		void Shutdown();

		template<class TManager>
		bool IsRegistered() const;

		/// \brief Registers and creates the manager.
		template<class TManager, typename... TArgs>
		void Register(const str::String& filename, TArgs&&... args);

		template<class TManager>
		auto GetManager()->TManager&;
		template<class TManager>
		auto GetManager() const -> const TManager&;

	private:
		Entries m_Entries = {};
	};
}

#include "TableHeadmaster.inl"