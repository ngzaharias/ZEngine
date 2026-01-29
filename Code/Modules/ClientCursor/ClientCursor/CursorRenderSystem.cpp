#include "ClientCursorPCH.h"
#include "ClientCursor/CursorRenderSystem.h"

#include "ClientCursor/CursorRenderComponent.h"
#include "Core/Algorithms.h"
#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraHelpers.h"
#include "Engine/CameraTypes.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strMesh = GUID("e94876a8e4cc4d1684c85859b48a1af6");
	const str::Guid strShader = GUID("b7d0c9eb5380438ba90b9a344626134a");
	const str::Guid strTexture = GUID("319277084bf14d798b940cbc1c6e3825");
}

void client::cursor::RenderSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strShader);
	assetManager.RequestAsset(strMesh);
	assetManager.RequestAsset(strTexture);
}

void client::cursor::RenderSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strShader);
	assetManager.ReleaseAsset(strMesh);
	assetManager.ReleaseAsset(strTexture);
}

void client::cursor::RenderSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(strShader);
	const auto* mesh = assetManager.ReadAsset<eng::StaticMeshAsset>(strMesh);
	const auto* texture = assetManager.ReadAsset<eng::Texture2DAsset>(strTexture);
	if (!shader || !mesh || !texture)
		return;

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const Vector2u& windowSize = window->GetSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	using Query = ecs::query
		::Include<
		const client::cursor::RenderComponent,
		const eng::TransformComponent>;
	for (auto&& view : world.Query<Query>())
	{
		glUseProgram(shader->m_ProgramId);
		//glBindVertexArray(m_AttributeObject);

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

		// texcoords
		if (shader->a_TexCoords)
		{
			glBindBuffer(GL_ARRAY_BUFFER, binding.m_TexCoordBuffer);

			const uint32 location = *shader->a_TexCoords;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
			glVertexAttribDivisor(location, GL_FALSE);
		}

		const auto& transformComponent = view.ReadRequired<eng::TransformComponent>();
		const Vector3f& translate = transformComponent.m_Translate;

		const Matrix4x4 cameraProj = eng::GetProjection(eng::UserInterface{}, windowSize);
		const Matrix4x4 transform = Matrix4x4::FromTransform(
			Vector3f(translate.x, translate.y, 0.f) + Vector3f(10.f, 14.f, 0.f),
			Vector3f(32.f / 100.f, 32.f / 100.f, 1.f));

		if (shader->u_CameraProj)
		{
			const uint32 location = *shader->u_CameraProj;
			glUniformMatrix4fv(location, 1, false, &cameraProj.m_Data[0][0]);
		}

		if (shader->u_Transform)
		{
			const uint32 location = *shader->u_Transform;
			glUniformMatrix4fv(location, 1, false, &transform.m_Data[0][0]);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
		glDrawElements(GL_TRIANGLES, mesh->m_Indices.GetCount(), GL_UNSIGNED_INT, 0);
	}
}