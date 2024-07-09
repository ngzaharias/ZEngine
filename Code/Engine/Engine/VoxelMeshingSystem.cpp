#include "EnginePCH.h"
#include "Engine/VoxelMeshingSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/VoxelComponents.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr float s_VertA = 0.f;
	constexpr float s_VertB = voxel::s_BlockSize1D;

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
		constexpr int32 s_Count1D = voxel::s_ChunkSize1D;
		constexpr int32 s_Count2D = voxel::s_ChunkSize2D;
		return innerPos.x
			+ innerPos.y * s_Count1D
			+ innerPos.z * s_Count2D;
	}

	Vector3i ToInnerPos(const int32 innerIndex)
	{
		constexpr int32 s_Count1D = voxel::s_ChunkSize1D;
		constexpr int32 s_Count2D = voxel::s_ChunkSize2D;
		return Vector3i(
			innerIndex % s_Count1D,
			(innerIndex / s_Count1D) % s_Count1D,
			(innerIndex / s_Count2D) % s_Count1D);
	}

	Vector3i ToInnerPos(Vector3i innerPos, const voxel::EDirection direction)
	{
		switch (direction)
		{
		case voxel::EDirection::XNeg:
			innerPos.x--;
			break;
		case voxel::EDirection::XPos:
			innerPos.x++;
			break;
		case voxel::EDirection::YNeg:
			innerPos.y--;
			break;
		case voxel::EDirection::YPos:
			innerPos.y++;
			break;
		case voxel::EDirection::ZNeg:
			innerPos.z--;
			break;
		case voxel::EDirection::ZPos:
			innerPos.z++;
			break;
		}

		return innerPos;
	}

	bool HasBlock(const Vector3i& innerPos, const Array<voxel::Block>& data)
	{
		using namespace voxel;

		if (innerPos.x < 0 || innerPos.y < 0 || innerPos.z < 0)
			return false;
		if (innerPos.x >= s_ChunkSize1D || innerPos.y >= s_ChunkSize1D || innerPos.z >= s_ChunkSize1D)
			return false;

		const int32 innerIndex = ToInnerIndex(innerPos);
		const EType type = data[innerIndex].m_Type;
		return type != voxel::EType::None;
	}
}

void voxel::MeshingSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	Set<ecs::Entity> entitiesToUpdate;

	for (const ecs::Entity& entity : world.Query<ecs::query::Include<const voxel::ChunkComponent>::Exclude<const eng::DynamicMeshComponent>>())
		world.AddComponent<eng::DynamicMeshComponent>(entity);

	using ChunkAddedQuery = ecs::query::Added<const voxel::ChunkComponent>::Include<const eng::DynamicMeshComponent>;
	using MeshAddedQuery = ecs::query::Added<const eng::DynamicMeshComponent>::Include<const voxel::ChunkComponent>;
	using ChangedQuery = ecs::query::Include<const eng::DynamicMeshComponent, const voxel::ChunkComponent, const voxel::ChunkChangedEventComponent>;
	using LoadedQuery = ecs::query::Include<const eng::DynamicMeshComponent, const voxel::ChunkComponent, const voxel::ChunkLoadedEventComponent>;

	for (const ecs::Entity& entity : world.Query<ChunkAddedQuery>())
		entitiesToUpdate.Add(entity);
	for (const ecs::Entity& entity : world.Query<MeshAddedQuery>())
		entitiesToUpdate.Add(entity);
	for (const ecs::Entity& entity : world.Query<ChangedQuery>())
		entitiesToUpdate.Add(entity);
	for (const ecs::Entity& entity : world.Query<LoadedQuery>())
		entitiesToUpdate.Add(entity);

	for (const ecs::Entity& entity : entitiesToUpdate)
	{
		const auto& chunkComponent = world.GetComponent<const voxel::ChunkComponent>(entity);
		auto& meshComponent = world.GetComponent<eng::DynamicMeshComponent>(entity);

		for (auto&& [i, block] : enumerate::Forward(chunkComponent.m_Data))
		{
			if (block.m_Type == EType::None)
				continue;

			const Vector3i innerPos = ToInnerPos(i);
			const Vector3f vertexOffset = Vector3f(
				innerPos.x * s_BlockSize1D,
				innerPos.y * s_BlockSize1D,
				innerPos.z * s_BlockSize1D);

			Vector2f texCoordOffset = Vector2f::Zero;
			switch (block.m_Type)
			{
			case EType::Black:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.x += s_TexCoordB.x;
				break;

			case EType::Green:
				texCoordOffset.x += s_TexCoordB.x;
				break;

			case EType::Grey:
				break;

			case EType::Orange:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.y += s_TexCoordB.y;
				break;

			case EType::Purple:
				texCoordOffset.x += s_TexCoordB.x;
				texCoordOffset.y += s_TexCoordB.y;
				break;

			case EType::Red:
				texCoordOffset.y += s_TexCoordB.y;
				break;
			}

			const Vector3i innerPos_XNeg = ToInnerPos(innerPos, EDirection::XNeg);
			if (!HasBlock(innerPos_XNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesXNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_XPos = ToInnerPos(innerPos, EDirection::XPos);
			if (!HasBlock(innerPos_XPos, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesXPos);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_YNeg = ToInnerPos(innerPos, EDirection::YNeg);
			if (!HasBlock(innerPos_YNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesYNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_YPos = ToInnerPos(innerPos, EDirection::YPos);
			if (!HasBlock(innerPos_YPos, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesYPos);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_ZNeg = ToInnerPos(innerPos, EDirection::ZNeg);
			if (!HasBlock(innerPos_ZNeg, chunkComponent.m_Data))
			{
				meshComponent.m_Vertices.Append(s_VerticesZNeg);
				SetOffset(vertexOffset, meshComponent.m_Vertices);

				meshComponent.m_TexCoords.Append(s_TexCoords);
				SetOffset(texCoordOffset, meshComponent.m_TexCoords);
			}

			const Vector3i innerPos_ZPos = ToInnerPos(innerPos, EDirection::ZPos);
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
