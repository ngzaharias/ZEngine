#pragma once

#include <Core/Map.h>

namespace eng
{
	class Visitor;

	/// \brief
	class ITableManager
	{
	public:
		ITableManager() {}
		virtual ~ITableManager() {}

		virtual void Visit(eng::Visitor& visitor) = 0;
	};

	/// \brief
	template<typename TKey, typename TObject>
	class TableManager : public ITableManager
	{
		using ObjectMap = Map<TKey, TObject>;

	public:
		TableManager() {}
		virtual ~TableManager() {}

		const ObjectMap& GetObjectMap() const { return m_ObjectMap; }

		void Visit(eng::Visitor& visitor) override;

	private:
		ObjectMap m_ObjectMap = {};
	};
}

#include "TableManager.inl"