#pragma once

#include "Math/Vector.h"

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

inline auto enumerate::Vector(const Vector3i& min, const Vector3i& max)
{
	struct Iterator
	{
		Iterator(const Vector3i& min, const Vector3i& max, const Vector3i& range, const int32 index)
			: m_Min(min), m_Max(max), m_Range(range), m_Index(index) { }

		auto operator*() -> Vector3i
		{
			return m_Min + Vector3i(
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

		const Vector3i& m_Min;
		const Vector3i& m_Max;
		const Vector3i& m_Range;
		int32 m_Index = UINT32_MAX;
	};

	struct Range
	{
		Range(const Vector3i& min, const Vector3i& max) 
			: m_Min(min), m_Max(max), m_Range(m_Max - m_Min + Vector3i::One) { }

		auto begin() -> Iterator
		{
			return Iterator{ m_Min, m_Max, m_Range, 0 };
		}

		auto end() -> Iterator
		{
			const int32 count = m_Range.x * m_Range.y * m_Range.z;
			return Iterator{ m_Min, m_Max, m_Range, count };
		}

		const Vector3i& m_Min;
		const Vector3i& m_Max;
		const Vector3i m_Range = Vector3i(UINT32_MAX);
	};

	return Range(min, max);
}