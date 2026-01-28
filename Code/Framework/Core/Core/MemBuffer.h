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
	uint32 GetBytes() const;
	int32 GetCount() const;

	uint8* GetData();
	const uint8* GetData() const;

	void Reset();

public:
	// Read
	template<typename Type>
	void Read(Type& value) const;
	void Read(MemBuffer& value) const;
	void Read(str::Path& value) const;
	void Read(str::String& value) const;

	void Read(void* data, uint32 bytes) const;

	// Write
	template<typename Type>
	void Write(const Type& value);
	void Write(const MemBuffer& value);
	void Write(const str::Path& value);
	void Write(const str::String& value);

	void Write(const void* data, uint32 bytes);

private:
	template<typename Type>
	inline void ReadCustom(Type& value) const;

	template<typename Type>
	inline void WriteCustom(const Type& value);

private:
	mutable uint32 m_ReadPosition = 0;
	uint32 m_WritePosition = 0;
	Array<uint8> m_Data = {};
};

#include "MemBuffer.inl"