#include "EnginePCH.h"
#include "Engine/StaticMeshComponent.h"

#include "Engine/Visitor.h"

namespace
{
	const str::StringView strStaticMesh = "m_StaticMesh";
}

template<>
void eng::Visitor::ReadCustom(eng::StaticMeshComponent& value) const
{
	Read(strStaticMesh, value.m_StaticMesh, value.m_StaticMesh);
}
template<>
void eng::Visitor::WriteCustom(const eng::StaticMeshComponent& value)
{
	Write(strStaticMesh, value.m_StaticMesh);
}