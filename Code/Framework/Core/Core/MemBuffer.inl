#pragma once

template<typename Type>
void MemBuffer::Read(Type& data) const
{
	if constexpr (std::is_trivially_copyable<Type>::value)
	{
		Read((void*)&data, sizeof(Type));
	}
	else
	{
		ReadCustom<Type>(data);
	}
}

template<typename Type>
void MemBuffer::Write(const Type& data)
{
	if constexpr (std::is_trivially_copyable<Type>::value)
	{
		Write((void*)&data, sizeof(Type));
	}
	else
	{
		WriteCustom<Type>(data);
	}
}