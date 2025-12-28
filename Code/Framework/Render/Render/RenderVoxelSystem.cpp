#include "RenderPCH.h"
#include "Render/RenderVoxelSystem.h"

#include "Core/Algorithms.h"
#include "Core/Guid.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/DynamicMeshComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/ShaderAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Matrix.h"
#include "Voxel/VoxelChunkComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strVoxelShader = GUID("fffaa79e28e044d9b515daef50fc27d2");
	const str::Guid strVoxelTexture = GUID("f87d23dd5e7b4d6dbff88b0eb676f80c");
}

void render::VoxelSystem::Initialise(World& world)
{
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strVoxelShader);
	assetManager.RequestAsset(strVoxelTexture);
}

void render::VoxelSystem::Shutdown(World& world)
{
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strVoxelShader);
	assetManager.ReleaseAsset(strVoxelTexture);
}

void render::VoxelSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(strVoxelShader);
	const auto* texture = assetManager.ReadAsset<eng::Texture2DAsset>(strVoxelTexture);
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

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	
	using CameraQuery = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;
	for (auto&& cameraView : world.Query<CameraQuery>())
	{
		const auto& cameraComponent = cameraView.ReadRequired<eng::CameraComponent>();
		const auto& cameraTransform = cameraView.ReadRequired<eng::TransformComponent>();

		const Matrix4x4 cameraProj = eng::GetProjection(cameraComponent.m_Projection, windowSize);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		glUseProgram(shader->m_ProgramId);

		using RenderQuery = ecs::query
			::Include<
			const eng::DynamicMeshComponent,
			const eng::TransformComponent,
			const voxel::ChunkComponent>;
		for (auto&& renderView : world.Query<RenderQuery>())
		{
			const auto& voxelComponent = renderView.ReadRequired<voxel::ChunkComponent>();
			const auto& voxelDynamicMesh = renderView.ReadRequired<eng::DynamicMeshComponent>();
			const auto& voxelTransform = renderView.ReadRequired<eng::TransformComponent>();

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
