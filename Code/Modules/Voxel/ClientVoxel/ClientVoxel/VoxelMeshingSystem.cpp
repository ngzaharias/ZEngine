#include "VoxelPCH.h"
#include "ClientVoxel/VoxelMeshingSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/DynamicMeshComponent.h"
#include "SharedVoxel/VoxelChunkComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr float s_VertA = 0.f;
	constexpr float s_VertB = shared::voxel::s_BlockSize1D;

	const Array<Vector3f> s_VerticesXNeg = {
		Vector3f(s_VertA, s_VertA, s_VertB),
		Vector3f(s_VertA, s_VertB, s_VertB),
		Vector3f(s_VertA, s_VertB, s_VertA),
		Vector3f(s_VertA, s_VertB, s_VertA),
		Vector3f(s_VertA, s_VertA, s_VertA),
		Vector3f(s_VertA, s_VertA, s_VertB) };
	const Array<Vector3f> s_VerticesXPos = {
		Vector3f(s_VertB, s_VertA, s_VertA),
		Vector3f(s_VertB, s_VertB, s_VertA),
		Vector3f(s_VertB, s_VertB, s_VertB),
		Vector3f(s_VertB, s_VertB, s_VertB),
		Vector3f(s_VertB, s_VertA, s_VertB),
		Vector3f(s_VertB, s_VertA, s_VertA) };
	const Array<Vector3f> s_VerticesYNeg = {
		Vector3f(s_VertA, s_VertA, s_VertB),
		Vector3f(s_VertA, s_VertA, s_VertA),
		Vector3f(s_VertB, s_VertA, s_VertA),
		Vector3f(s_VertB, s_VertA, s_VertA),
		Vector3f(s_VertB, s_VertA, s_VertB),
		Vector3f(s_VertA, s_VertA, s_VertB) };
	const Array<Vector3f> s_VerticesYPos = {
		Vector3f(s_VertA, s_VertB, s_VertA),
		Vector3f(s_VertA, s_VertB, s_VertB),
		Vector3f(s_VertB, s_VertB, s_VertB),
		Vector3f(s_VertB, s_VertB, s_VertB),
		Vector3f(s_VertB, s_VertB, s_VertA),
		Vector3f(s_VertA, s_VertB, s_VertA) };
	const Array<Vector3f> s_VerticesZNeg = {
		Vector3f(s_VertA, s_VertA, s_VertA),
		Vector3f(s_VertA, s_VertB, s_VertA),
		Vector3f(s_VertB, s_VertB, s_VertA),
		Vector3f(s_VertB, s_VertB, s_VertA),
		Vector3f(s_VertB, s_VertA, s_VertA),
		Vector3f(s_VertA, s_VertA, s_VertA) };
	const Array<Vector3f> s_VerticesZPos = {
		Vector3f(s_VertB, s_VertA, s_VertB),
		Vector3f(s_VertB, s_VertB, s_VertB),
		Vector3f(s_VertA, s_VertB, s_VertB),
		Vector3f(s_VertA, s_VertB, s_VertB),
		Vector3f(s_VertA, s_VertA, s_VertB),
		Vector3f(s_VertB, s_VertA, s_VertB) };

	constexpr Vector2f s_TexCoordA = Vector2f::Zero;
	constexpr Vector2f s_TexCoordB = Vector2f(1.f / 3.f, 1.f / 2.f);
	const Array<Vector2f> s_TexCoords = {
		Vector2f(s_TexCoordA.x, s_TexCoordA.y),
		Vector2f(s_TexCoordA.x, s_TexCoordB.y),
		Vector2f(s_TexCoordB.x, s_TexCoordB.y),
		Vector2f(s_TexCoordB.x, s_TexCoordB.y),
		Vector2f(s_TexCoordB.x, s_TexCoordA.y),
		Vector2f(s_TexCoordA.x, s_TexCoordA.y) };

	template<typename Vector>
	void SetOffset(const Vector& offset, Array<Vector>& inout_Vertices)
	{
		const int32 count = inout_Vertices.GetCount();
		const int32 first = count - 6;

		for (int32 i = first; i < count; ++i)
			inout_Vertices[i] += offset;
	}

	int32 ToInnerIndex(const Vector3i& innerPos)
	{
		constexpr int32 s_Count1D = shared::voxel::s_BlockCount1D;
		constexpr int32 s_Count2D = shared::voxel::s_BlockCount2D;
		return innerPos.x
			+ innerPos.y * s_Count1D
			+ innerPos.z * s_Count2D;
	}

	Vector3i ToInnerPos(const int32 innerIndex)
	{
		constexpr int32 s_Count1D = shared::voxel::s_BlockCount1D;
		constexpr int32 s_Count2D = shared::voxel::s_BlockCount2D;
		return Vector3i(
			innerIndex % s_Count1D,
			(innerIndex / s_Count1D) % s_Count1D,
			(innerIndex / s_Count2D) % s_Count1D);
	}

	Vector3i ToNeighbourPos(Vector3i innerPos, const shared::voxel::EDirection direction)
	{
		switch (direction)
		{
		case shared::voxel::EDirection::XNeg:
			innerPos.x--;
			break;
		case shared::voxel::EDirection::XPos:
			innerPos.x++;
			break;
		case shared::voxel::EDirection::YNeg:
			innerPos.y--;
			break;
		case shared::voxel::EDirection::YPos:
			innerPos.y++;
			break;
		case shared::voxel::EDirection::ZNeg:
			innerPos.z--;
			break;
		case shared::voxel::EDirection::ZPos:
			innerPos.z++;
			break;
		}

		return innerPos;
	}

	bool HasBlock(const Vector3i& innerPos, const Array<shared::voxel::Block>& data)
	{
		using namespace shared::voxel;

		if (innerPos.x < 0 || innerPos.y < 0 || innerPos.z < 0)
			return false;
		if (innerPos.x >= s_BlockCount1D || innerPos.y >= s_BlockCount1D || innerPos.z >= s_BlockCount1D)
			return false;

		const int32 innerIndex = ToInnerIndex(innerPos);
		const EType type = data[innerIndex].m_Type;
		return type != shared::voxel::EType::None;
	}
}

