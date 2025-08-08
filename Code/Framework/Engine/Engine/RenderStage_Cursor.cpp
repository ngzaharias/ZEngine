#include "EnginePCH.h"
#include "Engine/RenderStage_Cursor.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraHelpers.h"
#include "Engine/InputManager.h"
#include "Engine/ShaderAsset.h"
#include "Engine/StaticMeshAsset.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr float s_Distance = 32.f;
	constexpr Colour s_ColourX = Colour(0.58f, 0.23f, 0.29f);
	constexpr Colour s_ColourY = Colour(0.38f, 0.52f, 0.16f);
	constexpr Colour s_ColourZ = Colour(0.21f, 0.4f, 0.61f);

	const str::Guid strMesh = GUID("e94876a8e4cc4d1684c85859b48a1af6");
	const str::Guid strShader = GUID("b7d0c9eb5380438ba90b9a344626134a");
	const str::Guid strTexture = GUID("319277084bf14d798b940cbc1c6e3825");
}

void eng::RenderStage_Cursor::Initialise(ecs::EntityWorld& entityWorld)
{
	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::ShaderAsset>(strShader);
	assetManager.RequestAsset<eng::StaticMeshAsset>(strMesh);
	assetManager.RequestAsset<eng::Texture2DAsset>(strTexture);
}

void eng::RenderStage_Cursor::Shutdown(ecs::EntityWorld& entityWorld)
{
	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::ShaderAsset>(strShader);
	assetManager.ReleaseAsset<eng::StaticMeshAsset>(strMesh);
	assetManager.ReleaseAsset<eng::Texture2DAsset>(strTexture);
}

void eng::RenderStage_Cursor::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(strShader);
	const auto* mesh = assetManager.FetchAsset<eng::StaticMeshAsset>(strMesh);
	const auto* texture = assetManager.FetchAsset<eng::Texture2DAsset>(strTexture);
	if (!shader || !mesh || !texture)
		return;

	const auto& inputManager = world.ReadResource<eng::InputManager>();
	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	{
		const Vector2u& resolution = window->GetResolution();
		glViewport(0, 0, resolution.x, resolution.y);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

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

		//const Vector2f spritePos = Vector2f(spriteAsset->m_Position.x, spriteAsset->m_Position.y);
		//const Vector2f spriteSize = Vector2f(spriteAsset->m_Size.x, spriteAsset->m_Size.y);
		//const Vector2f textureSize = Vector2f((float)texture2DAsset->m_Width, (float)texture2DAsset->m_Height);
		//const Vector2u componentSize = spriteComponent.m_Size.value_or(Vector2u((uint32)spriteSize.x, (uint32)spriteSize.y));

		//const Vector3f modelScale = spriteTransform.m_Scale;
		//const Vector3f spriteScale = Vector3f(
		//	(float)componentSize.x / 100.f,
		//	(float)componentSize.y / 100.f,
		//	1.f);

		const Matrix4x4 cameraProj = eng::camera::GetProjection(resolution, camera::UserInterface{});
		const Matrix4x4 transform = Matrix4x4::FromTransform(
			Vector3f(inputManager.m_MousePosition, 0.f),
			Quaternion::Identity,
			Vector3f::One);

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