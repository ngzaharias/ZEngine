#pragma once

#include "Core/Name.h"
#include "Core/Variant.h"
#include "Engine/TableManager.h"
#include "Math/Vector.h"

namespace tactics
{
	struct Path
	{
		Array<Vector3i> m_Nodes = {};
	};

	struct Rush
	{
		Vector3i m_Direction = Vector3i::Zero;
	};

	struct Teleport
	{
		Vector3i m_Offset = Vector3i::Zero;
	};

	using Ability = Variant<
		Path,
		Rush,
		Teleport>;

	class AbilityTable final : public eng::TableManager<str::Name, tactics::Ability> { };
}