#include "EnginePCH.h"
#include "Engine/VoxelComponents.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strData = "m_Data";
}

template<>
void eng::Visitor::ReadCustom(voxel::ChunkComponent& value) const
{
	Array<voxel::EType> data;
	Read(strData, data, {});
	for (const voxel::EType blockType : data)
	{
		voxel::Block& block = value.m_Data.Emplace();
		block.m_Type = blockType;
	}
}
template<>
void eng::Visitor::WriteCustom(const voxel::ChunkComponent& value)
{
	Array<voxel::EType> data;
	for (const voxel::Block block : value.m_Data)
		data.Emplace(block.m_Type);
	Write(strData, data);
}