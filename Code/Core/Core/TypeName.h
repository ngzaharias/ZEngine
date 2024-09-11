#pragma once

#include "Core/StringView.h"

/// \brief Uses static initialization to automatically generate the type name of an object.
/// By calling TypeName<Type>::m_NoNamespace it will return the pre-generated string rather than constructing it inplace.
template<class Type>
class TypeName final
{
public:
	virtual ~TypeName() { m_NoNamespace; }
	static const str::StringView m_NoNamespace;

private:
	static inline str::StringView NoNamespace();
};

#include "Core/TypeName.inl"
