#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Plane.h"
#include "Math/Vector.h"

namespace drag
{
	struct SelectionComponent final : public ecs::Component<SelectionComponent>
	{
		ecs::Entity m_CameraEntity = { };
		ecs::Entity m_SelectedEntity = { };

		Vector3f m_TranslateOffset = { };
		Plane3f m_TranslatePlane = { };
	};
}