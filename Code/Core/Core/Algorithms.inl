#pragma once

#include <Core/Tuple.h>
#include <Core/TypeTraits.h>
#include <Core/Vector.h>

#include <algorithm>

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
bool core::Contains(const Range& range, const Type& item)
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
bool core::ContainsIf(const Range& range, const Predicate& predicate)
{
	return std::find_if(range.cbegin(), range.cend(), predicate) != range.cend();
}

template<typename Range>
void core::Difference(const Range& inputA, const Range& inputB, Range& output)
{
	std::set_difference(
		inputA.cbegin(),
		inputA.cend(),
		inputB.cbegin(),
		inputB.cend(),
		std::inserter(output, output.begin()));
}

template<typename Range>
void core::Intersection(const Range& inputA, const Range& inputB, Range& output)
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

inline auto enumerate::Length(const float stride, const float length)
{
	struct ItrEnd { };
	struct ItrBegin
	{
		auto operator*() -> float
		{
			return m_Distance;
		}

		auto operator++() -> ItrBegin&
		{
			const bool isLastIteration = m_Distance >= m_Length;
			m_Distance += m_Stride;
			if (!isLastIteration)
				m_Distance = math::Min(m_Distance, m_Length);
			return *this;
		}

		bool operator!=(const ItrEnd& rhs) const
		{
			return m_Distance <= m_Length;
		}

		float m_Distance = 0.f;
		float m_Length = 0.f;
		float m_Stride = 1.f;
	};

	struct Range
	{
		Range(const float distance, const float length, const float stride)
			: m_Distance(distance), m_Length(length), m_Stride(stride) { }

		auto begin() -> ItrBegin
		{
			return ItrBegin{ m_Distance, m_Length, m_Stride };
		}

		auto end() -> ItrEnd
		{
			return ItrEnd();
		}

		float m_Distance = 0.f;
		float m_Length = 0.f;
		float m_Stride = 1.f;
	};

	return Range(0.f, length, stride);
}

inline auto enumerate::Vector(const Vector3i& value)
{
	struct Iterator
	{
		Iterator(const Vector3i& range, const int32 index)
			: m_Range(range)
			, m_Index(index)
		{ }

		auto operator*() -> Vector3i
		{
			return Vector3i(
				m_Index % m_Range.x,
				(m_Index / m_Range.x) % m_Range.y,
				(m_Index / (m_Range.x * m_Range.y)) % m_Range.z);
		}

		auto operator++() -> Iterator&
		{
			m_Index++;
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_Index != other.m_Index;
		}

		const Vector3i& m_Range = Vector3i(UINT32_MAX);
		int32 m_Index = UINT32_MAX;
	};

	struct Range
	{
		Range(const Vector3i& value) : m_Range(value) {	}

		auto begin() -> Iterator
		{
			return Iterator{ m_Range, 0 };
		}

		auto end() -> Iterator
		{
			const int32 count = m_Range.x * m_Range.y * m_Range.z;
			return Iterator{ m_Range, count };
		}

		const Vector3i& m_Range;
	};

	return Range(value);
}