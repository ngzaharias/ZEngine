#pragma once

#include "ECS/Singleton.h"

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

	struct TransformSingleton final : public ecs::Singleton<TransformSingleton>
	{
		ETransformOper m_TransformOper = ETransformOper::Translate;
		ETransformType m_TransformType = ETransformType::Transform;
	};
}
