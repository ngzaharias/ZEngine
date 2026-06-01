#include "VoxelPCH.h"
#include "SharedVoxel/VoxelTypes.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::voxel::Block& value) const
{
	Read(value.m_Flags);
	Read(value.m_Type);
}
template<>
void MemBuffer::WriteCustom(const shared::voxel::Block& value)
{
	Write(value.m_Flags);
	Write(value.m_Type);
}