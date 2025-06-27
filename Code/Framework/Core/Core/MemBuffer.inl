#pragma once

template<typename Type>
void MemBuffer::Read(Type& data) const
{
	Read((void*)&data, sizeof(Type));
}

template<typename Type>
void MemBuffer::Write(const Type& data)
{
	Write((void*)&data, sizeof(Type));
}