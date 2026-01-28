#pragma once

#include "Core/SparseArray.h"

namespace ecs
{
	class ISingletonContainer
	{
	public:
		virtual ~ISingletonContainer() = default;
	};

	template<typename TSingleton>
	class SingletonContainer : public ISingletonContainer
	{
	public:
		~SingletonContainer() override = default;

		inline TSingleton& GetData();
		inline const TSingleton& GetData() const;

	private:
		TSingleton m_Data = {};
	};
}

#include "SingletonContainer.inl"