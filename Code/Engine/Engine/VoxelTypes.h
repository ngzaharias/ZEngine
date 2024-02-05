#pragma once

#define BIT32(value) (1<<value)
#define BIT64(value) (static_cast<uint64>(1)<<(value))

namespace voxel
{
	constexpr int32 s_BlockCount1D = 16;
	constexpr int32 s_BlockCount2D = s_BlockCount1D * s_BlockCount1D;
	constexpr int32 s_BlockCount3D = s_BlockCount1D * s_BlockCount1D * s_BlockCount1D;

	constexpr float	s_BlockSize1D = 100.f;
	constexpr float s_ChunkSize1D = s_BlockSize1D * s_BlockCount1D;

	enum class EDirection
	{
		XNeg,
		XPos,
		YNeg,
		YPos,
		ZNeg,
		ZPos,
	};

	enum class EFlags : uint64
	{
		None = 0,
	};

	enum class EType : uint16
	{
		None = 0,

		Black,
		Green,
		Grey,
		Orange,
		Purple,
		Red,
	};

	struct Block
	{
		EFlags m_Flags = EFlags::None;
		EType m_Type = EType::None;
	};
}

#undef BIT32
#undef BIT64