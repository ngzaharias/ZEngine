#pragma once

#include "Core/Tuple.h"
#include "Core/TypeTraits.h"

#include <algorithm>
#include <iterator>

namespace detail
{
	template<typename Range, typename Type>
	using HasContainsMethod = decltype(std::declval<Range&>().Contains(std::declval<Type&>()));

	template<typename Range, typename Type>
	using HasStdCountMethod = decltype(std::declval<Range&>().count(std::declval<Type&>()));

	template<typename Range, typename Type>
	using HasStdFindMethod = decltype(std::declval<Range&>().find(std::declval<Type&>()));
}

template<typename Range, typename Type>
bool enumerate::Contains(const Range& range, const Type& item)
{
	if constexpr (core::IsDetected<detail::HasContainsMethod, Range, Type>::value)
	{
		return range.Contains(item);
	}
	else if constexpr (core::IsDetected<detail::HasStdCountMethod, Range, Type>::value)
	{
		return range.count(item) > 0;
	}
	else if constexpr (core::IsDetected<detail::HasStdFindMethod, Range, Type>::value)
	{
		return range.find(item) != range.end();
	}
	else
	{
		return std::find(range.cbegin(), range.cend(), item) != range.cend();
	}
}

template<typename Range, typename Predicate>
bool enumerate::ContainsIf(const Range& range, const Predicate& predicate)
{
	return std::find_if(range.cbegin(), range.cend(), predicate) != range.cend();
}

template<typename Range>
void enumerate::Difference(const Range& inputA, const Range& inputB, Range& output)
{
	std::set_difference(
		inputA.cbegin(),
		inputA.cend(),
		inputB.cbegin(),
		inputB.cend(),
		std::inserter(output, output.begin()));
}

template<typename Range>
void enumerate::Intersection(const Range& inputA, const Range& inputB, Range& output)
{
	std::set_intersection(
		inputA.cbegin(),
		inputA.cend(),
		inputB.cbegin(),
		inputB.cend(),
		std::inserter(output, output.begin()));
}

template<typename Range>
constexpr auto enumerate::BeginForward(Range&& range) -> decltype(auto)
{
	return std::begin(range);
}

template<typename Range>
constexpr auto enumerate::BeginReverse(Range&& range) -> decltype(auto)
{
	return std::rbegin(range);
}

template<typename Range>
constexpr auto enumerate::EndForward(Range&& range) -> decltype(auto)
{
	return std::end(range);
}

template<typename Range>
constexpr auto enumerate::EndReverse(Range&& range) -> decltype(auto)
{
	return std::rend(range);
}

template<typename Range>
auto enumerate::Forward(Range&& value)
{
	using ItrBegin = decltype(enumerate::BeginForward(std::declval<Range>()));
	using ItrEnd = decltype(enumerate::EndForward(std::declval<Range>()));
	using Value = decltype(*std::declval<ItrBegin&>());

	static_assert(std::is_move_constructible<ItrBegin>::value);
	static_assert(std::is_move_constructible<ItrEnd>::value);
	static_assert(std::is_reference<Range>::value, "enumerate::Forward doesn't work with an r-value.");

	struct Iterator
	{
		auto operator*() -> Tuple<int32, Value>
		{
			return Tuple<int32, Value>(m_Index, *m_Itr);
		}

		auto operator++() -> Iterator&
		{
			m_Itr++;
			m_Index++;
			return *this;
		}

		bool operator!=(const ItrEnd& rhs) const
		{
			return m_Itr != rhs;
		}

		ItrBegin m_Itr;
		int32 m_Index;
	};

	struct Wrapper
	{
		Wrapper(Range&& data) : m_Data(std::forward<Range>(data)) { }
		Wrapper(const Wrapper&) = delete;
		Wrapper(Wrapper&&) = delete;

		auto begin() -> Iterator
		{
			ItrBegin itr = enumerate::BeginForward(std::forward<Range>(m_Data));
			return Iterator{ std::move(itr), 0 };
		}

		auto end() -> ItrEnd
		{
			return enumerate::EndForward(std::forward<Range>(m_Data));
		}

		Range&& m_Data;
	};

	return Wrapper{ std::forward<Range>(value) };
}

template<typename Range>
auto enumerate::Reverse(Range&& value)
{
	using ItrBegin = decltype(enumerate::BeginReverse(std::declval<Range>()));
	using ItrEnd = decltype(enumerate::EndReverse(std::declval<Range>()));
	using Value = decltype(*std::declval<ItrBegin&>());

	static_assert(std::is_move_constructible<ItrBegin>::value);
	static_assert(std::is_move_constructible<ItrEnd>::value);
	static_assert(std::is_reference<Range>::value, "enumerate::Reverse doesn't work with an r-value.");

	struct Iterator
	{
		auto operator*() -> Tuple<int32, Value>
		{
			return Tuple<int32, Value>(m_Index, *m_Itr);
		}

		auto operator++() -> Iterator&
		{
			m_Itr++;
			m_Index--;
			return *this;
		}

		bool operator!=(const ItrEnd& rhs) const
		{
			return m_Itr != rhs;
		}

		ItrBegin m_Itr;
		int32 m_Index;
	};

	struct Wrapper
	{
		auto begin() -> Iterator
		{
			const int32 count = m_Data.GetCount();
			ItrBegin itr = enumerate::BeginReverse(std::forward<Range>(m_Data));
			return Iterator{ std::move(itr), count - 1 };
		}

		auto end() -> ItrEnd
		{
			return enumerate::EndReverse(std::forward<Range>(m_Data));
		}

		Range&& m_Data;
	};

	return Wrapper{ std::forward<Range>(value) };
}