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
		void Register(TResource& manager);

		template<class TResource>
		bool IsRegistered() const;

		template<class TResource>
		TResource& GetResource();

	private:
		SparseArray<ecs::ResourceId, ecs::ResourceEntry> m_Entries = { };
	};
}

#include "ResourceRegistry.inl"

