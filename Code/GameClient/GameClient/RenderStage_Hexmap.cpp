#include "GameClientPCH.h"
#include "GameClient/RenderStage_Hexmap.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/ColourHelpers.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "GameClient/HexmapAssetComponent.h"
#include "GameClient/HexmapChartComponent.h"
#include "GameClient/HexmapFragmentComponent.h"
#include "Math/HexagonHelpers.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strMesh = str::Guid::Create("ecf9330f287d40fea37789491c3c32a3");

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
}

void hexmap::RenderStage::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenBuffers(1, &m_ColourBuffer);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexParamBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::StaticMeshAsset>(strMesh);
}

void hexmap::RenderStage::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::StaticMeshAsset>(strMesh);
}

void hexmap::RenderStage::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();

	{
		glViewport(0, 0, static_cast<int32>(Screen::width), static_cast<int32>(Screen::height));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraProj = eng::camera::GetProjection(screenSize, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		Array<eng::RenderBatchID> batchIDs;

		// fragment
		{
			using Query = ecs::query
				::Include<
				hexmap::AssetComponent,
				hexmap::FragmentComponent,
				eng::TransformComponent>;

			for (const ecs::Entity& renderEntity : world.Query<Query>())
			{
				const auto& assetComponent = world.ReadComponent<hexmap::AssetComponent>(renderEntity);
				const auto& fragmentComponent = world.ReadComponent<hexmap::FragmentComponent>(renderEntity);
				const auto& transformComponent = world.ReadComponent<eng::TransformComponent>(renderEntity);

				const eng::SpriteAsset* spriteAsset = assetComponent.m_Sprite;
				const eng::Texture2DAsset* texture2DAsset = assetComponent.m_Texture2D;
				if (!spriteAsset)
					continue;

				eng::RenderBatchID id;
				id.m_Entity = renderEntity;
				id.m_Depth = math::DistanceSqr(transformComponent.m_Translate, cameraTransform.m_Translate);
				id.m_ShaderId = spriteAsset->m_Shader;
				id.m_StaticMeshId = strMesh;
				id.m_TextureId = spriteAsset->m_Texture2D;
				batchIDs.Append(id);
			}
		}

		if (batchIDs.IsEmpty())
			continue;

		std::sort(batchIDs.begin(), batchIDs.end(), Sort());

		eng::RenderBatchID batchID = batchIDs.GetFirst();
		eng::RenderBatchData batchData;
		eng::RenderStageData stageData = { cameraProj, cameraView };

		for (const eng::RenderBatchID& id : batchIDs)
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

			if (world.HasComponent<hexmap::FragmentComponent>(id.m_Entity))
			{
				const auto& fragment = world.ReadComponent<hexmap::FragmentComponent>(id.m_Entity);
				const auto& transform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);

				const auto& assetComponent = world.ReadComponent<hexmap::AssetComponent>(id.m_Entity);
				const auto& fragmentComponent = world.ReadComponent<hexmap::FragmentComponent>(id.m_Entity);
				const auto& spriteTransform = world.ReadComponent<eng::TransformComponent>(id.m_Entity);

				const eng::SpriteAsset* spriteAsset = assetComponent.m_Sprite;
				const eng::Texture2DAsset* texture2DAsset = assetComponent.m_Texture2D;
				if (!spriteAsset || !texture2DAsset)
					continue;

				const Vector2f spritePos = Vector2f((float)spriteAsset->m_Position.x, (float)spriteAsset->m_Position.y);
				const Vector2f spriteSize = Vector2f((float)spriteAsset->m_Size.x, (float)spriteAsset->m_Size.y);
				const Vector2f textureSize = Vector2f((float)texture2DAsset->m_Width, (float)texture2DAsset->m_Height);

				const Vector3f modelScale = transform.m_Scale * 2.f;
				const Vector3f fragmentScale = Vector3f(
					fragment.m_Radius / 100.f,
					fragment.m_Radius / 100.f,
					1.f);

				Matrix4x4 model = transform.ToTransform();
				model.SetScale(math::Multiply(modelScale, fragmentScale));

				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				const int32 width = fragment.GetWidth();
				const int32 height = fragment.GetHeight();
				const int32 count = fragment.m_Data.GetCount();
				for (int32 i = 0; i < count; ++i)
				{
					const int32 x = i % width - width / 2;
					const int32 y = i / width - height / 2;
					const hexagon::Offset gridPos = { x, y };
					const Vector2f localPos = hexagon::ToWorldPos(gridPos, fragment.m_Radius);

					model.SetTranslate(transform.m_Translate + localPos.X0Y());

					batchData.m_Colours.Append(Vector3f(1.f));
					batchData.m_Models.Append(model);
					batchData.m_TexParams.Emplace(
						texcoordOffset.x, texcoordOffset.y,
						texcoordScale.x, texcoordScale.y);
				}
			}

			// do last
			batchID = id;
		}

		RenderBatch(world, batchID, batchData, stageData);
	}
}

void hexmap::RenderStage::RenderBatch(World& world, const eng::RenderBatchID& batchID, const eng::RenderBatchData& batchData, const eng::RenderStageData& stageData)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.FetchAsset<eng::StaticMeshAsset>(batchID.m_StaticMeshId);
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(batchID.m_ShaderId);
	const auto* texture = assetManager.FetchAsset<eng::Texture2DAsset>(batchID.m_TextureId);
	if (!mesh || !shader || !texture)
		return;
	if (texture->m_TextureId == 0)
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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
		glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
	}
}
