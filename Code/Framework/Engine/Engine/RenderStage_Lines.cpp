#include "EnginePCH.h"
#include "Engine/RenderStage_Lines.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/ShaderAsset.h"
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strLinesShader = GUID("dad72cc07659496b83b1c5a85a4b3695");
}

void eng::RenderStage_Lines::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);
	glGenBuffers(1, &m_VertexBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::ShaderAsset>(strLinesShader);
}

void eng::RenderStage_Lines::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteVertexArrays(1, &m_AttributeObject);
	glDeleteBuffers(1, &m_VertexBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::ShaderAsset>(strLinesShader);
}

void eng::RenderStage_Lines::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto& debugSettings = world.ReadSingleton<eng::settings::DebugComponent>();
	if (!debugSettings.m_AreLinesEnabled)
		return;

	const auto* shader = assetManager.FetchAsset<eng::ShaderAsset>(strLinesShader);
	if (!shader)
		return;

	const auto& windowManager = world.ReadResource<const eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	const auto& readComponent = world.ReadSingleton<eng::LinesComponent>();
	if (!readComponent.m_Vertices.IsEmpty())
	{
		const Vector2u& resolution = window->GetResolution();
		glViewport(0, 0, resolution.x, resolution.y);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glUseProgram(shader->m_ProgramId);
		glBindVertexArray(m_AttributeObject);

		const uint32 vertexCount = readComponent.m_Vertices.GetCount();
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
		{
			const bool isEditorActive = debugSettings.m_IsEditorModeEnabled;
			const bool isEditorCamera = world.HasComponent<eng::camera::EditorComponent>(cameraEntity);
			if (isEditorActive != isEditorCamera)
				continue;

			const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Matrix4x4 cameraProj = eng::camera::GetProjection(resolution, cameraComponent.m_Projection);
			const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

			constexpr size_t s_PointOffset = offsetof(LineVertex, m_Point);
			constexpr size_t s_ColourOffset = offsetof(LineVertex, m_Colour);

			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(LineVertex), &readComponent.m_Vertices[0], GL_DYNAMIC_DRAW);

			if (shader->a_Vertex)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

				const uint32 location = *shader->a_Vertex;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_PointOffset);
				glVertexAttribDivisor(location, GL_FALSE);
			}

			if (shader->a_Colour)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

				const uint32 location = *shader->a_Colour;
				glEnableVertexAttribArray(location);
				glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_ColourOffset);
				glVertexAttribDivisor(location, GL_FALSE);
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

			glDrawArrays(GL_LINES, 0, vertexCount);
		}

		auto& writeComponent = world.WriteSingleton<eng::LinesComponent>();
		writeComponent.m_Vertices.RemoveAll();
	}
}