void client::voxel::MeshingSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	for (auto&& view : world.Query<ecs::query::Include<const shared::voxel::ChunkComponent>::Exclude<const eng::DynamicMeshComponent>>())
		world.AddComponent<eng::DynamicMeshComponent>(view);

	using ChunkAddedQuery = ecs::query::Added<const shared::voxel::ChunkComponent>::Include<const eng::DynamicMeshComponent>;
	using MeshAddedQuery = ecs::query::Added<const eng::DynamicMeshComponent>::Include<const shared::voxel::ChunkComponent>;

	Set<ecs::Entity> entitiesToUpdate;
	entitiesToUpdate.Add(world.Query<ChunkAddedQuery>());
	entitiesToUpdate.Add(world.Query<MeshAddedQuery>());
	for (const ecs::Entity& entity : entitiesToUpdate)
	{
		const auto& chunkComponent = world.ReadComponent<shared::voxel::ChunkComponent>(entity);
		auto& meshComponent = world.WriteComponent<eng::DynamicMeshComponent>(entity);
		meshComponent.m_Indices.RemoveAll();
		meshComponent.m_Normals.RemoveAll();
		meshComponent.m_TexCoords.RemoveAll();
		meshComponent.m_Vertices.RemoveAll();

		for (auto&& [i, block] : enumerate::Forward(chunkComponent.m_Data))
		{
			if (block.m_Type == shared::voxel::EType::None)
				continue;

			const Vector3i innerPos = ToInnerPos(i);
			const Vector3f vertexOffset = Vector3f(
				innerPos.x * shared::voxel::s_BlockSize1D,
				innerPos.y * shared::voxel::s_BlockSize1D,
				innerPos.z * shared::voxel::s_BlockSize1D);

			Vector2f texCoordOffset = Vector2f::Zero;
			switch (block.m_Type)
			{
			case shared::voxel::EType::Black:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.x += s_TexCoordB.x;
				break;

			case shared::voxel::EType::Green:
				texCoordOffset.x += s_TexCoordB.x;
				break;

			case shared::voxel::EType::Grey:
				break;

			case shared::voxel::EType::Orange:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.y += s_TexCoordB.y;
				break;

			case shared::voxel::EType::Purple:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.y += s_TexCoordB.y;
				break;

			case shared::voxel::EType::Red:
				texCoordOffset.y += s_TexCoordB.y;
				break;
			}

			const Vector3i innerPos_XNeg = ToNeighbourPos(innerPos, shared::voxel::EDirection::XNeg);
			if (!HasBlock(innerPos_XNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesXNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_XPos = ToNeighbourPos(innerPos, shared::voxel::EDirection::XPos);
			if (!HasBlock(innerPos_XPos, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesXPos);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_YNeg = ToNeighbourPos(innerPos, shared::voxel::EDirection::YNeg);
			if (!HasBlock(innerPos_YNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesYNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_YPos = ToNeighbourPos(innerPos, shared::voxel::EDirection::YPos);
			if (!HasBlock(innerPos_YPos, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesYPos);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_ZNeg = ToNeighbourPos(innerPos, shared::voxel::EDirection::ZNeg);
			if (!HasBlock(innerPos_ZNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesZNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_ZPos = ToNeighbourPos(innerPos, shared::voxel::EDirection::ZPos);
			if (!HasBlock(innerPos_ZPos, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesZPos);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}
		}

		// generate the bindings
		if (meshComponent.m_Binding.m_AttributeObject == 0)
		{
			auto& binding = meshComponent.m_Binding;
			glGenVertexArrays(1, &binding.m_AttributeObject);
			glGenBuffers(1, &binding.m_VertexBuffer);
			glGenBuffers(1, &binding.m_TexCoordBuffer);
			glGenBuffers(1, &binding.m_NormalBuffer);
			glGenBuffers(1, &binding.m_IndexBuffer);
		}

		// update the buffers
		{
			auto& binding = meshComponent.m_Binding;
			auto& vertices = meshComponent.m_Vertices;
			auto& texcoords = meshComponent.m_TexCoords;
			auto& normals = meshComponent.m_Normals;
			auto& indices = meshComponent.m_Indices;

			glBindVertexArray(binding.m_AttributeObject);

			// vertex
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * vertices.GetCount(), vertices.GetData(), GL_DYNAMIC_DRAW);

			// texture coordinates
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * texcoords.GetCount(), texcoords.GetData(), GL_DYNAMIC_DRAW);

			// normal
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_NormalBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * normals.GetCount(), normals.GetData(), GL_DYNAMIC_DRAW);

			// index
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.m_IndexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.GetCount(), indices.GetData(), GL_DYNAMIC_DRAW);

			// unbind when we're done to avoid accidentally writing to these buffers
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}
