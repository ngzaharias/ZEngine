#include "TabletopPCH.h"
#include "SharedTabletop/TabletopCardTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(tabletop::CardTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const tabletop::CardTemplate& value)
{
}