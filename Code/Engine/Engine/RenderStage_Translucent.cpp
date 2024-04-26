#include "EnginePCH.h"
#include "Engine/RenderStage_Translucent.h"

#include <Core/Algorithms.h>

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/FlipbookAsset.h"
#include "Engine/FlipbookComponent.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteAsset.h"
#include "Engine/SpriteComponent.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"

namespace
{
	struct Sort
	{
		bool operator()(const eng::RenderBatchID& a, const eng::RenderBatchID& b)
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

	const str::Guid strQuadMesh = str::Guid::Create("e94876a8-e4cc-4d16-84c8-5859b48a1af6");
}

eng::RenderStage_Translucent::RenderStage_Translucent(eng::AssetManager& m_AssetManager)
	: RenderStage(m_AssetManager)
{
}

eng::RenderStage_Translucent::~RenderStage_Translucent()
{
}

void eng::RenderStage_Translucent::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenBuffers(1, &m_ColourBuffer);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexParamBuffer);
}

void eng::RenderStage_Translucent::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);
}

void eng::RenderStage_Translucent::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	using World = ecs::WorldView<
		const eng::CameraComponent,
		const eng::FlipbookComponent,
		const eng::SpriteComponent,
		const eng::TransformComponent>;
	World world = entityWorld.GetWorldView<World>();

	{
		glViewport(0, 0, static_cast<int32>(Screen::width), static_cast<int32>(Screen::height));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_FALSE);

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

		Array<RenderBatchID> batchIDs;

		// sprite
		{
			for (const ecs::Entity& renderEntity : world.Query<ecs::query::Include<const eng::SpriteComponent, const eng::TransformComponent>>())
			{
				const auto& spriteComponent = world.GetComponent<const eng::SpriteComponent>(renderEntity);
				const auto& spriteTransform = world.GetComponent<const eng::TransformComponent>(renderEntity);

				if (!spriteComponent.m_Sprite.IsValid())
					continue;

				const eng::SpriteAsset& spriteAsset = *m_AssetManager.LoadAsset<eng::SpriteAsset>(spriteComponent.m_Sprite);
				if (!spriteAsset.m_Texture2D.IsValid())
					continue;

				RenderBatchID id;
				id.m_Entity = renderEntity;
				id.m_Depth = math::DistanceSqr(spriteTransform.m_Translate, cameraTransform.m_Translate);
				id.m_ShaderId = spriteAsset.m_Shader;
				id.m_StaticMeshId = strQuadMesh;
				id.m_TextureId = spriteAsset.m_Texture2D;
				batchIDs.Append(id);
			}
		}

		// flipbook
		{
			for (const ecs::Entity& renderEntity : world.Query<ecs::query::Include<const eng::FlipbookComponent, const eng::TransformComponent>>())
			{
				const auto& flipbookComponent = world.GetComponent<const eng::FlipbookComponent>(renderEntity);
				const auto& flipbookTransform = world.GetComponent<const eng::TransformComponent>(renderEntity);

				if (!flipbookComponent.m_Flipbook.IsValid())
					continue;

				const eng::FlipbookAsset& flipbookAsset = *m_AssetManager.LoadAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);
				if (flipbookAsset.m_Frames.IsEmpty())
					continue;

				const int32 frameMax = flipbookAsset.m_Frames.GetCount() - 1;
				const int32 frameIndex = std::clamp(flipbookComponent.m_Index, 0, frameMax);

				const eng::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[frameIndex];
				if (!flipbookAsset.m_Texture2D.IsValid())
					continue;

				RenderBatchID& id = batchIDs.Emplace();
				id.m_Entity = renderEntity;
				id.m_Depth = math::DistanceSqr(flipbookTransform.m_Translate, cameraTransform.m_Translate);
				id.m_ShaderId = flipbookAsset.m_Shader;
				id.m_StaticMeshId = strQuadMesh;
				id.m_TextureId = flipbookAsset.m_Texture2D;
			}
		}

		if (batchIDs.IsEmpty())
			continue;

		std::sort(batchIDs.begin(), batchIDs.end(), Sort());

		RenderBatchID batchID = batchIDs.GetFirst();
		RenderBatchData batchData;
		RenderStageData stageData = { cameraProj, cameraView };

		for (const RenderBatchID& id : batchIDs)
		{
			const bool isSameBatch =
				id.m_ShaderId == batchID.m_ShaderId &&
				id.m_TextureId == batchID.m_TextureId &&
				id.m_StaticMeshId == batchID.m_StaticMeshId;

			if (!isSameBatch)
			{
				RenderBatch(batchID, batchData, stageData);
				batchData.m_Colours.RemoveAll();
				batchData.m_Models.RemoveAll();
				batchData.m_TexParams.RemoveAll();
			}

			if (world.HasComponent<eng::FlipbookComponent>(id.m_Entity))
			{
				const auto& flipbookComponent = world.GetComponent<const eng::FlipbookComponent>(id.m_Entity);
				const auto& flipbookTransform = world.GetComponent<const eng::TransformComponent>(id.m_Entity);
				const auto& flipbookAsset = *m_AssetManager.LoadAsset<eng::FlipbookAsset>(flipbookComponent.m_Flipbook);

				const int32 frameMax = flipbookAsset.m_Frames.GetCount() - 1;
				const int32 frameIndex = std::clamp(flipbookComponent.m_Index, 0, frameMax);

				const eng::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[frameIndex];
				const eng::Texture2DAsset& texture2DAsset = *m_AssetManager.LoadAsset<eng::Texture2DAsset>(flipbookAsset.m_Texture2D);

				const Vector2f& spritePos = flipbookFrame.m_Position;
				const Vector2f& spriteSize = flipbookFrame.m_Size;
				const Vector2f textureSize = Vector2f((float)texture2DAsset.m_Width, (float)texture2DAsset.m_Height);

				const Matrix4x4 model = flipbookTransform.ToTransform();

				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				const Vector4f& colour = colour::From(id.m_Entity);

				batchData.m_Colours.Emplace(colour.x, colour.y, colour.z);
				batchData.m_Models.Append(model);
				batchData.m_TexParams.Emplace(
					texcoordOffset.x, texcoordOffset.y,
					texcoordScale.x, texcoordScale.y);
			}
			else if (world.HasComponent<eng::SpriteComponent>(id.m_Entity))
			{
				const auto& spriteComponent = world.GetComponent<const eng::SpriteComponent>(id.m_Entity);
				const auto& spriteTransform = world.GetComponent<const eng::TransformComponent>(id.m_Entity);
				if (!spriteComponent.m_Sprite.IsValid())
					continue;

				const eng::SpriteAsset& spriteAsset = *m_AssetManager.LoadAsset<eng::SpriteAsset>(spriteComponent.m_Sprite);
				if (!spriteAsset.m_Texture2D.IsValid())
					continue;

				const eng::Texture2DAsset& texture2DAsset = *m_AssetManager.LoadAsset<eng::Texture2DAsset>(spriteAsset.m_Texture2D);
				if (texture2DAsset.m_TextureId == 0)
					continue;

				const Vector2f spritePos = Vector2f((float)spriteAsset.m_Position.x, (float)spriteAsset.m_Position.y);
				const Vector2f spriteSize = Vector2f((float)spriteAsset.m_Size.x, (float)spriteAsset.m_Size.y);
				const Vector2f textureSize = Vector2f((float)texture2DAsset.m_Width, (float)texture2DAsset.m_Height);

				const Vector3f modelScale = spriteTransform.m_Scale;
				const Vector3f spriteScale = Vector3f(
					(float)spriteComponent.m_Size.x / 100.f,
					(float)spriteComponent.m_Size.y / 100.f,
					1.f);

				Matrix4x4 model = spriteTransform.ToTransform();
				model.SetScale(math::Multiply(modelScale, spriteScale));

				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				const Vector4f& colour = colour::From(id.m_Entity);

				batchData.m_Colours.Emplace(colour.x, colour.y, colour.z);
				batchData.m_Models.Append(model);
				batchData.m_TexParams.Emplace(
					texcoordOffset.x, texcoordOffset.y,
					texcoordScale.x, texcoordScale.y);
			}

			// do last
			batchID = id;
		}

		RenderBatch(batchID, batchData, stageData);
	}
}

