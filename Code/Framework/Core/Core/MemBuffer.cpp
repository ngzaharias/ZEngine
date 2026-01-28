#include "Core/MemBuffer.h"

#include "Core/String.h"

void MemBuffer::Reset()
{
	m_Data.RemoveAll();
	m_ReadPosition = 0;
	m_WritePosition = 0;
}

uint32 MemBuffer::GetBytes() const
{
	return static_cast<int32>(m_Data.GetCount());
}

int32 MemBuffer::GetCount() const
{
	return m_Data.GetCount();
}

uint8* MemBuffer::GetData()
{
	return m_Data.GetData();
}

const uint8* MemBuffer::GetData() const
{
	return m_Data.GetData();
}

void MemBuffer::Read(MemBuffer& value) const
{
	int32 count;
	Read(count);

	const uint8* data = GetData();
	data = &data[m_ReadPosition];
	value.Write((const void*)data, count);
	m_ReadPosition += count;
}

void MemBuffer::Read(str::Path& value) const
{
	str::String string;
	Read(string);
	value = string;
}

void MemBuffer::Read(str::String& value) const
{
	uint32 count = 0;
	Read(count);

	value.resize(count);
	Read((void*)value.data(), sizeof(char) * count);
}

void MemBuffer::Read(void* data, uint32 bytes) const
{
	memcpy(data, m_Data.GetData() + m_ReadPosition, bytes);
	m_ReadPosition += bytes;
}

void MemBuffer::Write(const MemBuffer& value)
{
	Write(value.GetCount());
	Write((const void*)value.GetData(), value.GetCount());
}

void MemBuffer::Write(const str::Path& value)
{
	const str::String string = str::String(value.ToView());
	Write(string);
}

void MemBuffer::Write(const str::String& value)
{
	const uint32 count = static_cast<uint32>(value.size());
	Write(count);
	Write((const void*)value.data(), sizeof(char) * count);
}

void MemBuffer::Write(const void* data, uint32 bytes)
{
	m_Data.Resize(m_WritePosition + bytes);
	memcpy(m_Data.GetData() + m_WritePosition, data, bytes);

	m_WritePosition = m_Data.GetCount();
}
