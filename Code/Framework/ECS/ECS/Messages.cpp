#include "ECS/Messages.h"

//////////////////////////////////////////////////////////////////////////
// Entity

template<>
void MemBuffer::ReadCustom(ecs::EntityCreateMessage& value) const
{
	Read(value.m_Entity);
}
template<>
void MemBuffer::WriteCustom(const ecs::EntityCreateMessage& value)
{
	Write(value.m_Entity);
}

template<>
void MemBuffer::ReadCustom(ecs::EntityDestroyMessage& value) const
{
	Read(value.m_Entity);
}
template<>
void MemBuffer::WriteCustom(const ecs::EntityDestroyMessage& value)
{
	Write(value.m_Entity);
}

//////////////////////////////////////////////////////////////////////////
// Component

template<>
void MemBuffer::ReadCustom(ecs::ComponentAddMessage& value) const
{
	Read(value.m_TypeId);
	Read(value.m_Entity);
	Read(value.m_Data);
}
template<>
void MemBuffer::WriteCustom(const ecs::ComponentAddMessage& value)
{
	Write(value.m_TypeId);
	Write(value.m_Entity);
	Write(value.m_Data);
}

template<>
void MemBuffer::ReadCustom(ecs::ComponentUpdateMessage& value) const
{
	Read(value.m_TypeId);
	Read(value.m_Entity);
	Read(value.m_Data);
}
template<>
void MemBuffer::WriteCustom(const ecs::ComponentUpdateMessage& value)
{
	Write(value.m_TypeId);
	Write(value.m_Entity);
	Write(value.m_Data);
}

template<>
void MemBuffer::ReadCustom(ecs::ComponentRemoveMessage& value) const
{
	Read(value.m_TypeId);
	Read(value.m_Entity);
}
template<>
void MemBuffer::WriteCustom(const ecs::ComponentRemoveMessage& value)
{
	Write(value.m_TypeId);
	Write(value.m_Entity);
}

//////////////////////////////////////////////////////////////////////////
// Event

template<>
void MemBuffer::ReadCustom(ecs::EventAddMessage& value) const
{
	Read(value.m_TypeId);
	Read(value.m_Data);
}
template<>
void MemBuffer::WriteCustom(const ecs::EventAddMessage& value)
{
	Write(value.m_TypeId);
	Write(value.m_Data);
}