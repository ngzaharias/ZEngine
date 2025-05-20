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
#include "Engine/ShaderAsset.h"
#include "Engine/SpriteAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Math/HexagonHelpers.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr Vector2f s_SpriteSize = Vector2f(238.f, 207.f);

	const str::Guid strMesh = str::Guid::Create("ecf9330f287d40fea37789491c3c32a3");
	const str::Guid strShader = str::Guid::Create("94eb696e6c1841309a2071df1f0f1823");
	const str::Guid strTexture = str::Guid::Create("3c8610fc623e4496951dcefeaf0495ab");

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
	assetManager.RequestAsset<eng::ShaderAsset>(strShader);
	assetManager.RequestAsset<eng::Texture2DAsset>(strTexture);
}

void hexmap::RenderStage::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteBuffers(1, &m_ColourBuffer);
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexParamBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::StaticMeshAsset>(strMesh);
	assetManager.ReleaseAsset<eng::ShaderAsset>(strShader);
	assetManager.ReleaseAsset<eng::Texture2DAsset>(strTexture);
}

void hexmap::RenderStage::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.FetchAsset<eng::StaticMeshAsset>(strMesh);
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(strShader);
	const auto* texture = assetManager.FetchAsset<eng::Texture2DAsset>(strTexture);
	if (!mesh || !shader || !texture)
		return;
	if (texture->m_TextureId == 0)
		return;

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const bool isEditorActive = true;
		const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
		if (isEditorActive != isEditorCamera)
			continue;

		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Matrix4x4 cameraProj = eng::camera::GetProjection(resolution, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		Array<Matrix4x4> m_Models;
		Array<Vector4f>  m_TexParams;
		Array<Vector3f>  m_Colours;

		using Query = ecs::query
			::Include<
			hexmap::LayerComponent,
			eng::TransformComponent>;

		for (const ecs::Entity& renderEntity : world.Query<Query>())
		{
			const auto& layer = world.ReadComponent<hexmap::LayerComponent>(renderEntity);
			if (!world.IsAlive(layer.m_Root))
				continue;

			const auto& root = world.ReadComponent<hexmap::RootComponent>(layer.m_Root);
			const auto& transform = world.ReadComponent<eng::TransformComponent>(renderEntity);

			const Vector2f spriteSize = s_SpriteSize;
			const Vector2f textureSize = Vector2f((float)texture->m_Width, (float)texture->m_Height);

			const Vector3f modelScale = transform.m_Scale;
			const Vector3f fragmentScale = Vector3f(
				root.m_HexRadius / 100.f * 2.f,
				root.m_HexRadius / 100.f * 2.f,
				1.f);

			Matrix4x4 model = transform.ToTransform();
			model.SetScale(math::Multiply(modelScale, fragmentScale));

			const int32 width = root.m_HexCount.x;
			const int32 height = root.m_HexCount.y;
			const int32 count = layer.m_HexData.GetCount();
			for (int32 i = 0; i < count; ++i)
			{
				const int32 index = layer.m_HexData[i].m_Index % 3;
				const int32 indey = layer.m_HexData[i].m_Index / 3;

				const Vector2i gridPos = math::To2Dimension(i, width);
				const hexagon::Offset hexPos = { gridPos.x, gridPos.y };
				const Vector2f localPos = hexagon::ToWorldPos(hexPos, root.m_HexRadius);

				model.SetTranslate(transform.m_Translate + localPos.X0Y());

				const Vector2f spritePos = Vector2f(spriteSize.x * index, spriteSize.y * indey);
				const Vector2f texcoordOffset = Vector2f(
					spritePos.x / textureSize.x,
					spritePos.y / textureSize.y);
				const Vector2f texcoordScale = Vector2f(
					spriteSize.x / textureSize.x,
					spriteSize.y / textureSize.y);

				m_Colours.Append(Vector3f(1.f));
				m_Models.Append(model);
				m_TexParams.Emplace(
					texcoordOffset.x, texcoordOffset.y,
					texcoordScale.x, texcoordScale.y);
			}
		}

		glUseProgram(shader->m_ProgramId);

		if (!m_Models.IsEmpty())
		{
			const int32 i = 0;
			const int32 instanceCount = m_Models.GetCount();
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
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * instanceCount, &m_TexParams[i], GL_DYNAMIC_DRAW);

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
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * instanceCount, &m_Colours[i], GL_DYNAMIC_DRAW);

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
				glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceCount, &m_Models[i], GL_DYNAMIC_DRAW);

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
				glUniformMatrix4fv(location, 1, false, &cameraProj.m_Data[0][0]);
			}

			if (shader->u_CameraView)
			{
				const uint32 location = *shader->u_CameraView;
				glUniformMatrix4fv(location, 1, false, &cameraView.m_Data[0][0]);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
			glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
		}
	}
}
