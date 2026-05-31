#include "TabletopPCH.h"
#include "SharedTabletop/TabletopDeckTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(tabletop::DeckTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const tabletop::DeckTemplate& value)
{
}