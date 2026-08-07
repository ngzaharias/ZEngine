#pragma once

#include "Core/SparseMap.h"
#include "ECS/ResourceId.h"
#include "ECS/ResourceTag.h"

namespace ecs
{
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
		SparseMap<ecs::ResourceId, ecs::ResourceEntry> m_Entries = { };
	};
}

#include "ResourceRegistry.inl"

