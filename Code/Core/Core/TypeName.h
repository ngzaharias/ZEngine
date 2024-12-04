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
	static const str::StringView m_WithNamespace;

private:
	static inline str::StringView NoNamespace();
	static inline str::StringView WithNamespace();
};

#include "Core/TypeName.inl"
