#include "EnginePCH.h"
#include "Engine/StaticMeshComponent.h"

#include "Engine/InspectorHelpers.h"
#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

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
template<>
bool imgui::Inspector::WriteCustom(eng::StaticMeshComponent& value)
{
	bool result = false;
	if (!HasPayload<eng::AssetManager>())
	{
		result |= Write("m_StaticMesh", value.m_StaticMesh);
	}
	else
	{
		const auto& manager = GetPayload<eng::AssetManager>();
		result |= WriteStaticMesh(manager, "m_StaticMesh", value.m_StaticMesh);
	}
	return result;
}