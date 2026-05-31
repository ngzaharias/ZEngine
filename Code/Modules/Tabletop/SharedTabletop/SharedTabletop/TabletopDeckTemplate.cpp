#include "TabletopPCH.h"
#include "SharedTabletop/TabletopDeckTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::tabletop::DeckTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const shared::tabletop::DeckTemplate& value)
{
}