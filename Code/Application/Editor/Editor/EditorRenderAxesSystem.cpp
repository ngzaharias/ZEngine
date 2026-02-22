#include "EditorPCH.h"
#include "Editor/EditorRenderAxesSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsLocalSingleton.h"
#include "Engine/AssetManager.h"
#include "Engine/CameraComponent.h"
#include "Engine/CameraHelpers.h"
#include "Engine/LinesComponent.h"
#include "Engine/ShaderAsset.h"
#include "Engine/TransformComponent.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"
#include "GameState/GameStateEditModeComponent.h"
#include "GameState/GameStateEditorComponent.h"
#include "Math/Matrix.h"

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

void editor::RenderAxesSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_ColourBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset(strLinesShader);

	UpdateBuffers(world);
}

void editor::RenderAxesSystem::Shutdown(World& world)
{
	PROFILE_FUNCTION();

	glDeleteVertexArrays(1, &m_AttributeObject);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_ColourBuffer);

	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset(strLinesShader);
}

void editor::RenderAxesSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!world.HasAny<ecs::query::Include<gamestate::EditorComponent>>())
		return;
	if (!world.HasAny<ecs::query::Include<gamestate::EditModeComponent>>())
		return;

	if (world.HasAny<editor::settings::LocalSingleton>())
	{
		UpdateBuffers(world);
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

		using Query = ecs::query
			::Include<
			const eng::ActiveComponent,
			const eng::CameraComponent,
			const eng::TransformComponent>;
		for (auto&& view : world.Query<Query>())
		{
			const auto& cameraComponent = view.ReadRequired<eng::CameraComponent>();
			const auto& cameraTransform = view.ReadRequired<eng::TransformComponent>();

			const Matrix4x4 cameraProj = eng::GetProjection(cameraComponent.m_Projection, windowSize);
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

void editor::RenderAxesSystem::UpdateBuffers(World& world)
{
	PROFILE_FUNCTION();

	const auto& settings = world.ReadSingleton<editor::settings::LocalSingleton>();
	const auto& axes = settings.m_Gizmos.m_CoordAxes;

	m_Vertices.RemoveAll();
	m_Colours.RemoveAll();
	if (axes.m_ShowX)
	{
		m_Vertices.Append(-Vector3f::AxisX * s_Distance);
		m_Vertices.Append(+Vector3f::AxisX * s_Distance);
		m_Colours.Append(axes.m_ColourX);
		m_Colours.Append(axes.m_ColourX);
	}

	if (axes.m_ShowY)
	{
		m_Vertices.Append(-Vector3f::AxisY * s_Distance);
		m_Vertices.Append(+Vector3f::AxisY * s_Distance);
		m_Colours.Append(axes.m_ColourY);
		m_Colours.Append(axes.m_ColourY);
	}

	if (axes.m_ShowZ)
	{
		m_Vertices.Append(-Vector3f::AxisZ * s_Distance);
		m_Vertices.Append(+Vector3f::AxisZ * s_Distance);
		m_Colours.Append(axes.m_ColourZ);
		m_Colours.Append(axes.m_ColourZ);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_Vertices.GetCount(), m_Vertices.GetData(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colour) * m_Colours.GetCount(), m_Colours.GetData(), GL_STATIC_DRAW);
}