#pragma once

#include "ECS/Component.h"

namespace editor::gizmo
{
	enum class ETransformOper
	{
		Translate,
		Rotate,
		Scale,
	};

	enum class ETransformType
	{
		Physics,
		Transform,
	};

	struct TransformComponent final : public ecs::StaticComponent
	{
		ETransformOper m_TransformOper = ETransformOper::Translate;
		ETransformType m_TransformType = ETransformType::Transform;
	};
}
