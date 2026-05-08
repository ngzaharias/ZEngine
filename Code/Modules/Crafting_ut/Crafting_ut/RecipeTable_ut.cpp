#include <Catch2/catch.hpp>

#include "Crafting/CraftingRecipeTable.h"
#include "Engine/TableHeadmaster.h"

#define CLASS_TEST_CASE(name) TEST_CASE("crafting::RecipeTable. " name, "[crafting::RecipeTable]")

namespace
{
	const str::Guid strRecipeA = GUID64(1);
	const str::Guid strInputA = GUID64(2);
	const str::Guid strInputB = GUID64(3);
	const str::Guid strOutput = GUID64(4);
}

CLASS_TEST_CASE("Test.")
{
	eng::TableHeadmaster headmaster;
	headmaster.Register<crafting::RecipeTable>("Recipes");

	auto& table = headmaster.GetManager<crafting::RecipeTable>();
	{
		crafting::Recipe recipe;
		recipe.m_Input = { strInputA, strInputB };
		recipe.m_Output = strOutput;
		table.AddObject(strRecipeA, recipe);
	}
	
	const str::Guid recipe = table.GetRecipe({ strInputA, strInputB });
	CHECK(recipe == strRecipeA);
}