void eng::RenderStage_Translucent::RenderBatch(const RenderBatchID& batchID, const RenderBatchData& batchData, const RenderStageData& stageData)
{
	PROFILE_FUNCTION();

	const auto& mesh = *m_AssetManager.LoadAsset<eng::StaticMeshAsset>(batchID.m_StaticMeshId);
	const auto& shader = *m_AssetManager.LoadAsset<eng::ShaderAsset>(batchID.m_ShaderId);
	const auto& texture = *m_AssetManager.LoadAsset<eng::Texture2DAsset>(batchID.m_TextureId);

	glUseProgram(shader.m_ProgramId);

	{
		const int32 i = 0;
		const int32 instanceCount = batchData.m_Models.GetCount();
		const auto& binding = mesh.m_Binding;

		glBindVertexArray(binding.m_AttributeObject);

		// vertices
		// indices
		if (shader.a_Vertex)
		{
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.m_IndexBuffer);

			const uint32 location = *shader.a_Vertex;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
			glVertexAttribDivisor(location, GL_FALSE);
		}

		// normals
		if (shader.a_Normal)
		{
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_NormalBuffer);

			const uint32 location = *shader.a_Normal;
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

		// texparams
		if (shader.i_TexParams)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead

			glBindBuffer(GL_ARRAY_BUFFER, m_TexParamBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * instanceCount, &batchData.m_TexParams[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader.i_TexParams;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), (void*)(0));
			glVertexAttribDivisor(location, GL_TRUE);
		}

		// colours
		if (shader.i_Colour)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead

			glBindBuffer(GL_ARRAY_BUFFER, m_ColourBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * instanceCount, &batchData.m_Colours[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader.i_Colour;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)(0));
			glVertexAttribDivisor(location, GL_TRUE);
		}

		// models
		if (shader.i_Model)
		{
			// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead
			// #todo: glMapBuffer ?

			glBindBuffer(GL_ARRAY_BUFFER, m_ModelBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceCount, &batchData.m_Models[i], GL_DYNAMIC_DRAW);

			const uint32 location = *shader.i_Model;
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

		if (shader.u_CameraProj)
		{
			const uint32 location = *shader.u_CameraProj;
			glUniformMatrix4fv(location, 1, false, &stageData.m_CameraProj.m_Data[0][0]);
		}

		if (shader.u_CameraView)
		{
			const uint32 location = *shader.u_CameraView;
			glUniformMatrix4fv(location, 1, false, &stageData.m_CameraView.m_Data[0][0]);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.m_TextureId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh.m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
