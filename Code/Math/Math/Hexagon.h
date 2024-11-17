#pragma once

namespace hexagon
{
	struct Axial
	{
		operator Vector2i() { return Vector2i(q, r); }
		operator Vector2i() const { return Vector2i(q, r); }

		int32 q, r;
	};

	struct Offset
	{
		bool operator<(const Offset& rhs) const 
		{ 
			return col != rhs.col 
				? col < rhs.col
				: row < rhs.row;
		}

		operator Vector2i() { return Vector2i(col, row); }
		operator Vector2i() const { return Vector2i(col, row); }

		int32 col, row;
	};
}