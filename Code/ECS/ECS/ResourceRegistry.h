#pragma once

#include <Core/SparseArray.h>

namespace ecs
{
	using ResourceId = int32;
	struct ResourceTag {};

	struct ResourceEntry
	{
		void* m_Resource = nullptr;
		str::String m_Name = { };
	};

	class ResourceRegistry
	{
	public:
		template<class TResource>
		bool Has() const;

		template<class TResource>
		TResource& Get();

		template<class TResource>
		void Add(TResource& resource);

		template<class TResource>
		void Remove(TResource& resource);

	private:
		SparseArray<ecs::ResourceId, ecs::ResourceEntry> m_Entries = { };
	};
}

#include "ResourceRegistry.inl"

