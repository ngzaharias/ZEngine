#include "CraftingPCH.h"
#include "SharedCrafting/CraftingRecipeTable.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strInput = "m_Input";
	const str::StringView strOutput = "m_Output";
}

template<>
void Visitor::WriteCustom(const shared::crafting::Recipe& value)
{
	Write(strInput, value.m_Input);
	Write(strOutput, value.m_Output);
}
template<>
void Visitor::ReadCustom(shared::crafting::Recipe& value) const
{
	Read(strInput, value.m_Input, value.m_Input);
	Read(strOutput, value.m_Output, value.m_Output);
}

str::Guid shared::crafting::RecipeTable::GetRecipe(const Array<str::Guid> ingredients) const
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