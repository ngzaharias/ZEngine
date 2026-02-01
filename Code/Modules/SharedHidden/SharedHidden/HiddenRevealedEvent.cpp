#include "SharedHiddenPCH.h"
#include "SharedHidden/HiddenRevealedEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::hidden::RevealedEvent& value) const
{
	Read(value.m_Entity);
}

template<>
void MemBuffer::WriteCustom(const shared::hidden::RevealedEvent& value)
{
	Write(value.m_Entity);
}