#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "Engine/TableManager.h"

namespace crafting
{
	struct Ingredient
	{
		str::String m_Name = {};
	};

	class IngredientTable final : public eng::TableManager<str::Guid, crafting::Ingredient>
	{
	};
}