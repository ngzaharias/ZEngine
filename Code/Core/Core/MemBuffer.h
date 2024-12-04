#pragma once

#include "Core/Array.h"
#include "Core/Path.h"
#include "Core/String.h"

using int32 = int32_t;
using uint8 = uint8_t;
using uint32 = uint32_t;

class MemBuffer
{
	using Data = Array<uint8>;

public:
	template<typename Type>
	void Read(Type& data) const;
	void Read(str::Path& data) const;
	void Read(str::String& data) const;

	template<typename Type>
	void Write(const Type& data);
	void Write(const str::Path& data);
	void Write(const str::String& data);

	void Read(void* data, uint32 bytes) const;

	void Write(const void* data, uint32 bytes);

	int32 GetCount() const;

	Data& GetData();
	const Data& GetData() const;

private:
	mutable uint32 m_ReadPosition = 0;
	uint32 m_WritePosition = 0;
	Array<uint8> m_Data;
};

#include "MemBuffer.inl"