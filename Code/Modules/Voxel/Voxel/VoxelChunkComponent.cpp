#include "VoxelPCH.h"
#include "Voxel/VoxelChunkComponent.h"

#include "Core/EnumHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strData = "m_Data";
}

template<>
void eng::Visitor::ReadCustom(voxel::ChunkComponent& value) const
{
	Array<int32> data;
	Read(strData, data, {});
	for (const int32 index : data)
	{
		voxel::Block& block = value.m_Data.Emplace();
		block.m_Type = IndexToEnum<voxel::EType>(index);
	}
}
template<>
void eng::Visitor::WriteCustom(const voxel::ChunkComponent& value)
{
	Array<int32> data;
	for (const voxel::Block block : value.m_Data)
	{
		const int32 index = EnumToIndex<voxel::EType>(block.m_Type);
		data.Emplace(index);
	}
	Write(strData, data);
}
template<>
bool imgui::Inspector::WriteCustom(voxel::ChunkComponent& value)
{
	return false;
}