#include "RenderPCH.h"
#include "Render/RenderTranslucentSystem.h"

#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/VisibilityComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Render/RenderBatch.h"
#include "Render/RenderBatchId.h"
#include "Render/RenderData.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strQuadMesh = str::Guid::Create("e94876a8e4cc4d1684c85859b48a1af6");

	struct Sort
	{
		bool operator()(const render::BatchId& a, const render::BatchId& b)
		{
			// all translucent objects must be sorted by depth first
			if (a.m_Depth != b.m_Depth)
				return a.m_Depth > b.m_Depth;

			if (a.m_ShaderId != b.m_ShaderId)
				return a.m_ShaderId < b.m_ShaderId;

			if (a.m_TextureId != b.m_TextureId)
				return a.m_TextureId < b.m_TextureId;

			return a.m_StaticMeshId < b.m_StaticMeshId;
		}
	};
}

void render::TranslucentSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	glGenBuffers(1, &m_ColourBuffer);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexParamBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strQuadMesh);
}

void render::TranslucentSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strQuadMesh);
}

void render::TranslucentSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		// sprite
		{
			using RenderQuery = ecs::query
				::Include<
				const eng::SpriteComponent,
				const eng::TransformComponent>;
			for (auto&& renderView : world.Query<RenderQuery>())
			{
				const auto& spriteComponent = renderView.ReadRequired<eng::SpriteComponent>();
				const auto& spriteTransform = renderView.ReadRequired<eng::TransformComponent>();
				const auto* spriteAsset = assetManager.ReadAsset<eng::SpriteAsset>(spriteComponent.m_Sprite);
				if (!spriteAsset)
					continue;

				render::BatchId id;
				id.m_Entity = renderView;
				id.m_Depth = math::DistanceSqr(spriteTransform.m_Translate, cameraTransform.m_Translate);
				id.m_ShaderId = spriteAsset->m_Shader;
				id.m_StaticMeshId = strQuadMesh;
				id.m_TextureId = spriteAsset->m_Texture2D;
				ids.Append(id);
			}
		}

		// flipbook
		{
			using RenderQuery = ecs::query
				::Include<
				const eng::FlipbookComponent,
				const eng::TransformComponent>;
			for (auto&& renderView : world.Query<RenderQuery>())
			{
				const auto& flipbookComponent = renderView.ReadRequired<eng::FlipbookComponent>();
				const auto& flipbookTransform = renderView.ReadRequired<eng::TransformComponent>();
				const auto* flipbookAsset = assetManager.ReadAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);
				if (!flipbookAsset || flipbookAsset->m_Frames.IsEmpty())
					continue;
				if (flipbookComponent.m_Index >= flipbookAsset->m_Frames.GetCount())
					continue;

				const eng::FlipbookFrame& flipbookFrame = flipbookAsset->m_Frames[flipbookComponent.m_Index];
				if (!flipbookAsset->m_Texture2D.IsValid())
					continue;

				render::BatchId& id = ids.Emplace();
				id.m_Entity = renderView;
				id.m_Depth = math::DistanceSqr(flipbookTransform.m_Translate, cameraTransform.m_Translate);
				id.m_ShaderId = flipbookAsset->m_Shader;
				id.m_StaticMeshId = strQuadMesh;
				id.m_TextureId = flipbookAsset->m_Texture2D;
			}
		}

		if (ids.IsEmpty())
			continue;

		std::sort(ids.begin(), ids.end(), Sort());

		render::BatchId lastId = ids.GetFirst();
		render::Batch batch;
		render::Data data = { cameraProj, cameraView };

		for (const render::BatchId& id : ids)
		{
			const bool isSameBatch =
				id.m_ShaderId == id.m_ShaderId &&
				id.m_TextureId == id.m_TextureId &&
				id.m_StaticMeshId == id.m_StaticMeshId;

			if (!isSameBatch)
			{
				RenderBatch(world, id, batch, data);
				batch.m_Colours.RemoveAll();
				batch.m_Models.RemoveAll();
				batch.m_TexParams.RemoveAll();
			}

			if (world.HasComponent<eng::VisibilityComponent>(id.m_Entity))
			{
				const auto& visibileComponent = world.ReadComponent<eng::VisibilityComponent>(id.m_Entity);
				if (!visibileComponent.m_IsVisible)
					continue;
			}

			if (world.HasComponent<eng::FlipbookComponent>(id.m_Entity))
			{
				const auto& flipbookComponent = world.ReadComponent<eng::FlipbookComponent>(id.m_Entity);
				const auto& flipbookTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);
				const auto* flipbookAsset = assetManager.ReadAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);
				if (!flipbookAsset)
					continue;
				const auto* texture2DAsset = assetManager.ReadAsset<eng::Texture2DAsset>(flipbookAsset->m_Texture2D);
				if (!texture2DAsset)
					continue;

				const eng::FlipbookFrame& flipbookFrame = flipbookAsset->m_Frames[flipbookComponent.m_Index];
				const Vector2f& spritePos = flipbookFrame.m_Position;
				const Vector2f& spriteSize = flipbookFrame.m_Size;
				const Vector2f textureSize = Vector2f((float)texture2DAsset->m_Width, (float)texture2DAsset->m_Height);

				const Vector3f modelScale = flipbookTransform.m_Scale;
				const Vector3f spriteScale = Vector3f(
					(float)flipbookComponent.m_Size.x / 100.f,
					(float)flipbookComponent.m_Size.y / 100.f,
					1.f);

				Matrix4x4 model = flipbookTransform.ToTransform();
				model.SetScale(math::Multiply(modelScale, spriteScale));

				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				batch.m_Colours.Append(Colour::White);
				batch.m_Models.Append(model);
				batch.m_TexParams.Emplace(
					texcoordOffset.x, texcoordOffset.y,
					texcoordScale.x, texcoordScale.y);
			}
			else if (world.HasComponent<eng::SpriteComponent>(id.m_Entity))
			{
				const auto& spriteComponent = world.ReadComponent<eng::SpriteComponent>(id.m_Entity);
				const auto& spriteTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);
				const auto* spriteAsset = assetManager.ReadAsset<eng::SpriteAsset>(spriteComponent.m_Sprite);
				if (!spriteAsset)
					continue;
				const auto* texture2DAsset = assetManager.ReadAsset<eng::Texture2DAsset>(spriteAsset->m_Texture2D);
				if (!texture2DAsset)
					continue;

				const Vector2f spritePos = Vector2f(spriteAsset->m_Position.x, spriteAsset->m_Position.y);
				const Vector2f spriteSize = Vector2f(spriteAsset->m_Size.x, spriteAsset->m_Size.y);
				const Vector2f textureSize = Vector2f((float)texture2DAsset->m_Width, (float)texture2DAsset->m_Height);
				const Vector2u componentSize = spriteComponent.m_Size.value_or(Vector2u((uint32)spriteSize.x, (uint32)spriteSize.y));

				const Vector3f modelScale = spriteTransform.m_Scale;
				const Vector3f spriteScale = Vector3f(
					(float)componentSize.x / 100.f,
					(float)componentSize.y / 100.f,
					1.f);

				Matrix4x4 model = spriteTransform.ToTransform();
				model.SetScale(math::Multiply(modelScale, spriteScale));

				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				batch.m_Colours.Append(spriteComponent.m_Colour.value_or(Colour::White));
				batch.m_Models.Append(model);
				batch.m_TexParams.Emplace(
					texcoordOffset.x, texcoordOffset.y,
					texcoordScale.x, texcoordScale.y);
			}

			// do last
			lastId = id;
		}

		RenderBatch(world, lastId, batch, data);
	}
}

void render::TranslucentSystem::RenderBatch(World& world, const render::BatchId& id, const render::Batch& batch, const render::Data& data)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.ReadAsset<eng::StaticMeshAsset>(id.m_StaticMeshId);
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(id.m_ShaderId);
	const auto* texture = assetManager.ReadAsset<eng::Texture2DAsset>(id.m_TextureId);
	if (!mesh || !shader || !texture)
		return;
	if (texture->m_TextureId == 0)
		return;
	if (batch.m_Models.IsEmpty())
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Colour) * instanceCount, &batch.m_Colours[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader->i_Colour;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Colour), (void*)(0));
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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
