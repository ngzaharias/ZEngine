#pragma once

#include "Math/Hexagon.h"
#include "Math/Vector.h"

namespace hexmap
{
	class LayerPos
	{
	public:
		constexpr LayerPos()  noexcept : x(0), y(0) {}
		constexpr explicit LayerPos(const int32 value)  noexcept : x(value), y(value) {}
		constexpr explicit LayerPos(const int32 x, const int32 y)  noexcept : x(x), y(y) {}
		constexpr explicit LayerPos(const Vector2i& value)  noexcept : x(value.x), y(value.y) {}

		[[nodiscard]] constexpr bool operator==(const LayerPos& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
		[[nodiscard]] constexpr bool operator!=(const LayerPos& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

		[[nodiscard]] constexpr bool operator<(const LayerPos& rhs) const { return x != rhs.x ? x < rhs.x : y < rhs.y; }
		[[nodiscard]] constexpr bool operator>(const LayerPos& rhs) const { return x != rhs.x ? x > rhs.x : y > rhs.y; }
		[[nodiscard]] constexpr bool operator<=(const LayerPos& rhs) const { return x != rhs.x ? x <= rhs.x : y <= rhs.y; }
		[[nodiscard]] constexpr bool operator>=(const LayerPos& rhs) const { return x != rhs.x ? x >= rhs.x : y >= rhs.y; }

		[[nodiscard]] constexpr LayerPos operator+(const LayerPos& rhs) const noexcept { return LayerPos(x + rhs.x, y + rhs.y); }
		[[nodiscard]] constexpr LayerPos operator-(const LayerPos& rhs) const noexcept { return LayerPos(x - rhs.x, y - rhs.y); }

		constexpr LayerPos& operator+=(const LayerPos& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		constexpr LayerPos& operator-=(const LayerPos& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

		constexpr LayerPos operator+() const noexcept { return *this; }
		constexpr LayerPos operator-() const noexcept { return LayerPos(-x, -y); }

		operator Vector2i() { return Vector2i(x, y); }
		operator Vector2i() const { return Vector2i(x, y); }

	public:
		int32 x, y;
	};

	struct HexData
	{
		int32 m_Index = 0;
	};

	class HexPos
	{
	public:
		constexpr HexPos()  noexcept : x(0), y(0) {}
		constexpr HexPos(const int32 value)  noexcept : x(value), y(value) {}
		constexpr HexPos(const int32 x, const int32 y)  noexcept : x(x), y(y) {}
		constexpr HexPos(const Vector2i& value)  noexcept : x(value.x), y(value.y) {}
		constexpr HexPos(const hexagon::Offset& value)  noexcept : x(value.x), y(value.y) {}

		[[nodiscard]] constexpr bool operator==(const HexPos& rhs) const noexcept { return (x == rhs.x) && (y == rhs.y); }
		[[nodiscard]] constexpr bool operator!=(const HexPos& rhs) const noexcept { return (x != rhs.x) || (y != rhs.y); }

		[[nodiscard]] constexpr bool operator<(const HexPos& rhs) const { return x != rhs.x ? x < rhs.x : y < rhs.y; }
		[[nodiscard]] constexpr bool operator>(const HexPos& rhs) const { return x != rhs.x ? x > rhs.x : y > rhs.y; }
		[[nodiscard]] constexpr bool operator<=(const HexPos& rhs) const { return x != rhs.x ? x <= rhs.x : y <= rhs.y; }
		[[nodiscard]] constexpr bool operator>=(const HexPos& rhs) const { return x != rhs.x ? x >= rhs.x : y >= rhs.y; }

		[[nodiscard]] constexpr HexPos operator+(const HexPos& rhs) const noexcept { return HexPos(x + rhs.x, y + rhs.y); }
		[[nodiscard]] constexpr HexPos operator-(const HexPos& rhs) const noexcept { return HexPos(x - rhs.x, y - rhs.y); }

		constexpr HexPos& operator+=(const HexPos& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		constexpr HexPos& operator-=(const HexPos& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }

		constexpr HexPos operator+() const noexcept { return *this; }
		constexpr HexPos operator-() const noexcept { return HexPos(-x, -y); }

		operator hexagon::Offset() { return hexagon::Offset(x, y); }
		operator hexagon::Offset() const { return hexagon::Offset(x, y); }

		operator Vector2i() { return Vector2i(x, y); }
		operator Vector2i() const { return Vector2i(x, y); }

	public:
		int32 x, y;
	};

}