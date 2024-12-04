#pragma once

class Vector3i;

using int32 = int32_t;

namespace enumerate
{
	/// \brief Enumerates the values and returns true if the value is found.
	template<typename Range, typename Type>
	bool Contains(const Range& range, const Type& item);

	/// \brief Enumerates the values and returns true if the predicate returns true.
	template<typename Range, typename Predicate>
	bool ContainsIf(const Range& range, const Predicate& predicate);

	/// \brief Fills the output with any values that are present in inputA but not in inputB.
	/// Example: The difference of [0, 1, 2] and [1, 2, 3] will be the value [0].
	template<typename Range>
	void Difference(const Range& inputA, const Range& inputB, Range& output);

	/// \brief Fills the output with any values that are present in both inputs.
	/// Example: The intersection of [0, 1, 2] and [1, 2, 3] will be the value [1, 2].
	template<typename Range>
	void Intersection(const Range& inputA, const Range& inputB, Range& output);

	template<typename Range>
	constexpr auto BeginForward(Range&& value) -> decltype(auto);

	template<typename Range>
	constexpr auto BeginReverse(Range&& value) -> decltype(auto);

	template<typename Range>
	constexpr auto EndForward(Range&& value) -> decltype(auto);

	template<typename Range>
	constexpr auto EndReverse(Range&& value) -> decltype(auto);

	template<typename Range>
	auto Forward(Range&& value);

	template<typename Range>
	inline auto Reverse(Range&& value);
}

#include "Algorithms.inl"