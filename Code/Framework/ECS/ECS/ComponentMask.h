#pragma once

#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "Core/Types.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentTag.h"

#include <bitset>

namespace ecs
{
	// multiples of 64
	constexpr int32 COMPONENTS_MAX = 4 * 64;

	class ComponentMask
	{
	public:
		ComponentMask operator|(const ComponentMask& rhs) const;
		ComponentMask operator&(const ComponentMask& rhs) const;
		ComponentMask operator^(const ComponentMask& rhs) const;
		ComponentMask operator~() const;

		ComponentMask& operator|=(const ComponentMask& rhs);
		ComponentMask& operator&=(const ComponentMask& rhs);
		ComponentMask& operator^=(const ComponentMask& rhs);

		void Reset();
		void Clear(const int32 index);
		void Raise(const int32 index);
		bool Has(const int32 index) const;

		bool HasAll() const;
		bool HasAll(const ComponentMask& rhs) const;

		bool HasAny() const;
		bool HasAny(const ComponentMask& rhs) const;

		bool HasNone() const;
		bool HasNone(const ComponentMask& rhs) const;

	private:
		std::bitset<COMPONENTS_MAX> m_Data;
	};

	template<typename TComponent>
	inline ecs::ComponentId ToComponentId();

	template<typename ...TComponents>
	inline ecs::ComponentMask ToComponentMask();

	template<typename ...TComponents>
	inline ecs::ComponentMask ToComponentMask(TypeList<TComponents...>);
}

#include "ComponentMask.inl"