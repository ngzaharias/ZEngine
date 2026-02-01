#include "SharedHiddenPCH.h"
#include "SharedHidden/HiddenSelectedEvent.h"

#include "Core/MemBuffer.h"

template<>
void MemBuffer::ReadCustom(shared::hidden::SelectedEvent& value) const
{
	Read(value.m_Entity);
}

template<>
void MemBuffer::WriteCustom(const shared::hidden::SelectedEvent& value)
{
	Write(value.m_Entity);
}