#include "RenderPCH.h"
#include "Render/RenderOpaqueSystem.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
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
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Math/Matrix.h"
#include "Render/RenderBatch.h"
#include "Render/RenderBatchId.h"
#include "Render/RenderData.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strDebugDepthShader = GUID("2f0111d089594da7a2442b80f21b318f");
	const str::Guid strPhongShader = GUID("ba38d79a20274095b6b45ceca225cb3e");
	const str::Guid strUnlitShader = GUID("a926fe4131fa440b8dd72086d3258b75");

	struct Sort
	{
		bool operator()(const render::BatchId& a, const render::BatchId& b)
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

void render::OpaqueSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	glGenBuffers(1, &m_ColourBuffer);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexParamBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strDebugDepthShader);
	assetManager.RequestAsset(strPhongShader);
	assetManager.RequestAsset(strUnlitShader);
}

void render::OpaqueSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strDebugDepthShader);
	assetManager.ReleaseAsset(strPhongShader);
	assetManager.ReleaseAsset(strUnlitShader);
}

void render::OpaqueSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

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
				const auto& meshTransform = renderView.ReadRequired<eng::TransformComponent>();

				render::BatchId& id = ids.Emplace();
				id.m_Entity = renderView;
				id.m_Depth = math::DistanceSqr(meshTransform.m_Translate, cameraTransform.m_Translate);
				id.m_TextureId = { };
				id.m_ShaderId = strPhongShader;
				id.m_StaticMeshId = meshComponent.m_StaticMesh;
			}
		}

		if (ids.IsEmpty())
			continue;

		// Opaque objects are sorted by shader/mesh/material 
		std::sort(ids.begin(), ids.end(), Sort());

		render::BatchId lastId = ids.GetFirst();
		render::Batch batch;
		render::Data data = { cameraProj, cameraView };

		// Ambient Lights
		{
			for (auto&& view : world.Query<ecs::query::Include<const eng::light::AmbientComponent>>())
			{
				const auto& lightComponent = view.ReadRequired<eng::light::AmbientComponent>();
				data.m_LightAmbient_Colour.Append(lightComponent.m_Colour);
			}
		}

		// Directional Lights
		{
			for (auto&& view : world.Query<ecs::query::Include<const eng::light::DirectionalComponent, const eng::TransformComponent>>())
			{
				const auto& lightComponent = view.ReadRequired<eng::light::DirectionalComponent>();
				const auto& lightTransform = view.ReadRequired<eng::TransformComponent>();
				const Matrix3x3 rotation = Matrix3x3::FromRotate(lightTransform.m_Rotate);

				data.m_LightDirectional_Colour.Append(lightComponent.m_Colour);
				data.m_LightDirectional_Direction.Append(Vector3f::AxisZ * rotation);
			}
		}

		// Point Lights
		{
			for (auto&& view : world.Query<ecs::query::Include<const eng::light::PointComponent, const eng::TransformComponent>>())
			{
				const auto& lightComponent = view.ReadRequired<eng::light::PointComponent>();
				const auto& lightTransform = view.ReadRequired<eng::TransformComponent>();

				data.m_LightPoint_Range.Append(lightComponent.m_Range);
				data.m_LightPoint_Colour.Append(lightComponent.m_Colour);
				data.m_LightPoint_Position.Append(lightTransform.m_Translate);
			}
		}

		for (const render::BatchId& id : ids)
		{
			const bool isSameBatch =
				id.m_ShaderId == lastId.m_ShaderId &&
				id.m_TextureId == lastId.m_TextureId &&
				id.m_StaticMeshId == lastId.m_StaticMeshId;

			if (!isSameBatch)
			{
				RenderBatch(world, lastId, batch, data);
				batch.m_Colours.RemoveAll();
				batch.m_Models.RemoveAll();
				batch.m_TexParams.RemoveAll();
			}

			const auto& meshTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);

			const Matrix4x4 model = meshTransform.ToTransform();

			const Colour& colour = colour::Generate(id.m_Entity);

			batch.m_Colours.Emplace(colour);
			batch.m_Models.Append(model);
			batch.m_TexParams.Append(Vector4f::Zero);

			// do last
			lastId = id;
		}

		RenderBatch(world, lastId, batch, data);
	}
}

void render::OpaqueSystem::RenderBatch(World& world, const render::BatchId& id, const render::Batch& batch, const render::Data& data)
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * instanceCount, &batch.m_TexParams[i], GL_DYNAMIC_DRAW);

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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * instanceCount, &batch.m_Colours[i], GL_DYNAMIC_DRAW);

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

		// Ambient
		{
			if (shader->u_LightAmbient_Colour && !data.m_LightAmbient_Colour.IsEmpty())
			{
				const int32 size = data.m_LightAmbient_Colour.GetCount();
				const uint32 location = *shader->u_LightAmbient_Colour;
				glUniform3fv(location, size, &data.m_LightAmbient_Colour[0].x);
			}
		}

		// Directional
		{
			if (shader->u_LightDirectional_Colour && !data.m_LightDirectional_Colour.IsEmpty())
			{
				const int32 size = data.m_LightDirectional_Colour.GetCount();
				const uint32 location = *shader->u_LightDirectional_Colour;
				glUniform3fv(location, size, &data.m_LightDirectional_Colour[0].x);
			}

			if (shader->u_LightDirectional_Direction && !data.m_LightDirectional_Direction.IsEmpty())
			{
				const int32 size = data.m_LightDirectional_Direction.GetCount();
				const uint32 location = *shader->u_LightDirectional_Direction;
				glUniform3fv(location, size, &data.m_LightDirectional_Direction[0].x);
			}

			if (shader->u_Texture_ShadowMap)
			{
				auto& component = world.WriteSingleton<eng::FrameBufferSingleton>();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, component.m_ShadowTexture);
			}
		}

		// Point
		{
			if (shader->u_LightPoint_Range && !data.m_LightPoint_Range.IsEmpty())
			{
				const int32 size = data.m_LightPoint_Range.GetCount();
				const uint32 location = *shader->u_LightPoint_Range;
				glUniform1fv(location, size, &data.m_LightPoint_Range[0]);
			}

			if (shader->u_LightPoint_Colour && !data.m_LightPoint_Colour.IsEmpty())
			{
				const int32 size = data.m_LightPoint_Colour.GetCount();
				const uint32 location = *shader->u_LightPoint_Colour;
				glUniform3fv(location, size, &data.m_LightPoint_Colour[0].x);
			}

			if (shader->u_LightPoint_Position && !data.m_LightPoint_Position.IsEmpty())
			{
				const int32 size = data.m_LightPoint_Position.GetCount();
				const uint32 location = *shader->u_LightPoint_Position;
				glUniform3fv(location, size, &data.m_LightPoint_Position[0].x);
			}
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

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture.m_TextureId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
