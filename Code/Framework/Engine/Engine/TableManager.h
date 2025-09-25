#pragma once

#include "Core/Map.h"

namespace eng
{
	class Visitor;

	/// \brief
	class ITableManager
	{
	public:
		ITableManager() {}
		virtual ~ITableManager() {}

		virtual void Read(eng::Visitor& visitor) = 0;
	};

	/// \brief
	template<typename TKey, typename TObject>
	class TableManager : public ITableManager
	{
		using ObjectMap = Map<TKey, TObject>;

	public:
		TableManager() {}
		virtual ~TableManager() {}

		bool HasObject(const TKey& value) const { return m_ObjectMap.Contains(value); }
		const TObject& GetObject(const TKey& value) const { return m_ObjectMap.Get(value); }
		const ObjectMap& GetObjectMap() const { return m_ObjectMap; }

		void Read(eng::Visitor& visitor) override;

	protected:
		ObjectMap m_ObjectMap = {};
	};
}

#include "TableManager.inl"