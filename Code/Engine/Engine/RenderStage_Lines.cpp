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
#include "Engine/TransformComponent.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace
{
	const str::Guid strLinesShader = GUID("dad72cc0-7659-496b-83b1-c5a85a4b3695");
}

void eng::RenderStage_Lines::Initialise(ecs::EntityWorld& entityWorld)
{
	glGenVertexArrays(1, &m_AttributeObject);
	glBindVertexArray(m_AttributeObject);
	glGenBuffers(1, &m_VertexBuffer);
}

void eng::RenderStage_Lines::Shutdown(ecs::EntityWorld& entityWorld)
{
}

void eng::RenderStage_Lines::Render(ecs::EntityWorld& entityWorld)
{
	PROFILE_FUNCTION();

	World world = entityWorld.GetWorldView<World>();
	auto& assetManager = world.WriteResource<eng::AssetManager>();

	{
		glViewport(0, 0, static_cast<int32>(Screen::width), static_cast<int32>(Screen::height));

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
	}

	const auto* linesShader = assetManager.LoadAsset<eng::ShaderAsset>(strLinesShader);
	if (!linesShader)
		return;

	for (const ecs::Entity& cameraEntity : world.Query<ecs::query::Include<const eng::camera::ProjectionComponent, const eng::TransformComponent>>())
	{
		const auto& cameraComponent = world.ReadComponent<eng::camera::ProjectionComponent>(cameraEntity);
		const auto& cameraTransform = world.ReadComponent<eng::TransformComponent>(cameraEntity);

		const Vector2u screenSize = Vector2u(static_cast<uint32>(Screen::width), static_cast<uint32>(Screen::height));
		const Matrix4x4 cameraProj = eng::camera::GetProjection(screenSize, cameraComponent.m_Projection);
		const Matrix4x4 cameraView = cameraTransform.ToTransform().Inversed();

		constexpr size_t s_PointOffset = offsetof(LineVertex, m_Point);
		constexpr size_t s_ColourOffset = offsetof(LineVertex, m_Colour);

		glUseProgram(linesShader->m_ProgramId);

		{
			auto& linesComponent = world.WriteSingleton<eng::LinesComponent>();
			if (!linesComponent.m_Vertices.IsEmpty())
			{
				const uint32 lineCount = linesComponent.m_Vertices.GetCount();

				glBindVertexArray(m_AttributeObject);

				const uint32 transformId = glGetUniformLocation(linesShader->m_ProgramId, "u_Transform");
				const Matrix4x4 transform = cameraView * cameraProj;
				glUniformMatrix4fv(transformId, 1, GL_FALSE, &transform[0][0]);

				glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, lineCount * sizeof(LineVertex), &linesComponent.m_Vertices[0], GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_PointOffset);
				glVertexAttribDivisor(0, GL_FALSE);

				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (void*)s_ColourOffset);
				glVertexAttribDivisor(1, GL_FALSE);

				glDrawArrays(GL_LINES, 0, lineCount);

				linesComponent.m_Vertices.RemoveAll();
			}
		}
	}
}