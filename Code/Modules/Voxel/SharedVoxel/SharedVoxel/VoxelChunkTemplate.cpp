#include "VoxelPCH.h"
#include "SharedVoxel/VoxelChunkTemplate.h"

#include "Core/EnumHelpers.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strData = "m_Data";
}

template<>
void Visitor::ReadCustom(shared::voxel::ChunkTemplate& value) const
{
	Array<int32> data;
	Read(strData, data, {});
	for (const int32 index : data)
	{
		shared::voxel::Block& block = value.m_Data.Emplace();
		block.m_Type = IndexToEnum<shared::voxel::EType>(index);
	}
}
template<>
void Visitor::WriteCustom(const shared::voxel::ChunkTemplate& value)
{
	Array<int32> data;
	for (const shared::voxel::Block block : value.m_Data)
	{
		const int32 index = EnumToIndex<shared::voxel::EType>(block.m_Type);
		data.Emplace(index);
	}
	Write(strData, data);
}