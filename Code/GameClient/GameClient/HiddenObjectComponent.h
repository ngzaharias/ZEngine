#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace hidden
{
	struct SetColour
	{
		Vector3f m_Colour = Vector3f::One;
	};

	struct SetSprite
	{
		str::Guid m_Sprite = { };
	};

	using Effect = Variant<
		SetColour,
		SetSprite>;

	/// \brief Base object to mark which entities are hidden objects.
	struct ObjectComponent : public ecs::Component<ObjectComponent>
	{
		str::Guid m_Group = {};
		Array<Effect> m_Effects = {};
	};
}