#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace spellcraft
{
	struct CastComponent final : public ecs::Component
	{
		// the entity it will apply to
		ecs::Entity m_Entity = {};
		// the glyph that will be cast
		str::Guid m_Glyph = {};

		Vector3f m_Translate = {};
		Vector3f m_Direction = {};
	};
}