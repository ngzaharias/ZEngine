#include "Core/MemBuffer.h"

#include "Core/String.h"

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

void MemBuffer::Read(void* data, uint32 bytes) const
{
	memcpy(data, m_Data.GetData() + m_ReadPosition, bytes);
	m_ReadPosition += bytes;
}

void MemBuffer::Write(const void* data, uint32 bytes)
{
	m_Data.Resize(m_WritePosition + bytes);
	memcpy(m_Data.GetData() + m_WritePosition, data, bytes);

	m_WritePosition = m_Data.GetCount();
}

int32 MemBuffer::GetCount() const 
{ 
	return m_Data.GetCount(); 
}

MemBuffer::Data& MemBuffer::GetData() 
{ 
	return m_Data; 
}

const MemBuffer::Data& MemBuffer::GetData() const 
{ 
	return m_Data; 
}
