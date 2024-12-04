#pragma once

#include "Core/SparseArray.h"

namespace ecs
{
	using ResourceId = int32;
	struct ResourceTag {};

	struct ResourceEntry
	{
		str::String m_Name = { };
		void* m_Resource = nullptr;
	};

	class ResourceRegistry
	{
	public:
		template<class TResource>
		bool IsRegistered() const;

		template<class TResource>
		void Register(TResource& resource);

		template<class TResource>
		TResource& Get();

	private:
		SparseArray<ecs::ResourceId, ecs::ResourceEntry> m_Entries = { };
	};
}

#include "ResourceRegistry.inl"

