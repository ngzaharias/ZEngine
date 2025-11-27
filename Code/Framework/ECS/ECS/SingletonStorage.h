#pragma once

#include "Core/SparseArray.h"
#include "ECS/Entity.h"

namespace ecs
{
	class ISingletonStorage
	{
		friend class EntityWorld;

	public:
		virtual ~ISingletonStorage() = default;
	};

	template<typename TSingleton>
	class SingletonStorage : public ISingletonStorage
	{
		friend class EntityWorld;

	public:
		~SingletonStorage() override = default;

		inline TSingleton& GetData();
		inline const TSingleton& GetData() const;

	private:
		TSingleton m_Data = { };
	};
}

#include "SingletonStorage.inl"