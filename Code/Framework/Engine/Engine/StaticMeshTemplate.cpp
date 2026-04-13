#include "EnginePCH.h"
#include "Engine/StaticMeshTemplate.h"

#include "Engine/InspectorHelpers.h"
#include "Serialize/Visitor.h"

namespace
{
	const str::StringView strStaticMesh = "m_StaticMesh";
}

template<>
void Visitor::ReadCustom(eng::StaticMeshTemplate& value) const
{
	Read(strStaticMesh, value.m_StaticMesh, value.m_StaticMesh);
}
template<>
void Visitor::WriteCustom(const eng::StaticMeshTemplate& value)
{
	Write(strStaticMesh, value.m_StaticMesh);
}