#include "TabletopPCH.h"
#include "SharedTabletop/TabletopCardTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(shared::tabletop::CardTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const shared::tabletop::CardTemplate& value)
{
}