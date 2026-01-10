#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "Core/String.h"

using int32 = int32_t;
using uint8 = uint8_t;
using uint32 = uint32_t;

class MemBuffer
{
public:
	int32 GetCount() const;

	uint8* GetData();
	const uint8* GetData() const;

public:
	// Read
	template<typename Type>
	void Read(Type& data) const;
	void Read(str::Path& data) const;
	void Read(str::String& data) const;
	void Read(void* data, uint32 bytes) const;

	// Write
	template<typename Type>
	void Write(const Type& data);
	void Write(const str::Path& data);
	void Write(const str::String& data);
	void Write(const void* data, uint32 bytes);

private:
	mutable uint32 m_ReadPosition = 0;
	uint32 m_WritePosition = 0;
	Array<uint8> m_Data = {};
};

#include "MemBuffer.inl"