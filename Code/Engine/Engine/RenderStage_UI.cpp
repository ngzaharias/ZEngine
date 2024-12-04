#include "EnginePCH.h"
#include "Engine/RenderStage_UI.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/FontAsset.h"
#include "Engine/Screen.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/TextComponent.h"
#include "Engine/TransformComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strModel = GUID("e94876a8e4cc4d1684c85859b48a1af6");
	const str::Guid strShader = GUID("0205bbd9a15b459eaf0d810ebe98b8d8");
}

void eng::RenderStage_UI::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);
	glGenBuffers(1, &m_ModelBuffer);
	glGenBuffers(1, &m_TexCoordBuffer);
	glGenBuffers(1, &m_TexDepthBuffer);
	glGenBuffers(1, &m_VertexBuffer);

	const Vector3f vertices[] = {
		Vector3f(-1.f, -1.f, 0.f),
		Vector3f(-1.f, +1.f, 0.f),
		Vector3f(+1.f, +1.f, 0.f),
		Vector3f(+1.f, +1.f, 0.f),
		Vector3f(+1.f, -1.f, 0.f),
		Vector3f(-1.f, -1.f, 0.f),
	};

	const Vector2f texcoords[] = {
		Vector2f(0.f, 0.f),
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f),
		Vector2f(1.f, 1.f),
		Vector2f(1.f, 0.f),
		Vector2f(0.f, 0.f),
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * 6, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
	glVertexAttribDivisor(0, GL_FALSE);

	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);
	glVertexAttribDivisor(1, GL_FALSE);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::StaticMeshAsset>(strModel);
	assetManager.RequestAsset<eng::ShaderAsset>(strShader);
}

void eng::RenderStage_UI::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteBuffers(1, &m_ModelBuffer);
	glDeleteBuffers(1, &m_TexCoordBuffer);
	glDeleteBuffers(1, &m_TexDepthBuffer);
	glDeleteBuffers(1, &m_VertexBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::StaticMeshAsset>(strModel);
	assetManager.ReleaseAsset<eng::ShaderAsset>(strShader);
}

void eng::RenderStage_UI::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* mesh = assetManager.FetchAsset<eng::StaticMeshAsset>(strModel);
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(strShader);
	if (!mesh || !shader)
		return;

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

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraProj = camera::GetProjection(screenSize, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		glUseProgram(shader->m_ProgramId);
		glBindVertexArray(m_AttributeObject);

		using Query = ecs::query
			::Include<
			eng::TextAssetComponent,
			eng::TextComponent,
			eng::TransformComponent>;

		for (const ecs::Entity& textEntity : world.Query<Query>())
		{
			const auto& assetComponent = world.ReadComponent<eng::TextAssetComponent>(textEntity);
			const auto& textComponent = world.ReadComponent<eng::TextComponent>(textEntity);
			const auto& textTransform = world.ReadComponent<eng::TransformComponent>(textEntity);
			const auto& binding = mesh->m_Binding;

			int32 instanceCount = static_cast<int32>(textComponent.m_Text.size());
			const auto* fontAsset = assetComponent.m_Font;
			if (!fontAsset || fontAsset->m_TextureId == 0)
				continue;

			const Matrix4x4 model = textTransform.ToTransform();

			Array<float> depths;
			Array<Matrix4x4> models;
			depths.Reserve(instanceCount);
			models.Reserve(instanceCount);
			for (auto&& [i, character] : enumerate::Forward(textComponent.m_Text))
			{
				if (fontAsset->m_Glyphs.Contains(character))
				{
					const eng::Glyph glyph = fontAsset->m_Glyphs.Get(character);
					depths.Append(glyph.m_Depth);

					Vector3f offset = Vector3f::Zero;
					offset.x = static_cast<float>(glyph.m_AdvanceX * i);
					offset.y = static_cast<float>(glyph.m_AdvanceY);

					Matrix4x4& charModel = models.Append(model);
					charModel.Translate(offset);
				}
			}
			instanceCount = models.GetCount();

			// vertices
			if (shader->a_Vertex)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_VertexBuffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, binding.m_IndexBuffer);

				const uint32 location = *shader->a_Vertex;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			// texture coordinates
			if (shader->a_TexCoords)
			{
				glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);

				const uint32 location = *shader->a_TexCoords;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			// models
			if (shader->i_Model)
			{
				// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead
				// #todo: glMapBuffer ?

				glBindBuffer(GL_ARRAY_BUFFER, m_ModelBuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix4x4) * instanceCount, models.GetData(), GL_DYNAMIC_DRAW);

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

			// texture depth
			if (shader->i_TexDepth)
			{
				// #todo: only call glBufferData if we need to shrink/grow the buffer, use glSubBufferData instead

				glBindBuffer(GL_ARRAY_BUFFER, m_TexDepthBuffer);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instanceCount, depths.GetData(), GL_DYNAMIC_DRAW);

				const uint32 location = *shader->i_TexDepth;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(0));
				glVertexAttribDivisor(location, GL_TRUE);
			}

			// camera - projection
			if (shader->u_CameraProj)
			{
				const uint32 location = *shader->u_CameraProj;
				glUniformMatrix4fv(location, 1, false, &cameraProj.m_Data[0][0]);
			}

			// camera - view
			if (shader->u_CameraView)
			{
				const uint32 location = *shader->u_CameraView;
				glUniformMatrix4fv(location, 1, false, &cameraView.m_Data[0][0]);
			}

			// model
			if (shader->u_Model)
			{
				Matrix4x4 model = textTransform.ToTransform().Inversed();

				const uint32 location = *shader->u_Model;
				glUniformMatrix4fv(location, 1, false, &model[0][0]);
			}

			// pixel range
			if (shader->u_PixelRange)
			{
				const uint32 location = *shader->u_PixelRange;
				glUniform1i(location, fontAsset->m_PixelRange);
			}

			// resolution
			if (shader->u_Resolution)
			{
				const uint32 location = *shader->u_Resolution;
				glUniform1f(location, 100.f);
			}

			// texture
			if (shader->u_Texture)
			{
				glActiveTexture(GL_TEXTURE0 + *shader->u_Texture);
				glBindTexture(GL_TEXTURE_2D_ARRAY, fontAsset->m_TextureId);
			}

			glDrawElementsInstanced(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
		}
	}
}