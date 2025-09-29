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
#include "Engine/SettingsComponents.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Voxel/VoxelComponents.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strVoxelShader = GUID("fffaa79e28e044d9b515daef50fc27d2");
	const str::Guid strVoxelTexture = GUID("f87d23dd5e7b4d6dbff88b0eb676f80c");
}

void eng::RenderStage_Voxels::Initialise(ecs::EntityWorld& entityWorld)
{
	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::ShaderAsset>(strVoxelShader);
	assetManager.RequestAsset<eng::Texture2DAsset>(strVoxelTexture);
}

void eng::RenderStage_Voxels::Shutdown(ecs::EntityWorld& entityWorld)
{
	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::ShaderAsset>(strVoxelShader);
	assetManager.ReleaseAsset<eng::Texture2DAsset>(strVoxelTexture);
}

void eng::RenderStage_Voxels::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(strVoxelShader);
	const auto* texture = assetManager.FetchAsset<eng::Texture2DAsset>(strVoxelTexture);
	if (!shader || !texture)
		return;

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<eng::camera::ProjectionComponent, eng::TransformComponent>>())
	{
		const bool isEditorActive = debugSettings.m_IsEditorModeEnabled;
		const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
		if (isEditorActive != isEditorCamera)
			continue;

		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Matrix4x4 cameraProj = camera::GetProjection(cameraComponent.m_Projection, windowSize);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		glUseProgram(shader->m_ProgramId);

		for (const ecs::Entity& voxelEntity : world.Query<ecs::query::Include<eng::DynamicMeshComponent, eng::TransformComponent, voxel::ChunkComponent>>())
		{
			const auto& voxelComponent = world.ReadComponent<voxel::ChunkComponent>(voxelEntity);
			const auto& voxelDynamicMesh = world.ReadComponent<eng::DynamicMeshComponent>(voxelEntity);
			const auto& voxelTransform = world.ReadComponent<eng::TransformComponent>(voxelEntity);

			const Matrix4x4 voxelModel = voxelTransform.ToTransform();

			const auto& binding = voxelDynamicMesh.m_Binding;

			glBindVertexArray(binding.m_AttributeObject);

			// vertices
			if (shader->a_Vertex)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);

				const uint32 location = *shader->a_Vertex;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			// texcoords
			if (shader->a_TexCoords)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);

				const uint32 location = *shader->a_TexCoords;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			if (shader->u_CameraProj)
			{
				const uint32 location = *shader->u_CameraProj;
				glUniformMatrix4fv(location, 1, false, &cameraProj.m_Data[0][0]);
			}

			if (shader->u_CameraView)
			{
				const uint32 location = *shader->u_CameraView;
				glUniformMatrix4fv(location, 1, false, &cameraView.m_Data[0][0]);
			}

			if (shader->u_Transform)
			{
				const uint32 location = *shader->u_Transform;
				glUniformMatrix4fv(location, 1, false, &voxelModel.m_Data[0][0]);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
			glDrawArrays(GL_TRIANGLES, 0, voxelDynamicMesh.m_Vertices.GetCount());

#ifdef ASSERT_RENDER
			Z_PANIC(voxelModel.IsValid(), "Invalid matrix for entity model!");
#endif
		}
	}
}
