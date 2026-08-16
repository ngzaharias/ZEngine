#pragma once

#include "Core/TypeInfo.h"
#include "Core/TypeList.h"
#include "Core/Types.h"
#include "ECS/Component.h"
#include "ECS/ComponentId.h"
#include "ECS/ComponentTag.h"

#include <bitset>

namespace ecs
{
	// multiples of 64
	constexpr int32 COMPONENTS_MAX = 64 * 4;

	class ComponentMask
	{
		static constexpr uint32 COUNT = COMPONENTS_MAX;
		static constexpr uint16 STRIDE = sizeof(uint64) * 8;
		static constexpr uint16 CHUNKS = COUNT / STRIDE;

	public:
		struct Iterator
		{
			friend class ComponentMask;

			auto operator*() -> ecs::ComponentId;
			auto operator++() -> Iterator&;
			bool operator!=(const Iterator& rhs) const;

			const ComponentMask& m_Mask;
			int32 m_Index;
		};

		auto begin() const -> Iterator;
		auto end() const -> Iterator;

	public:
		[[nodiscard]] bool operator<(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] bool operator<=(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] bool operator>(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] bool operator>=(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] bool operator==(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] bool operator!=(const ecs::ComponentMask& rhs) const noexcept;

		[[nodiscard]] ecs::ComponentMask operator|(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] ecs::ComponentMask operator&(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] ecs::ComponentMask operator^(const ecs::ComponentMask& rhs) const noexcept;
		[[nodiscard]] ecs::ComponentMask operator~() const noexcept;

		ecs::ComponentMask& operator|=(const ecs::ComponentMask& rhs) noexcept;
		ecs::ComponentMask& operator&=(const ecs::ComponentMask& rhs) noexcept;
		ecs::ComponentMask& operator^=(const ecs::ComponentMask& rhs) noexcept;

		/// \brief Clears the bit at index.
		void Clear(const int32 index) noexcept;
		/// \brief Clears all bits in the mask that are raised in value.
		void Clear(const ecs::ComponentMask& value) noexcept;
		/// \brief Clears all bits in the mask.
		void ClearAll() noexcept;

		/// \brief Raises the bit at index.
		void Raise(const int32 index) noexcept;
		/// \brief Raises all bits in the mask that are raised in value.
		void Raise(const ecs::ComponentMask& value) noexcept;
		/// \brief Raises all bits in the mask.
		void RaiseAll() noexcept;

		/// \brief Returns true if the bit at index is raised.
		bool Has(const int32 index) const noexcept;

		/// \brief Returns true if all bits are raised.
		bool HasAll() const noexcept;
		/// \brief Returns true if all bits that are raised in value are also raised in the mask.
		bool HasAll(const ecs::ComponentMask& value) const noexcept;

		/// \brief Returns true if any bits are raised.
		bool HasAny() const noexcept;
		/// \brief Returns true if any bits that are raised in value are also raised in the mask.
		bool HasAny(const ecs::ComponentMask& value) const noexcept;

		/// \brief Returns true if no bits are raised.
		bool HasNone() const noexcept;
		/// \brief Returns true if none of the bits that are raised in value are raised in the mask.
		bool HasNone(const ecs::ComponentMask& value) const noexcept;

	private:
		uint64 m_Data[CHUNKS] = {};
	};

	template<typename TComponent>
	inline ecs::ComponentId ToComponentId();

	template<typename ...TComponents>
	inline ecs::ComponentMask ToComponentMask();

	template<typename ...TComponents>
	inline ecs::ComponentMask ToComponentMask(TypeList<TComponents...>);
}

#include "ComponentMask.inl"