#pragma once

#define BIT32(value) (1<<value)
#define BIT64(value) (static_cast<uint64>(1)<<(value))

namespace voxel
{
	constexpr float	s_BlockSize1D = 100.f;
	constexpr int32 s_ChunkSize1D = 16;
	constexpr int32 s_ChunkSize2D = s_ChunkSize1D * s_ChunkSize1D;
	constexpr int32 s_ChunkSize3D = s_ChunkSize1D * s_ChunkSize1D * s_ChunkSize1D;

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