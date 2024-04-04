#pragma once

#include <Core/Plane.h>

#include <ECS/Component.h>
#include <ECS/Entity.h>

namespace drag
{
	struct SelectionComponent : public ecs::Component<SelectionComponent>
	{
		ecs::Entity m_CameraEntity = { };
		ecs::Entity m_InputEntity = { };
		ecs::Entity m_SelectedEntity = { };

		Vector3f m_TranslateOffset = { };
		Plane3f m_TranslatePlane = { };
	};
}