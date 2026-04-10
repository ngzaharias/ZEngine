#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenCountTemplate.h"

#include "Serialize/Visitor.h"

template<>
void Visitor::ReadCustom(client::hidden::CountTemplate& value) const
{
}
template<>
void Visitor::WriteCustom(const client::hidden::CountTemplate& value)
{
}