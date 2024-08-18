#include "EnginePCH.h"
#include "Engine/RenderStage_Voxels.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/VoxelComponents.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strVoxelMesh = GUID("37cb9789-ccec-4a51-9297-fd6472e53d7a");
	const str::Guid strVoxelShader = GUID("fffaa79e-28e0-44d9-b515-daef50fc27d2");
	const str::Guid strVoxelTexture = GUID("f87d23dd-5e7b-4d6d-bff8-8b0eb676f80c");
}

void eng::RenderStage_Voxels::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	auto& assetManager = world.GetResource<eng::AssetManager>();

	{
		glViewport(0, 0, static_cast<int32>(Screen::width), static_cast<int32>(Screen::height));

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::CameraComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.GetComponent<const eng::CameraComponent>(cameraEntity);
		const auto& cameraTransform = world.GetComponent<const eng::TransformComponent>(cameraEntity);

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraProj = camera::GetProjection(screenSize, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		const auto& shader = *assetManager.LoadAsset<eng::ShaderAsset>(strVoxelShader);
		const auto& texture = *assetManager.LoadAsset<eng::Texture2DAsset>(strVoxelTexture);

		glUseProgram(shader.m_ProgramId);

		for (const ecs::Entity& voxelEntity : world.Query<ecs::query::Include<const eng::DynamicMeshComponent, const eng::TransformComponent>>())
		{
			const auto& voxelComponent = world.GetComponent<const voxel::ChunkComponent>(voxelEntity);
			const auto& voxelDynamicMesh = world.GetComponent<const eng::DynamicMeshComponent>(voxelEntity);
			const auto& voxelTransform = world.GetComponent<const eng::TransformComponent>(voxelEntity);

			const Matrix4x4 voxelModel = voxelTransform.ToTransform();

			const auto& binding = voxelDynamicMesh.m_Binding;

			glBindVertexArray(binding.m_AttributeObject);

			// vertices
			if (shader.a_Vertex)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);

				const uint32 location = *shader.a_Vertex;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			// texcoords
			if (shader.a_TexCoords)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);

				const uint32 location = *shader.a_TexCoords;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			if (shader.u_CameraProj)
			{
				const uint32 location = *shader.u_CameraProj;
				glUniformMatrix4fv(location, 1, false, &cameraProj.m_Data[0][0]);
			}

			if (shader.u_CameraView)
			{
				const uint32 location = *shader.u_CameraView;
				glUniformMatrix4fv(location, 1, false, &cameraView.m_Data[0][0]);
			}

			if (shader.u_Transform)
			{
				const uint32 location = *shader.u_Transform;
				glUniformMatrix4fv(location, 1, false, &voxelModel.m_Data[0][0]);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.m_TextureId);
			glDrawArrays(GL_TRIANGLES, 0, voxelDynamicMesh.m_Vertices.GetCount());

#ifdef ASSERT_RENDER
			Z_PANIC(voxelModel.IsValid(), "Invalid matrix for entity model!");
#endif
		}
	}
}
