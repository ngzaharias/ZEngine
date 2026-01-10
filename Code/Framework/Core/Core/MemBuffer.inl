#pragma once

template<typename Type>
void MemBuffer::Read(Type& data) const
{
	static_assert(std::is_trivially_copyable<Type>::value, "Error");
	Read((void*)&data, sizeof(Type));
}

template<typename Type>
void MemBuffer::Write(const Type& data)
{
	static_assert(std::is_trivially_copyable<Type>::value, "Error");
	Write((void*)&data, sizeof(Type));
}