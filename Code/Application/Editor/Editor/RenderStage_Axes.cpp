#include "EditorPCH.h"
#include "Editor/RenderStage_Axes.h"

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
#include "Engine/SettingsComponents.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	constexpr float s_Distance = 100000.f;
	constexpr Colour s_ColourX = Colour(0.58f, 0.23f, 0.29f);
	constexpr Colour s_ColourY = Colour(0.38f, 0.52f, 0.16f);
	constexpr Colour s_ColourZ = Colour(0.21f, 0.4f, 0.61f);

	const str::Guid strLinesShader = GUID("dad72cc07659496b83b1c5a85a4b3695");
}

void editor::RenderStage_Axes::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_ColourBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strLinesShader);

	const auto& settings = entityWorld.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& gizmos = settings.m_Gizmos;
	const auto& value = gizmos.m_CoordAxes;
	UpdateBuffers(value.m_ShowX, value.m_ShowY, value.m_ShowZ);
}

void editor::RenderStage_Axes::Shutdown(ecs::EntityWorld& entityWorld)
{
	glDeleteVertexArrays(1, &m_AttributeObject);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_ColourBuffer);

	auto& assetManager = entityWorld.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strLinesShader);
}

void editor::RenderStage_Axes::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	if (world.HasAny<editor::settings::LocalSingleton>())
	{
		const auto& settings = world.ReadSingleton<editor::settings::LocalSingleton>();
		const auto& gizmos = settings.m_Gizmos;
		const auto& value = gizmos.m_CoordAxes;
		UpdateBuffers(value.m_ShowX, value.m_ShowY, value.m_ShowZ);
	}

	const auto& localSettings = world.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& gizmos = localSettings.m_Gizmos;
	const auto& settings = gizmos.m_CoordAxes;
	if (!gizmos.m_IsEnabled || !settings.m_IsEnabled)
		return;

	const auto& assetManager = world.ReadResource<eng::AssetManager>();
	const auto* shader = assetManager.ReadAsset<eng::ShaderAsset>(strLinesShader);
	if (!shader)
		return;

	const auto& windowManager = world.ReadResource<eng::WindowManager>();
	const eng::Window* window = windowManager.GetWindow(0);
	if (!window)
		return;

	if (!m_Vertices.IsEmpty())
	{
		const Vector2u& windowSize = window->GetSize();
		glViewport(0, 0, windowSize.x, windowSize.y);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		glUseProgram(shader->m_ProgramId);
		glBindVertexArray(m_AttributeObject);

		if (shader->a_Vertex)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

			const uint32 location = *shader->a_Vertex;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0);
			glVertexAttribDivisor(location, GL_FALSE);
		}

		if (shader->a_Colour)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ColourBuffer);

			const uint32 location = *shader->a_Colour;
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Colour), 0);
			glVertexAttribDivisor(location, GL_FALSE);
		}

		const uint32 vertexCount = m_Vertices.GetCount();

		using Query = ecs::query::Include<
			const eng::camera::EditorComponent,
			const eng::camera::ProjectionComponent, 
			const eng::TransformComponent>;
		for (const ecs::Entity& cameraEntity : world.Query<Query>())
		{
			const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
			const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

			const Matrix4x4 cameraProj = eng::camera::GetProjection(cameraComponent.m_Projection, windowSize);
			const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

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
	}
}

void editor::RenderStage_Axes::UpdateBuffers(const bool showX, const bool showY, const bool showZ)
{
	m_Vertices.RemoveAll();
	m_Colours.RemoveAll();
	if (showX)
	{
		m_Vertices.Append(-Vector3f::AxisX * s_Distance);
		m_Vertices.Append(+Vector3f::AxisX * s_Distance);
		m_Colours.Append(s_ColourX);
		m_Colours.Append(s_ColourX);
	}

	if (showY)
	{
		m_Vertices.Append(-Vector3f::AxisY * s_Distance);
		m_Vertices.Append(+Vector3f::AxisY * s_Distance);
		m_Colours.Append(s_ColourY);
		m_Colours.Append(s_ColourY);
	}

	if (showZ)
	{
		m_Vertices.Append(-Vector3f::AxisZ * s_Distance);
		m_Vertices.Append(+Vector3f::AxisZ * s_Distance);
		m_Colours.Append(s_ColourZ);
		m_Colours.Append(s_ColourZ);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_Vertices.GetCount(), m_Vertices.GetData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colour) * m_Colours.GetCount(), m_Colours.GetData(), GL_STATIC_DRAW);
}