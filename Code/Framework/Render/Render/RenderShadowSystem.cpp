#include "RenderPCH.h"
#include "Render/RenderShadowSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/FrameBufferSingleton.h"
#include "Engine/LightAmbientComponent.h"
#include "Engine/LightDirectionalComponent.h"
#include "Engine/LightPointComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/StaticMeshComponent.h"
#include "Engine/TransformComponent.h"
#include "Render/RenderBatch.h"
#include "Render/RenderBatchId.h"
#include "Render/RenderData.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strDepthShader = GUID("5ab1ac45783b4a54aa9892f6e36bc639");

	struct Sort
	{
		bool operator()(const render::BatchId& a, const render::BatchId& b)
		{
			return a.m_StaticMeshId < b.m_StaticMeshId;
		}
	};
}

void render::ShadowSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	// texture and buffer
	{
		auto& bufferComponent = world.WriteSingleton<eng::FrameBufferSingleton>();
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

void render::ShadowSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	auto& bufferComponent = world.WriteSingleton<eng::FrameBufferSingleton>();

	glDeleteFramebuffers(1, &bufferComponent.m_ShadowBuffer);
	glDeleteTextures(1, &bufferComponent.m_ShadowTexture);
	glDeleteBuffers(1, &m_ModelBuffer);
}

void render::ShadowSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto& bufferComponent = world.ReadSingleton<eng::FrameBufferSingleton>();

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

	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();

	using CameraQuery = ecs::query
		::Include<
		const eng::ActiveComponent,
		const eng::CameraComponent,
		const eng::TransformComponent>;
	for (auto&& cameraView : world.Query<CameraQuery>())
	{
		const auto& cameraTransform = cameraView.ReadRequired<eng::TransformComponent>();
		const Matrix3x3 cameraRotate = Matrix3x3::FromRotate(cameraTransform.m_Rotate);
		const Vector3f cameraFoward = Vector3f::AxisZ * cameraRotate;

		using LightQuery = ecs::query
			::Include<
			const eng::light::DirectionalComponent, 
			const eng::TransformComponent>;
		for (auto&& lightView : world.Query<LightQuery>())
		{
			const auto& lightComponent = lightView.ReadRequired<eng::light::DirectionalComponent>();
			const auto& lightTransform = lightView.ReadRequired<eng::TransformComponent>();

			eng::Orthographic orthographic;

			const float distForward = math::Lerp(orthographic.m_ClippingNear, orthographic.m_ClippingFar, 0.5f);
			const Matrix3x3 lightRotate = Matrix3x3::FromRotate(lightTransform.m_Rotate);
			const Vector3f lightFoward = Vector3f::AxisZ * lightRotate;

			const Vector3f offset = (cameraFoward * 1000.f) + (-lightFoward * distForward);
			const Vector3f translate = cameraTransform.m_Translate + offset;

			const Matrix4x4 lightProj = eng::GetProjection(orthographic, bufferComponent.m_ShadowSize);
			const Matrix4x4 lightView = lightTransform.ToTransform().Inversed();

			Array<render::BatchId> ids;

			// static mesh
			{
				using RenderQuery = ecs::query
					::Include<
					const eng::StaticMeshComponent,
					const eng::TransformComponent>;
				for (auto&& renderView : world.Query<RenderQuery>())
				{
					const auto& meshComponent = renderView.ReadRequired<eng::StaticMeshComponent>();

					render::BatchId& id = ids.Emplace();
					id.m_Entity = renderView;
					id.m_ShaderId = strDepthShader;
					id.m_StaticMeshId = meshComponent.m_StaticMesh;
				}
			}

			if (ids.IsEmpty())
				continue;

			std::sort(ids.begin(), ids.end(), Sort());

			render::BatchId lastId = ids.GetFirst();
			render::Batch batch;
			render::Data data = { lightProj, lightView };

			for (const render::BatchId& id : ids)
			{
				const bool isSameBatch =
					id.m_ShaderId == lastId.m_ShaderId &&
					id.m_StaticMeshId == lastId.m_StaticMeshId;

				if (!isSameBatch)
				{
					RenderBatch(world, lastId, batch, data);
					batch.m_Models.RemoveAll();
				}

				const auto& meshComponent = world.ReadComponent<eng::StaticMeshComponent>(id.m_Entity);
				const auto& meshTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);
				const auto* meshAsset = assetManager.ReadAsset<eng::StaticMeshAsset>(meshComponent.m_StaticMesh);
				if (!meshAsset)
					continue;

				const Matrix4x4 model = meshTransform.ToTransform();
				batch.m_Models.Append(model);

				// do last
				lastId = id;
			}

			RenderBatch(world, lastId, batch, data);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render::ShadowSystem::RenderBatch(World& world, const render::BatchId& id, const render::Batch& batch, const render::Data& data)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.ReadAsset<eng::StaticMeshAsset>(id.m_StaticMeshId);
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(id.m_ShaderId);
	if (!mesh || !shader)
		return;

	glUseProgram(shader->m_ProgramId);

	{
		const int32 i = 0;
		const int32 instanceCount = batch.m_Models.GetCount();
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceCount, &batch.m_Models[i], GL_DYNAMIC_DRAW);

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
			glUniformMatrix4fv(location, 1, false, &data.m_CameraProj.m_Data[0][0]);
		}

		if (shader->u_CameraView)
		{
			const uint32 location = *shader->u_CameraView;
			glUniformMatrix4fv(location, 1, false, &data.m_CameraView.m_Data[0][0]);
		}

		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
