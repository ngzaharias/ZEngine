#include "VoxelPCH.h"
#include "SharedVoxel/VoxelChunkComponent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::voxel::ChunkComponent& value) const
{
	Read(value.m_Data);
}
template<>
void MemBuffer::WriteCustom(const shared::voxel::ChunkComponent& value)
{
	Write(value.m_Data);
}