#pragma once

#include "Math/Vector.h"

namespace hexagon
{
	class Axial
	{
	public:
		constexpr Axial()  noexcept : x(0), y(0) {}
		constexpr Axial(const int32 value)  noexcept : x(value), y(value) {}
		constexpr Axial(const int32 x, const int32 y)  noexcept : x(x), y(y) {}
		constexpr Axial(const Vector2i& value)  noexcept : x(value.x), y(value.y) {}

		[[nodiscard]] constexpr bool operator==(const Axial& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
		[[nodiscard]] constexpr bool operator!=(const Axial& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

		[[nodiscard]] constexpr bool operator<(const Axial& rhs) const { return x != rhs.x ? x < rhs.x : y < rhs.y; }
		[[nodiscard]] constexpr bool operator>(const Axial& rhs) const { return x != rhs.x ? x > rhs.x : y > rhs.y; }
		[[nodiscard]] constexpr bool operator<=(const Axial& rhs) const { return x != rhs.x ? x <= rhs.x : y <= rhs.y; }
		[[nodiscard]] constexpr bool operator>=(const Axial& rhs) const { return x != rhs.x ? x >= rhs.x : y >= rhs.y; }

		[[nodiscard]] constexpr Axial operator+(const Axial& rhs) const noexcept { return Axial(x + rhs.x, y + rhs.y); }
		[[nodiscard]] constexpr Axial operator-(const Axial& rhs) const noexcept { return Axial(x - rhs.x, y - rhs.y); }

		constexpr Axial& operator+=(const Axial& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		constexpr Axial& operator-=(const Axial& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

		constexpr Axial operator+() const noexcept { return *this; }
		constexpr Axial operator-() const noexcept { return Axial(-x, -y); }

		operator Vector2i() { return Vector2i(x, y); }
		operator Vector2i() const { return Vector2i(x, y); }

	public:
		union
		{
			struct { int32 q, r; };
			struct { int32 x, y; };
		};
	};

	class Offset
	{
	public:
		constexpr Offset()  noexcept : x(0), y(0) {}
		constexpr Offset(const int32 value)  noexcept : x(value), y(value) {}
		constexpr Offset(const int32 x, const int32 y)  noexcept : x(x), y(y) {}
		constexpr Offset(const Vector2i& value)  noexcept : x(value.x), y(value.y) {}

		[[nodiscard]] constexpr bool operator==(const Offset& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
		[[nodiscard]] constexpr bool operator!=(const Offset& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

		[[nodiscard]] constexpr bool operator<(const Offset& rhs) const { return x != rhs.x ? x < rhs.x : y < rhs.y; }
		[[nodiscard]] constexpr bool operator>(const Offset& rhs) const { return x != rhs.x ? x > rhs.x : y > rhs.y; }
		[[nodiscard]] constexpr bool operator<=(const Offset& rhs) const { return x != rhs.x ? x <= rhs.x : y <= rhs.y; }
		[[nodiscard]] constexpr bool operator>=(const Offset& rhs) const { return x != rhs.x ? x >= rhs.x : y >= rhs.y; }

		[[nodiscard]] constexpr Offset operator+(const Offset& rhs) const noexcept { return Offset(x + rhs.x, y + rhs.y); }
		[[nodiscard]] constexpr Offset operator-(const Offset& rhs) const noexcept { return Offset(x - rhs.x, y - rhs.y); }

		constexpr Offset& operator+=(const Offset& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		constexpr Offset& operator-=(const Offset& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

		constexpr Offset operator+() const noexcept { return *this; }
		constexpr Offset operator-() const noexcept { return Offset(-x, -y); }

		operator Vector2i() { return Vector2i(x, y); }
		operator Vector2i() const { return Vector2i(x, y); }

	public:
		union
		{
			struct { int32 col, row; };
			struct { int32 x, y; };
		};
	};
}