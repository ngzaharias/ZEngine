#pragma once

#include "Core/Map.h"
#include "Core/Path.h"
#include "Serialize/Visitor.h"

class Visitor;

namespace eng
{
	/// \brief
	class ITableManager
	{
	public:
		ITableManager() {}
		virtual ~ITableManager() {}

		virtual void Load(const str::Path& filepath) {}
		virtual void PostLoad() {}
	};

	/// \brief
	template<typename TKey, typename TObject>
	class TableManager : public ITableManager
	{
		using ObjectMap = Map<TKey, TObject>;

	public:
		TableManager() {}
		virtual ~TableManager() {}

		void Load(const str::Path& filepath) override;

		void AddObject(const TKey& key, TObject&& value);
		void AddObject(const TKey& key, const TObject& value);
		bool HasObject(const TKey& key) const;
		auto GetObject(const TKey& key) const -> const TObject&;
		auto GetObjects() const -> const ObjectMap&;

	protected:
		ObjectMap m_ObjectMap = {};
	};
}

#include "TableManager.inl"