#include "EditorPCH.h"
#include "Editor/RenderStage_Grid.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SettingsDebugSingleton.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// https://bgolus.medium.com/the-best-darn-grid-shader-yet-727f9278b9d8#bba3
// https://gist.github.com/bgolus/3a561077c86b5bfead0d6cc521097bae

namespace
{
	const str::Guid strShader = GUID("f681f1dfc59e40f7864f01d1efbe1d84");

	constexpr float s_Size = KINDA_LARGE_FLOAT;
	const Array<Vector3f> s_Vertices = {
		Vector3f(-1.f, 0.f, -1.f) * s_Size,
		Vector3f(-1.f, 0.f, +1.f) * s_Size,
		Vector3f(+1.f, 0.f, +1.f) * s_Size,
		Vector3f(+1.f, 0.f, -1.f) * s_Size };
	const Array<Vector2f> s_TexCoords = {
		Vector2f(0.f, 0.f),
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f),
		Vector2f(1.f, 0.f) };
	const Array<uint32> s_Indices = { 0, 1, 2, 0, 2, 3};
}

void editor::RenderStage_Grid::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);

	// vertex
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * s_Vertices.GetCount(), s_Vertices.GetData(), GL_STATIC_DRAW);

	// texture coordinates
	glGenBuffers(1, &m_TexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * s_TexCoords.GetCount(), s_TexCoords.GetData(), GL_STATIC_DRAW);

	// index
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * s_Indices.GetCount(), s_Indices.GetData(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strShader);
}

void editor::RenderStage_Grid::Shutdown(ecs::EntityWorld& entityWorld)
{
	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strShader);
}

void editor::RenderStage_Grid::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugSingleton>();
	const auto& localSettings = world.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_FloorGrid;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(strShader);
	if (!shader)
		return;

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;


	{
		const Vector2u& windowSize = window->GetSize();
		glViewport(0, 0, windowSize.x, windowSize.y);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_FALSE);

		glDisable(GL_CULL_FACE);
	}

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const bool isEditorActive = debugSettings.m_IsEditorModeEnabled;
		const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
		if (isEditorActive != isEditorCamera)
			continue;

		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Vector3f translate = cameraTransform.m_Translate.X0Z();

		const Vector2u& windowSize = window->GetSize();
		const Matrix4x4 cameraProj = eng::camera::GetProjection(cameraComponent.m_Projection, windowSize);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();
		const Matrix4x4 modelTran = Matrix4x4::FromTranslate(translate);

		glUseProgram(shader->m_ProgramId);

		{
			const int32 instanceCount = 1;

			glBindVertexArray(m_AttributeObject);

			// vertices
			// indices
			if (shader->a_Vertex)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

				const uint32 location = *shader->a_Vertex;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			// tex-coords
			if (shader->a_TexCoords)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);

				const uint32 location = *shader->a_TexCoords;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), 0);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			if (shader->u_CameraProj)
			{
				const uint32 location = *shader->u_CameraProj;
				glUniformMatrix4fv(location, 1, GL_FALSE, &cameraProj[0][0]);
			}

			if (shader->u_CameraView)
			{
				const uint32 location = *shader->u_CameraView;
				glUniformMatrix4fv(location, 1, GL_FALSE, &cameraView[0][0]);
			}

			if (shader->u_Transform)
			{
				const uint32 location = *shader->u_Transform;
				glUniformMatrix4fv(location, 1, GL_FALSE, &modelTran[0][0]);
			}

			{
				const int32 location = glGetUniformLocation(shader->m_ProgramId, "u_MajorScale");
				glUniform1f(location, settings.m_MajorScale);
			}

			{
				const int32 location = glGetUniformLocation(shader->m_ProgramId, "u_MajorWidth");
				glUniform1f(location, settings.m_MajorWidth);
			}

			{
				const int32 location = glGetUniformLocation(shader->m_ProgramId, "u_MajorColour");
				glUniform3fv(location, 1, &settings.m_MajorColour.x);
			}

			glDrawElementsInstanced(GL_TRIANGLES, s_Indices.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
		}
	}
}