#pragma once

template<typename Type>
void MemBuffer::Read(Type& data) const
{
	if constexpr (core::IsSpecialization<Type, Array>::value)
	{
		ReadArray(data);
	}
	else if constexpr (std::is_trivially_copyable<Type>::value)
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
	if constexpr (core::IsSpecialization<Type, Array>::value)
	{
		WriteArray(data);
	}
	else if constexpr (std::is_trivially_copyable<Type>::value)
	{
		Write((void*)&data, sizeof(Type));
	}
	else
	{
		WriteCustom<Type>(data);
	}
}

template<typename Type>
void MemBuffer::ReadArray(Array<Type>& data) const
{
	int32 count = 0;
	Read(count);
	data.Resize(count);
	for (int32 i = 0; i < count; ++i)
		Read(data[i]);
}

template<typename Type>
void MemBuffer::WriteArray(const Array<Type>& data)
{
	int32 count = data.GetCount();
	Write(count);
	for (int32 i = 0; i < count; ++i)
		Write(data[i]);
}