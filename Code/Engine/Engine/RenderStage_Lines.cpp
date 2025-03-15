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
#include "Engine/Screen.h"
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

	const auto* linesShader = assetManager.FetchAsset<eng::ShaderAsset>(strLinesShader);
	if (!linesShader)
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

		const uint32 vertexCount = readComponent.m_Vertices.GetCount();
		for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
		{
			const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Matrix4x4 cameraProj = eng::camera::GetProjection(resolution, cameraComponent.m_Projection);
			const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

			constexpr size_t s_PointOffset = offsetof(LineVertex, m_Point);
			constexpr size_t s_ColourOffset = offsetof(LineVertex, m_Colour);

			glUseProgram(linesShader->m_ProgramId);

			glBindVertexArray(m_AttributeObject);

			const uint32 transformId = glGetUniformLocation(linesShader->m_ProgramId, "u_Transform");
			const Matrix4x4 transform = cameraView * cameraProj;
			glUniformMatrix4fv(transformId, 1, GL_FALSE, &transform[0][0]);

			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(LineVertex), &readComponent.m_Vertices[0], GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_PointOffset);
			glVertexAttribDivisor(0, GL_FALSE);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_ColourOffset);
			glVertexAttribDivisor(1, GL_FALSE);

			glDrawArrays(GL_LINES, 0, vertexCount);
		}

		auto& writeComponent = world.WriteSingleton<eng::LinesComponent>();
		writeComponent.m_Vertices.RemoveAll();
	}
}