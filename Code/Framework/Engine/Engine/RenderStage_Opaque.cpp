#include "EnginePCH.h"
#include "Engine/RenderStage_Opaque.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/LightComponents.h"
#include "Engine/SettingsComponents.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strDebugDepthShader = GUID("2f0111d089594da7a2442b80f21b318f");
	const str::Guid strPhongShader = GUID("ba38d79a20274095b6b45ceca225cb3e");
	const str::Guid strUnlitShader = GUID("a926fe4131fa440b8dd72086d3258b75");

	struct Sort
	{
		bool operator()(const eng::RenderBatchID& a, const eng::RenderBatchID& b)
		{
			// most expensive -> least
			if (a.m_ShaderId != b.m_ShaderId)
				return a.m_ShaderId < b.m_ShaderId;

			if (a.m_TextureId != b.m_TextureId)
				return a.m_TextureId < b.m_TextureId;

			if (a.m_StaticMeshId != b.m_StaticMeshId)
				return a.m_StaticMeshId < b.m_StaticMeshId;

			// lastly, front to back to
			return a.m_Depth < b.m_Depth;
		}
	};
}

void eng::RenderStage_Opaque::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenBuffers(1, &m_ColourBuffer);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexParamBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strDebugDepthShader);
	assetManager.RequestAsset(strPhongShader);
	assetManager.RequestAsset(strUnlitShader);
}

void eng::RenderStage_Opaque::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strDebugDepthShader);
	assetManager.ReleaseAsset(strPhongShader);
	assetManager.ReleaseAsset(strUnlitShader);
}

void eng::RenderStage_Opaque::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
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
	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const bool isEditorActive = debugSettings.m_IsEditorModeEnabled;
		const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
		if (isEditorActive != isEditorCamera)
			continue;

		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Matrix4x4 cameraProj = camera::GetProjection(cameraComponent.m_Projection, windowSize);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		Array<RenderBatchID> batchIDs;

		// static mesh
		{
			using Query = ecs::query
				::Include<
				eng::StaticMeshComponent,
				eng::TransformComponent>;

			for (const ecs::Entity& renderEntity : world.Query<Query>())
			{
				const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(renderEntity);
				const auto& meshTransform = world.ReadComponent<eng::TransformComponent>(renderEntity);

				RenderBatchID& id = batchIDs.Emplace();
				id.m_Entity = renderEntity;
				id.m_Depth = math::DistanceSqr(meshTransform.m_Translate, cameraTransform.m_Translate);
				id.m_TextureId = { };
				id.m_ShaderId = strPhongShader;
				id.m_StaticMeshId = meshComponent.m_StaticMesh;
			}
		}

		if (batchIDs.IsEmpty())
			continue;

		// Opaque objects are sorted by shader/mesh/material 
		std::sort(batchIDs.begin(), batchIDs.end(), Sort());

		RenderBatchID batchID = batchIDs.GetFirst();
		RenderBatchData batchData;
		RenderStageData stageData = { cameraProj, cameraView };

		// Ambient Lights
		{
			for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::LightAmbientComponent>>())
			{
				const auto& lightComponent = world.ReadComponent<eng::LightAmbientComponent>(entity);
				stageData.m_LightAmbient_Colour.Append(lightComponent.m_Colour);
			}
		}

		// Directional Lights
		{
			for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::LightDirectionalComponent, const eng::TransformComponent>>())
			{
				const auto& lightComponent = world.ReadComponent<eng::LightDirectionalComponent>(entity);
				const auto& lightTransform = world.ReadComponent<eng::TransformComponent>(entity);
				const Matrix3x3 rotation = Matrix3x3::FromRotate(lightTransform.m_Rotate);

				stageData.m_LightDirectional_Colour.Append(lightComponent.m_Colour);
				stageData.m_LightDirectional_Direction.Append(Vector3f::AxisZ * rotation);
			}
		}

		// Point Lights
		{
			for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::LightPointComponent, const eng::TransformComponent>>())
			{
				const auto& lightComponent = world.ReadComponent<eng::LightPointComponent>(entity);
				const auto& lightTransform = world.ReadComponent<eng::TransformComponent>(entity);

				stageData.m_LightPoint_Range.Append(lightComponent.m_Range);
				stageData.m_LightPoint_Colour.Append(lightComponent.m_Colour);
				stageData.m_LightPoint_Position.Append(lightTransform.m_Translate);
			}
		}

		for (const RenderBatchID& id : batchIDs)
		{
			const bool isSameBatch =
				id.m_ShaderId == batchID.m_ShaderId &&
				id.m_TextureId == batchID.m_TextureId &&
				id.m_StaticMeshId == batchID.m_StaticMeshId;

			if (!isSameBatch)
			{
				RenderBatch(world, batchID, batchData, stageData);
				batchData.m_Colours.RemoveAll();
				batchData.m_Models.RemoveAll();
				batchData.m_TexParams.RemoveAll();
			}

			const auto& meshTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);

			const Matrix4x4 model = meshTransform.ToTransform();

			const Colour& colour = colour::Generate(id.m_Entity);

			batchData.m_Colours.Emplace(colour);
			batchData.m_Models.Append(model);
			batchData.m_TexParams.Append(Vector4f::Zero);

			// do last
			batchID = id;
		}

		RenderBatch(world, batchID, batchData, stageData);
	}
}

