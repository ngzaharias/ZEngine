#pragma once

#include "Core/Name.h"
#include "Core/Variant.h"
#include "Engine/TableManager.h"
#include "Math/Vector.h"

namespace tactics
{
	struct Move
	{
		Array<Vector3i> m_Path = {};
	};

	struct Teleport
	{
		Array<Vector3i> m_Path = {};
	};

	using Skill = Variant<
		Move,
		Teleport>;

	class SkillTable final : public eng::TableManager<str::Name, tactics::Skill> { };
}