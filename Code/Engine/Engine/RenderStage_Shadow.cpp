#include "EnginePCH.h"
#include "Engine/RenderStage_Shadow.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/FrameBufferComponent.h"
#include "Engine/LightComponents.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TransformComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strDepthShader = GUID("5ab1ac45783b4a54aa9892f6e36bc639");

	struct Sort
	{
		bool operator()(const eng::RenderBatchID& a, const eng::RenderBatchID& b)
		{
			return a.m_StaticMeshId < b.m_StaticMeshId;
		}
	};
}

void eng::RenderStage_Shadow::Initialise(ecs::EntityWorld& entityWorld)
{
	World world = entityWorld.GetWorldView<World>();

	// texture and buffer
	{
		auto& bufferComponent = world.WriteSingleton<eng::FrameBufferComponent>();
		bufferComponent.m_ShadowSize = Vector2u(1024, 1024);

		glGenFramebuffers(1, &bufferComponent.m_ShadowBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferComponent.m_ShadowBuffer);

		glGenTextures(1, &bufferComponent.m_ShadowTexture);
		glBindTexture(GL_TEXTURE_2D, bufferComponent.m_ShadowTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, bufferComponent.m_ShadowSize.x, bufferComponent.m_ShadowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferComponent.m_ShadowTexture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// instance attributes
	{
		glGenBuffers(1, &m_ModelBuffer);
	}
}

void eng::RenderStage_Shadow::Shutdown(ecs::EntityWorld& entityWorld)
{
	auto& bufferComponent = entityWorld.WriteSingleton<eng::FrameBufferComponent>();

	glDeleteFramebuffers(1, &bufferComponent.m_ShadowBuffer);
	glDeleteTextures(1, &bufferComponent.m_ShadowTexture);
	glDeleteBuffers(1, &m_ModelBuffer);
}

void eng::RenderStage_Shadow::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();

	const auto& bufferComponent = entityWorld.ReadSingleton<eng::FrameBufferComponent>();

	{
		glViewport(0, 0, bufferComponent.m_ShadowSize.x, bufferComponent.m_ShadowSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferComponent.m_ShadowBuffer);

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		// the depth mask must be enabled BEFORE clearing the depth buffer
		// the frame buffer must be bound BEFORE clearing it
		glClearDepthf(1.f);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);
		const Matrix3x3 cameraRotate = Matrix3x3::FromRotate(cameraTransform.m_Rotate);
		const Vector3f cameraFoward = Vector3f::AxisZ * cameraRotate;

		for (const ecs::Entity& lightEntity : world.Query<ecs::query::Include<const eng::LightDirectionalComponent, const eng::TransformComponent>>())
		{
			const auto& lightComponent = world.ReadComponent<eng::LightDirectionalComponent>(lightEntity);
			const auto& lightTransform = world.ReadComponent<eng::TransformComponent>(lightEntity);

			camera::Orthographic orthographic;

			const float distForward = math::Lerp(orthographic.m_ClippingNear, orthographic.m_ClippingFar, 0.5f);
			const Matrix3x3 lightRotate = Matrix3x3::FromRotate(lightTransform.m_Rotate);
			const Vector3f lightFoward = Vector3f::AxisZ * lightRotate;

			const Vector3f offset = (cameraFoward * 1000.f) + (-lightFoward * distForward);
			const Vector3f translate = cameraTransform.m_Translate + offset;

			const Matrix4x4 lightProj = camera::GetProjection(bufferComponent.m_ShadowSize, orthographic);
			const Matrix4x4 lightView = lightTransform.ToTransform().Inversed();

			Array<RenderBatchID> batchIDs;

			// static mesh
			{
				for (const ecs::Entity& renderEntity : world.Query<ecs::query::Include<const eng::StaticMeshComponent, const eng::TransformComponent>>())
				{
					const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(renderEntity);
					if (!meshComponent.m_StaticMesh.IsValid())
						continue;

					RenderBatchID& id = batchIDs.Emplace();
					id.m_Entity = renderEntity;
					id.m_ShaderId = strDepthShader;
					id.m_StaticMeshId = meshComponent.m_StaticMesh;
				}
			}

			if (batchIDs.IsEmpty())
				continue;

			std::sort(batchIDs.begin(), batchIDs.end(), Sort());

			RenderBatchID batchID = batchIDs.GetFirst();
			RenderBatchData batchData;
			RenderStageData stageData = { lightProj, lightView };

			for (const RenderBatchID& id : batchIDs)
			{
				const bool isSameBatch =
					id.m_ShaderId == batchID.m_ShaderId &&
					id.m_StaticMeshId == batchID.m_StaticMeshId;

				if (!isSameBatch)
				{
					RenderBatch(world, batchID, batchData, stageData);
					batchData.m_Models.RemoveAll();
				}

				const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(id.m_Entity);
				const auto& meshTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);
				const auto* meshAsset = assetManager.FetchAsset<eng::StaticMeshAsset>(meshComponent.m_StaticMesh);
				if (!meshAsset)
					continue;

				const Matrix4x4 model = meshTransform.ToTransform();
				batchData.m_Models.Append(model);

				// do last
				batchID = id;
			}

			RenderBatch(world, batchID, batchData, stageData);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void eng::RenderStage_Shadow::RenderBatch(World& world, const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData)
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

		// models
		if (shader->i_Model)
		{
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

		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
