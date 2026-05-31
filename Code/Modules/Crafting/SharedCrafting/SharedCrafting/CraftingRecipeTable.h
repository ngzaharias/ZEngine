#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "Engine/TableManager.h"

namespace shared::crafting
{
	struct Recipe
	{
		Array<str::Guid> m_Input = {};
		str::Guid m_Output = {};
	};

	class RecipeTable final : public eng::TableManager<str::Guid, shared::crafting::Recipe>
	{
	public:
		str::Guid GetRecipe(const Array<str::Guid> ingredients) const;
	};
}