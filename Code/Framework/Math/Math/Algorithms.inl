#pragma once

#include "Math/Vector.h"

// https://www.redblobgames.com/grids/hexagons/
inline auto enumerate::Hexagonal(const int32 radius)
{
	struct Iterator
	{
		Iterator(const int32 radius, const int32 index)
			: m_Radius(radius)
			, m_Modulo(m_Radius * 2 + 1)
			, m_Index(index) 
		{ 
			FindNextValue();
		}

		auto operator*() -> const Vector2i&
		{
			return m_Value;
		}

		auto operator++() -> Iterator&
		{
			FindNextValue();
			return *this;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_Index < other.m_Index;
		}

		void FindNextValue()
		{
			for (;;)
			{
				m_Index++;
				const int32 q = (m_Index % m_Modulo) - m_Radius;
				const int32 r = ((m_Index / m_Modulo) % m_Modulo) - m_Radius;
				const int32 s = -q - r;

				const int32 length = std::abs(q) + std::abs(r) + std::abs(s);
				if (length <= m_Modulo)
				{
					m_Value.x = q;
					m_Value.y = r;
					break;
				}
			}
		}

		const int32 m_Radius;
		const int32 m_Modulo;
		Vector2i m_Value;
		int32 m_Index;
	};

	struct Range
	{
		Range(const int32 value) : m_Radius(value) { }

		auto begin() -> Iterator
		{
			// -1 because the first thing we do inside is m_Index++
			return Iterator{ m_Radius, -1 };
		}

		auto end() -> Iterator
		{
			const int32 count = m_Radius * 2 + 1;
			return Iterator{ m_Radius, count * count };
		}

		const int32 m_Radius;
	};

	return Range(radius);
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

inline auto enumerate::Vector(const Vector2i& value)
{
	struct Iterator
	{
		Iterator(const Vector2i& range, const int32 index)
			: m_Range(range)
			, m_Index(index)
		{ }

		auto operator*() -> Vector2i
		{
			return Vector2i(
				m_Index % m_Range.x,
				(m_Index / m_Range.x) % m_Range.y);
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

		const Vector2i& m_Range = Vector2i(UINT32_MAX);
		int32 m_Index = UINT32_MAX;
	};

	struct Range
	{
		Range(const Vector2i& value) : m_Range(value) {	}

		auto begin() -> Iterator
		{
			return Iterator{ m_Range, 0 };
		}

		auto end() -> Iterator
		{
			const int32 count = m_Range.x * m_Range.y;
			return Iterator{ m_Range, count };
		}

		const Vector2i& m_Range;
	};

	return Range(value);
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

inline auto enumerate::Vector(const Vector2i& min, const Vector2i& max)
{
	struct Iterator
	{
		Iterator(const Vector2i& min, const Vector2i& max, const Vector2i& range, const int32 index)
			: m_Min(min), m_Max(max), m_Range(range), m_Index(index) { }

		auto operator*() -> Vector2i
		{
			return m_Min + Vector2i(
				m_Index % m_Range.x,
				(m_Index / m_Range.x) % m_Range.y);
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

		const Vector2i& m_Min;
		const Vector2i& m_Max;
		const Vector2i& m_Range;
		int32 m_Index;
	};

	struct Range
	{
		Range(const Vector2i& min, const Vector2i& max) 
			: m_Min(min), m_Max(max), m_Range(m_Max - m_Min) { }

		auto begin() -> Iterator
		{
			return Iterator{ m_Min, m_Max, m_Range, 0 };
		}

		auto end() -> Iterator
		{
			const int32 count = m_Range.x * m_Range.y;
			return Iterator{ m_Min, m_Max, m_Range, count };
		}

		const Vector2i& m_Min;
		const Vector2i& m_Max;
		const Vector2i m_Range;
	};

	return Range(min, max);
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
		int32 m_Index;
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
		const Vector3i m_Range;
	};

	return Range(min, max);
}