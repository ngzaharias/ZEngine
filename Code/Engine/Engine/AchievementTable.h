#pragma once

#include "Core/Guid.h"
#include "Core/Name.h"
#include "Core/String.h"
#include "Engine/TableManager.h"

namespace eng
{
	struct Achievement
	{
		str::Name m_Identifier = {};
	};

	class AchievementTable final : public eng::TableManager<str::Guid, eng::Achievement> { };
}