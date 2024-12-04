#include "imgui/InspectorDemo.h"

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/Name.h"
#include "Core/Optional.h"
#include "Core/Path.h"
#include "Core/Set.h"
#include "Core/String.h"
#include "Core/Variant.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

#define VISIT(label, value) \
{ \
	if (isReading) { inspector.Read(label, value); } \
	else { inspector.Write(label, value); } \
}

void imgui::InspectorDemo_Set(bool isReading)
{
	if (ImGui::BeginTabItem("Set"))
	{
		static Set<bool> m_Bool = { false };
		static Set<float> m_Float = { 0.f };
		static Set<double> m_Double = { 0.0 };
		static Set<int8> m_Int8 = { 0 };
		static Set<int16> m_Int16 = { 0 };
		static Set<int32> m_Int32 = { 0 };
		static Set<int64> m_Int64 = { 0 };
		static Set<uint8> m_UInt8 = { 0 };
		static Set<uint16> m_UInt16 = { 0 };
		static Set<uint32> m_UInt32 = { 0 };
		static Set<uint64> m_UInt64 = { 0 };
		static Set<EEnum> m_Enum = { EEnum::First };
		static Set<str::Guid> m_Guid = { {} };
		static Set<str::Name> m_Name = { {} };
		static Set<str::Path> m_Path = { {} };
		static Set<str::String> m_String = { {} };
		static Set<StructSimple> m_StructSimple = { {} };
		static Set<StructRecursive> m_StructRecursive = { {} };

		imgui::Inspector inspector;
		if (inspector.Begin("##table"))
		{
			VISIT("m_Bool", m_Bool);
			VISIT("m_Float", m_Float);
			VISIT("m_Double", m_Double);
			VISIT("m_Int8", m_Int8);
			VISIT("m_Int16", m_Int16);
			VISIT("m_Int32", m_Int32);
			VISIT("m_Int64", m_Int64);
			VISIT("m_UInt8", m_UInt8);
			VISIT("m_UInt16", m_UInt16);
			VISIT("m_UInt32", m_UInt32);
			VISIT("m_UInt64", m_UInt64);
			VISIT("m_Enum", m_Enum);
			VISIT("m_Guid", m_Guid);
			VISIT("m_Name", m_Name);
			VISIT("m_Path", m_Path);
			VISIT("m_String", m_String);
			VISIT("m_StructSimple", m_StructSimple);
			VISIT("m_StructRecursive", m_StructRecursive);
			inspector.End();
		}
		ImGui::EndTabItem();
	}
}

#undef VISIT