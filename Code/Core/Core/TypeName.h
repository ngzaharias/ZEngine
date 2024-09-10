#pragma once

#include "Core/StringView.h"

template<class Type>
class TypeName final
{
public:
	virtual ~TypeName() { m_NoNamespace; }
	static inline str::StringView NoNamespace();
	static str::StringView m_NoNamespace;
};

#include "Core/TypeName.inl"
