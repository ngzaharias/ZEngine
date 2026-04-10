#include "TabletopPCH.h"
#include "Tabletop/TabletopDeckTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(tabletop::DeckTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const tabletop::DeckTemplate& value)
{
}