void eng::RenderStage_Opaque::RenderBatch(World& world, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.FetchAsset<eng::StaticMeshAsset>(batchID.m_StaticMeshId);
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(batchID.m_ShaderId);
	if (!mesh || !shader)
		return;

	glUseProgram(shader->m_ProgramId);

	{
		const int32 i = 0;
		const int32 instanceCount = batchData.m_Models.GetCount();
		const auto& binding = mesh->m_Binding;

		glBindVertexArray(binding.m_AttributeObject);

		// vertices
		// indices
		if (shader->a_Vertex)
		{
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.m_IndexBuffer);

			const uint32 location = *shader->a_Vertex;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
			glVertexAttribDivisor(location, GL_FALSE);
		}

		// normals
		if (shader->a_Normal)
		{
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_NormalBuffer);

			const uint32 location = *shader->a_Normal;
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

		// texparams
		if (shader->i_TexParams)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead

			glBindBuffer(GL_ARRAY_BUFFER, m_TexParamBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * instanceCount, &batchData.m_TexParams[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader->i_TexParams;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), (void*)(0));
			glVertexAttribDivisor(location, GL_TRUE);
		}

		// colours
		if (shader->i_Colour)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead

			glBindBuffer(GL_ARRAY_BUFFER, m_ColourBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * instanceCount, &batchData.m_Colours[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader->i_Colour;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(0));
			glVertexAttribDivisor(location, GL_TRUE);
		}

		// models
		if (shader->i_Model)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead
			// #todo: glMapBuffer ?

			glBindBuffer(GL_ARRAY_BUFFER, m_ModelBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceCount, &batchData.m_Models[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader->i_Model;
			glEnableVertexAttribArray(location + 0);
			glEnableVertexAttribArray(location + 1);
			glEnableVertexAttribArray(location + 2);
			glEnableVertexAttribArray(location + 3);
			glVertexAttribPointer(location + 0, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(sizeof(Vector4f) * 0));
			glVertexAttribPointer(location + 1, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(sizeof(Vector4f) * 1));
			glVertexAttribPointer(location + 2, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(sizeof(Vector4f) * 2));
			glVertexAttribPointer(location + 3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4x4), (void*)(sizeof(Vector4f) * 3));
			glVertexAttribDivisor(location + 0, GL_TRUE);
			glVertexAttribDivisor(location + 1, GL_TRUE);
			glVertexAttribDivisor(location + 2, GL_TRUE);
			glVertexAttribDivisor(location + 3, GL_TRUE);
		}

		// Ambient
		{
			if (shader->u_LightAmbient_Colour && !stageData.m_LightAmbient_Colour.IsEmpty())
			{
				const int32 size = stageData.m_LightAmbient_Colour.GetCount();
				const uint32 location = *shader->u_LightAmbient_Colour;
				glUniform3fv(location, size, &stageData.m_LightAmbient_Colour[0].x);
			}
		}

		// Directional
		{
			if (shader->u_LightDirectional_Colour && !stageData.m_LightDirectional_Colour.IsEmpty())
			{
				const int32 size = stageData.m_LightDirectional_Colour.GetCount();
				const uint32 location = *shader->u_LightDirectional_Colour;
				glUniform3fv(location, size, &stageData.m_LightDirectional_Colour[0].x);
			}

			if (shader->u_LightDirectional_Direction && !stageData.m_LightDirectional_Direction.IsEmpty())
			{
				const int32 size = stageData.m_LightDirectional_Direction.GetCount();
				const uint32 location = *shader->u_LightDirectional_Direction;
				glUniform3fv(location, size, &stageData.m_LightDirectional_Direction[0].x);
			}

			if (shader->u_Texture_ShadowMap)
			{
				auto& component = world.WriteSingleton<eng::FrameBufferComponent>();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, component.m_ShadowTexture);
			}
		}

		// Point
		{
			if (shader->u_LightPoint_Range && !stageData.m_LightPoint_Range.IsEmpty())
			{
				const int32 size = stageData.m_LightPoint_Range.GetCount();
				const uint32 location = *shader->u_LightPoint_Range;
				glUniform1fv(location, size, &stageData.m_LightPoint_Range[0]);
			}

			if (shader->u_LightPoint_Colour && !stageData.m_LightPoint_Colour.IsEmpty())
			{
				const int32 size = stageData.m_LightPoint_Colour.GetCount();
				const uint32 location = *shader->u_LightPoint_Colour;
				glUniform3fv(location, size, &stageData.m_LightPoint_Colour[0].x);
			}

			if (shader->u_LightPoint_Position && !stageData.m_LightPoint_Position.IsEmpty())
			{
				const int32 size = stageData.m_LightPoint_Position.GetCount();
				const uint32 location = *shader->u_LightPoint_Position;
				glUniform3fv(location, size, &stageData.m_LightPoint_Position[0].x);
			}
		}

		if (shader->u_CameraProj)
		{
			const uint32 location = *shader->u_CameraProj;
			glUniformMatrix4fv(location, 1, false, &stageData.m_CameraProj.m_Data[0][0]);
		}

		if (shader->u_CameraView)
		{
			const uint32 location = *shader->u_CameraView;
			glUniformMatrix4fv(location, 1, false, &stageData.m_CameraView.m_Data[0][0]);
		}

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture.m_TextureId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
