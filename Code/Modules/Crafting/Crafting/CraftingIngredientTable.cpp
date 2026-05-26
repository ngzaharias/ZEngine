#include "CraftingPCH.h"
#include "Crafting/CraftingIngredientTable.h"

#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strName = "m_Name";
}

template<>
void Visitor::WriteCustom(const crafting::Ingredient& value)
{
	Write(strName, value.m_Name);
}
template<>
void Visitor::ReadCustom(crafting::Ingredient& value) const
{
	Read(strName, value.m_Name, value.m_Name);
}