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

void MemBuffer::Read(MemBuffer& data) const
{
	int32 count;
	Read(count);

	data.m_Data.Increase(count);
	for (int32 i = 0; i < count; ++i)
		data.m_Data.Append(m_Data[m_ReadPosition + i]);
	m_ReadPosition += count;
}

void MemBuffer::Read(str::Path& data) const
{
	str::String string;
	Read(string);
	data = string;
}

void MemBuffer::Read(str::String& data) const
{
	uint32 count = 0;
	Read(count);

	data.resize(count);
	Read((void*)data.data(), sizeof(char) * count);
}

void MemBuffer::Read(void* data, uint32 bytes) const
{
	memcpy(data, m_Data.GetData() + m_ReadPosition, bytes);
	m_ReadPosition += bytes;
}

void MemBuffer::Write(const MemBuffer& data)
{
	const int32 count = data.GetCount();
	Write(count);

	m_Data.Increase(count);
	for (int32 i = 0; i < count; ++i)
		m_Data.Append(data.m_Data[i]);
	m_WritePosition += count;
}

void MemBuffer::Write(const str::Path& data)
{
	const str::String string = str::String(data.ToView());
	Write(string);
}

void MemBuffer::Write(const str::String& data)
{
	const uint32 count = static_cast<uint32>(data.size());
	Write(count);
	Write((const void*)data.data(), sizeof(char) * count);
}

void MemBuffer::Write(const void* data, uint32 bytes)
{
	m_Data.Resize(m_WritePosition + bytes);
	memcpy(m_Data.GetData() + m_WritePosition, data, bytes);

	m_WritePosition = m_Data.GetCount();
}
