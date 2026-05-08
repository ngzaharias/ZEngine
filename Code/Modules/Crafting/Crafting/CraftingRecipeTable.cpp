#include "CraftingPCH.h"
#include "Crafting/CraftingRecipeTable.h"

#include "Serialize/Visitor.h"

namespace
{
}

template<>
void Visitor::ReadCustom(crafting::Recipe& value) const
{
}

str::Guid crafting::RecipeTable::GetRecipe(const Array<str::Guid> ingredients) const
{
	for (const auto& [key, recipe] : m_ObjectMap)
	{
		const int32 count = recipe.m_Input.GetCount();
		if (count != ingredients.GetCount())
			continue;

		int32 matches = 0;
		for (int32 i = 0; i < count; ++i)
		{
			const str::Guid& ingredientA = recipe.m_Input[i];
			const str::Guid& ingredientB = ingredients[i];
			if (ingredientA != ingredientB)
				break;

			matches++;
		}

		if (matches != count)
			continue;

		return key;
	}

	return {};
}