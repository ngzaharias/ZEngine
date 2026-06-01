#include "VoxelPCH.h"
#include "SharedVoxel/VoxelClientModifyEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::voxel::Modify& value) const
{
	Read(value.m_WorldPos);
	Read(value.m_Data);
}
template<>
void MemBuffer::WriteCustom(const shared::voxel::Modify& value)
{
	Write(value.m_WorldPos);
	Write(value.m_Data);
}

template<>
void MemBuffer::ReadCustom(shared::voxel::ClientModifyEvent& value) const
{
	Read(value.m_Changes);
}
template<>
void MemBuffer::WriteCustom(const shared::voxel::ClientModifyEvent& value)
{
	Write(value.m_Changes);
}