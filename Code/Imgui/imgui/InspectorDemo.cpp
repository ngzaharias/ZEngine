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
	if (isReadOnly) { inspector.Read(label, value); } \
	else { inspector.Write(label, value); } \
}

namespace
{
	enum class EEnum
	{
		First,
		Second,
	};

	struct StructSimple
	{
		bool operator<(const StructSimple& rhs) const
		{
			return m_Int32 < rhs.m_Int32;
		}

		float m_Float = 0.f;
		int32 m_Int32 = 0;
	};

	struct StructRecursive
	{
		bool operator<(const StructRecursive& rhs) const
		{
			return m_Struct < rhs.m_Struct;
		}

		StructSimple m_Struct = {};
	};

	struct StructB
	{
		bool m_Bool = false;
		float m_Float = 0.f;
		double m_Double = 0.0;
		int8 m_Int8 = 0;
		int16 m_Int16 = 0;
		int32 m_Int32 = 0;
		int64 m_Int64 = 0;
		uint8 m_UInt8 = 0;
		uint16 m_UInt16 = 0;
		uint32 m_UInt32 = 0;
		uint64 m_UInt64 = 0;
		EEnum m_Enum = EEnum::First;

		str::Guid m_Guid = {};
		str::Name m_Name = {};
		str::Path m_Path = {};
		str::String m_String = {};

		Quaternion m_Quaternion = {};
		Rotator m_Rotator = {};
		Vector2f m_Vector2f = {};
		Vector2i m_Vector2i = {};
		Vector2u m_Vector2u = {};
		Vector3f m_Vector3f = {};
		Vector3i m_Vector3i = {};
		Vector4f m_Vector4f = {};
		StructSimple m_StructSimple = {};
		StructRecursive m_StructRecursive = {};
		Optional<int32> m_Optional = { 0 };
		Variant<bool, int32> m_Variant = { false };

		Array<int32> m_Array = { 0 };
		Map<str::String, int32> m_Map = { { "A", 0 } };
		Set<int32> m_Set = { 0 };
	};
}

template<>
void imgui::Inspector::ReadCustom(const StructSimple& value)
{
	Read("m_Float", value.m_Float);
	Read("m_Int32", value.m_Int32);
}

template<>
bool imgui::Inspector::WriteCustom(StructSimple& value)
{
	bool result = false;
	result |= Write("m_Float", value.m_Float);
	result |= Write("m_Int32", value.m_Int32);
	return result;
}

template<>
void imgui::Inspector::ReadCustom(const StructRecursive& value)
{
	Read("m_Struct", value.m_Struct);
}

template<>
bool imgui::Inspector::WriteCustom(StructRecursive& value)
{
	bool result = false;
	result |= Write("m_Struct", value.m_Struct);
	return result;
}

template<>
void imgui::Inspector::ReadCustom(const StructB& value)
{
	Read("m_Bool", value.m_Bool);
	Read("m_Float", value.m_Float);
	Read("m_Double", value.m_Double);
	Read("m_Int8", value.m_Int8);
	Read("m_Int16", value.m_Int16);
	Read("m_Int32", value.m_Int32);
	Read("m_Int64", value.m_Int64);
	Read("m_UInt8", value.m_UInt8);
	Read("m_UInt16", value.m_UInt16);
	Read("m_UInt32", value.m_UInt32);
	Read("m_UInt64", value.m_UInt64);
	Read("m_Enum", value.m_Enum);
	Read("m_Guid", value.m_Guid);
	Read("m_Name", value.m_Name);
	Read("m_Path", value.m_Path);
	Read("m_String", value.m_String);
	Read("m_Quaternion", value.m_Quaternion);
	Read("m_Rotator", value.m_Rotator);
	Read("m_Vector2f", value.m_Vector2f);
	Read("m_Vector2i", value.m_Vector2i);
	Read("m_Vector2u", value.m_Vector2u);
	Read("m_Vector3f", value.m_Vector3f);
	Read("m_Vector3i", value.m_Vector3i);
	Read("m_Vector4f", value.m_Vector4f);
	Read("m_Optional", value.m_Optional);
	Read("m_StructSimple", value.m_StructSimple);
	Read("m_StructRecursive", value.m_StructRecursive);
	//Read("m_Variant", value.m_Variant);
	Read("m_Array", value.m_Array);
	Read("m_Map", value.m_Map);
	Read("m_Set", value.m_Set);
}

template<>
bool imgui::Inspector::WriteCustom(StructB& value)
{
	bool result = false;
	result |= Write("m_Bool", value.m_Bool);
	result |= Write("m_Float", value.m_Float);
	result |= Write("m_Double", value.m_Double);
	result |= Write("m_Int8", value.m_Int8);
	result |= Write("m_Int16", value.m_Int16);
	result |= Write("m_Int32", value.m_Int32);
	result |= Write("m_Int64", value.m_Int64);
	result |= Write("m_UInt8", value.m_UInt8);
	result |= Write("m_UInt16", value.m_UInt16);
	result |= Write("m_UInt32", value.m_UInt32);
	result |= Write("m_UInt64", value.m_UInt64);
	result |= Write("m_Enum", value.m_Enum);
	result |= Write("m_Guid", value.m_Guid);
	result |= Write("m_Name", value.m_Name);
	result |= Write("m_Path", value.m_Path);
	result |= Write("m_String", value.m_String);
	result |= Write("m_Quaternion", value.m_Quaternion);
	result |= Write("m_Rotator", value.m_Rotator);
	result |= Write("m_Vector2f", value.m_Vector2f);
	result |= Write("m_Vector2i", value.m_Vector2i);
	result |= Write("m_Vector2u", value.m_Vector2u);
	result |= Write("m_Vector3f", value.m_Vector3f);
	result |= Write("m_Vector3i", value.m_Vector3i);
	result |= Write("m_Vector4f", value.m_Vector4f);
	result |= Write("m_Optional", value.m_Optional);
	result |= Write("m_StructSimple", value.m_StructSimple);
	result |= Write("m_StructRecursive", value.m_StructRecursive);
	//result |= Write("m_Variant", value.m_Variant);
	result |= Write("m_Array", value.m_Array);
	result |= Write("m_Map", value.m_Map);
	result |= Write("m_Set", value.m_Set);
	return result;
}

void imgui::InspectorDemo(bool& isWindowOpen)
{
	if (!ImGui::Begin("Inspector Demo", &isWindowOpen))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	static bool isReadOnly = true;
	ImGui::Checkbox("IsReadOnly", &isReadOnly);

	if (ImGui::BeginTabBar("##read/write"))
	{
		if (ImGui::BeginTabItem("Basic"))
		{
			static bool m_Bool = false;
			static float m_Float = 0.f;
			static double m_Double = 0.0;
			static int8 m_Int8 = 0;
			static int16 m_Int16 = 0;
			static int32 m_Int32 = 0;
			static int64 m_Int64 = 0;
			static uint8 m_UInt8 = 0;
			static uint16 m_UInt16 = 0;
			static uint32 m_UInt32 = 0;
			static uint64 m_UInt64 = 0;
			static EEnum m_Enum = EEnum::First;
			static str::Guid m_Guid = {};
			static str::Name m_Name = {};
			static str::Path m_Path = {};
			static str::String m_String = {};
			static Quaternion m_Quaternion = {};
			static Rotator m_Rotator = {};
			static Vector2f m_Vector2f = {};
			static Vector2i m_Vector2i = {};
			static Vector2u m_Vector2u = {};
			static Vector3f m_Vector3f = {};
			static Vector3i m_Vector3i = {};
			static Vector4f m_Vector4f = {};
			static StructSimple m_StructSimple = {};
			static StructRecursive m_StructRecursive = {};
			static Optional<int32> m_Optional = { 0 };
			static Variant<bool, int32> m_Variant = { false };
			static Array<int32> m_Array = { 0, 1 };
			static Map<str::String, int32> m_Map = { { "A", 0 } };
			static Set<int32> m_Set = { 0 };

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
				VISIT("m_Quaternion", m_Quaternion);
				VISIT("m_Rotator", m_Rotator);
				VISIT("m_Vector2f", m_Vector2f);
				VISIT("m_Vector2i", m_Vector2i);
				VISIT("m_Vector2u", m_Vector2u);
				VISIT("m_Vector3f", m_Vector3f);
				VISIT("m_Vector3i", m_Vector3i);
				VISIT("m_Vector4f", m_Vector4f);
				VISIT("m_StructSimple", m_StructSimple);
				VISIT("m_StructRecursive", m_StructRecursive);
				VISIT("m_Optional", m_Optional);
				//VISIT("m_Variant", m_Variant);
				VISIT("m_Array", m_Array);
				VISIT("m_Map", m_Map);
				VISIT("m_Set", m_Set);
				inspector.End();
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Array"))
		{
			static Array<float> m_Float = { 0.f };
			static Array<double> m_Double = { 0.0 };
			static Array<int8> m_Int8 = { 0 };
			static Array<int16> m_Int16 = { 0 };
			static Array<int32> m_Int32 = { 0 };
			static Array<int64> m_Int64 = { 0 };
			static Array<uint8> m_UInt8 = { 0 };
			static Array<uint16> m_UInt16 = { 0 };
			static Array<uint32> m_UInt32 = { 0 };
			static Array<uint64> m_UInt64 = { 0 };
			static Array<EEnum> m_Enum = { EEnum::First };
			static Array<str::Guid> m_Guid = { {} };
			static Array<str::Name> m_Name = { {} };
			static Array<str::Path> m_Path = { {} };
			static Array<str::String> m_String = { {} };
			static Array<Quaternion> m_Quaternion = { {} };
			static Array<Rotator> m_Rotator = { {} };
			static Array<Vector2f> m_Vector2f = { {} };
			static Array<Vector2i> m_Vector2i = { {} };
			static Array<Vector2u> m_Vector2u = { {} };
			static Array<Vector3f> m_Vector3f = { {} };
			static Array<Vector3i> m_Vector3i = { {} };
			static Array<Vector4f> m_Vector4f = { {} };
			static Array<StructSimple> m_StructSimple = { {}, {} };
			static Array<StructRecursive> m_StructRecursive = { {} };
			static Array<Optional<int32>> m_Optional = { 0 };
			static Array<Variant<bool, int32>> m_Variant = { false };
			static Array<Array<int32>> m_Array = { { 0 } };
			static Array<Map<str::String, int32>> m_Map = { { { "A", 0 } } };
			static Array<Set<int32>> m_Set = { { 0 } };

			imgui::Inspector inspector;
			if (inspector.Begin("##table"))
			{
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
				VISIT("m_Quaternion", m_Quaternion);
				VISIT("m_Rotator", m_Rotator);
				VISIT("m_Vector2f", m_Vector2f);
				VISIT("m_Vector2i", m_Vector2i);
				VISIT("m_Vector2u", m_Vector2u);
				VISIT("m_Vector3f", m_Vector3f);
				VISIT("m_Vector3i", m_Vector3i);
				VISIT("m_Vector4f", m_Vector4f);
				VISIT("m_StructSimple", m_StructSimple);
				VISIT("m_StructRecursive", m_StructRecursive);
				VISIT("m_Optional", m_Optional);
				//VISIT("m_Variant", m_Variant);
				VISIT("m_Array", m_Array);
				VISIT("m_Map", m_Map);
				VISIT("m_Set", m_Set);
				inspector.End();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Map"))
		{
			static Map<str::String, bool> m_Bool = { { "A", 0.f } };
			static Map<str::String, float> m_Float = { { "A", 0.f } };
			static Map<str::String, double> m_Double = { { "A", 0.0 } };
			static Map<str::String, int8> m_Int8 = { { "A", 0 } };
			static Map<str::String, int16> m_Int16 = { { "A", 0 } };
			static Map<str::String, int32> m_Int32 = { { "A", 0 } };
			static Map<str::String, int64> m_Int64 = { { "A", 0 } };
			static Map<str::String, uint8> m_UInt8 = { { "A", 0 } };
			static Map<str::String, uint16> m_UInt16 = { { "A", 0 } };
			static Map<str::String, uint32> m_UInt32 = { { "A", 0 } };
			static Map<str::String, uint64> m_UInt64 = { { "A", 0 } };
			static Map<str::String, EEnum> m_Enum = { { "A", EEnum::First } };
			static Map<str::String, str::Guid> m_Guid = { { "A", {} } };
			static Map<str::String, str::Name> m_Name = { { "A", {} } };
			static Map<str::String, str::Path> m_Path = { { "A", {} } };
			static Map<str::String, str::String> m_String = { { "A", {} } };
			static Map<str::String, Quaternion> m_Quaternion = { { "A", {} } };
			static Map<str::String, Rotator> m_Rotator = { { "A", {} } };
			static Map<str::String, Vector2f> m_Vector2f = { { "A", {} } };
			static Map<str::String, Vector2i> m_Vector2i = { { "A", {} } };
			static Map<str::String, Vector2u> m_Vector2u = { { "A", {} } };
			static Map<str::String, Vector3f> m_Vector3f = { { "A", {} } };
			static Map<str::String, Vector3i> m_Vector3i = { { "A", {} } };
			static Map<str::String, Vector4f> m_Vector4f = { { "A", {} } };
			static Map<str::String, StructSimple> m_StructSimple = { { "A", {} } };
			static Map<str::String, StructRecursive> m_StructRecursive = { { "A", {} } };
			static Map<str::String, Optional<int32>> m_Optional = { { "A", 0 } };
			static Map<str::String, Variant<bool, int32>> m_Variant = { { "A", false } };
			static Map<str::String, Array<int32>> m_Array = { { "A", { 0 } } };
			static Map<str::String, Map<str::String, int32>> m_Map = { { "A", { { "A", 0 } } } };
			static Map<str::String, Set<int32>> m_Set = { { "A", { 0 } } };

			imgui::Inspector inspector;
			if (inspector.Begin("##table"))
			{
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
				VISIT("m_Quaternion", m_Quaternion);
				VISIT("m_Rotator", m_Rotator);
				VISIT("m_Vector2f", m_Vector2f);
				VISIT("m_Vector2i", m_Vector2i);
				VISIT("m_Vector2u", m_Vector2u);
				VISIT("m_Vector3f", m_Vector3f);
				VISIT("m_Vector3i", m_Vector3i);
				VISIT("m_Vector4f", m_Vector4f);
				VISIT("m_StructSimple", m_StructSimple);
				VISIT("m_StructRecursive", m_StructRecursive);
				VISIT("m_Optional", m_Optional);
				//VISIT("m_Variant", m_Variant);
				VISIT("m_Array", m_Array);
				VISIT("m_Map", m_Map);
				VISIT("m_Set", m_Set);
				inspector.End();
			}
			ImGui::EndTabItem();
		}

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

		if (ImGui::BeginTabItem("Struct"))
		{
			static StructB m_StructSimple = {};

			imgui::Inspector inspector;
			if (inspector.Begin("##table"))
			{
				VISIT("m_StructSimple", m_StructSimple);
				inspector.End();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Optional"))
		{
			static Optional<bool> m_Bool = false;
			static Optional<float> m_Float = 0.f;
			static Optional<double> m_Double = 0.0;
			static Optional<int8> m_Int8 = 0;
			static Optional<int16> m_Int16 = 0;
			static Optional<int32> m_Int32 = 0;
			static Optional<int64> m_Int64 = 0;
			static Optional<uint8> m_UInt8 = 0;
			static Optional<uint16> m_UInt16 = 0;
			static Optional<uint32> m_UInt32 = 0;
			static Optional<uint64> m_UInt64 = 0;
			static Optional<EEnum> m_Enum = EEnum::First;
			static Optional<str::Guid> m_Guid = { str::Guid::Unassigned };
			static Optional<str::Name> m_Name = { str::Name::Unassigned };
			static Optional<str::Path> m_Path = { "" };
			static Optional<str::String> m_String = { "" };
			static Optional<Quaternion> m_Quaternion = Quaternion::Identity;
			static Optional<Rotator> m_Rotator = Rotator::Zero;
			static Optional<Vector2f> m_Vector2f = Vector2f::Zero;
			static Optional<Vector2i> m_Vector2i = Vector2i::Zero;
			static Optional<Vector2u> m_Vector2u = Vector2u::Zero;
			static Optional<Vector3f> m_Vector3f = Vector3f::Zero;
			static Optional<Vector3i> m_Vector3i = Vector3i::Zero;
			static Optional<Vector4f> m_Vector4f = Vector4f::Zero;
			static Optional<StructSimple> m_StructSimple = StructSimple{};
			static Optional<StructRecursive> m_StructRecursive = StructRecursive{};
			static Optional<Variant<bool, int32>> m_Variant = false;
			static Optional<Array<int32>> m_Array = Array<int32>{ 0 };
			static Optional<Map<str::String, int32>> m_Map = Map<str::String, int32>{ { "A", 0 } };
			static Optional<Set<int32>> m_Set = Set<int32>{ 0 };

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
				VISIT("m_Quaternion", m_Quaternion);
				VISIT("m_Rotator", m_Rotator);
				VISIT("m_Vector2f", m_Vector2f);
				VISIT("m_Vector2i", m_Vector2i);
				VISIT("m_Vector2u", m_Vector2u);
				VISIT("m_Vector3f", m_Vector3f);
				VISIT("m_Vector3i", m_Vector3i);
				VISIT("m_Vector4f", m_Vector4f);
				VISIT("m_StructSimple", m_StructSimple);
				VISIT("m_StructRecursive", m_StructRecursive);
				//VISIT("m_Variant", m_Variant);
				VISIT("m_Array", m_Array);
				VISIT("m_Map", m_Map);
				VISIT("m_Set", m_Set);
				inspector.End();
			}
			ImGui::EndTabItem();
		}

		//if (ImGui::BeginTabItem("Variant"))
		//{
		//	static Variant<bool, float> m_Float = 0.f;
		//	static Variant<bool, double> m_Double = 0.0;
		//	static Variant<bool, int8> m_Int8 = int8(0);
		//	static Variant<bool, int16> m_Int16 = int16(0);
		//	static Variant<bool, int32> m_Int32 = int32(0);
		//	static Variant<bool, int64> m_Int64 = int64(0);
		//	static Variant<bool, uint8> m_UInt8 = uint8(0);
		//	static Variant<bool, uint16> m_UInt16 = uint16(0);
		//	static Variant<bool, uint32> m_UInt32 = uint32(0);
		//	static Variant<bool, uint64> m_UInt64 = uint64(0);
		//	static Variant<bool, EEnum> m_Enum = EEnum::First;
		//	static Variant<bool, str::Guid> m_Guid = { str::Guid::Unassigned };
		//	static Variant<bool, str::Name> m_Name = { str::Name::Unassigned };
		//	static Variant<bool, str::Path> m_Path = { "" };
		//	static Variant<bool, str::String> m_String = { "" };
		//	static Variant<bool, Quaternion> m_Quaternion = Quaternion::Identity;
		//	static Variant<bool, Rotator> m_Rotator = Rotator::Zero;
		//	static Variant<bool, Vector2f> m_Vector2f = Vector2f::Zero;
		//	static Variant<bool, Vector2i> m_Vector2i = Vector2i::Zero;
		//	static Variant<bool, Vector2u> m_Vector2u = Vector2u::Zero;
		//	static Variant<bool, Vector3f> m_Vector3f = Vector3f::Zero;
		//	static Variant<bool, Vector3i> m_Vector3i = Vector3i::Zero;
		//	static Variant<bool, Vector4f> m_Vector4f = Vector4f::Zero;
		//	static Variant<bool, StructSimple> m_StructSimple = StructSimple{};
		//	static Variant<bool, StructRecursive> m_StructRecursive = StructRecursive{};
		//	static Variant<bool, Optional<int32>> m_Optional = 0;
		//	static Variant<bool, Variant<bool, int32>> m_Variant = 0;
		//	static Variant<bool, Array<int32>> m_Array = Array<int32>{ 0 };
		//	static Variant<bool, Map<str::String, int32>> m_Map = Map<str::String, int32>{ { "A", 0 } };
		//	static Variant<bool, Set<int32>> m_Set = Set<int32>{ 0 };

		//	imgui::Inspector inspector;
		//	if (inspector.Begin("##table"))
		//	{
		//		VISIT("m_Float", m_Float);
		//		VISIT("m_Double", m_Double);
		//		VISIT("m_Int8", m_Int8);
		//		VISIT("m_Int16", m_Int16);
		//		VISIT("m_Int32", m_Int32);
		//		VISIT("m_Int64", m_Int64);
		//		VISIT("m_UInt8", m_UInt8);
		//		VISIT("m_UInt16", m_UInt16);
		//		VISIT("m_UInt32", m_UInt32);
		//		VISIT("m_UInt64", m_UInt64);
		//		VISIT("m_Enum", m_Enum);
		//		VISIT("m_Guid", m_Guid);
		//		VISIT("m_Name", m_Name);
		//		VISIT("m_Path", m_Path);
		//		VISIT("m_String", m_String);
		//		VISIT("m_Quaternion", m_Quaternion);
		//		VISIT("m_Rotator", m_Rotator);
		//		VISIT("m_Vector2f", m_Vector2f);
		//		VISIT("m_Vector2i", m_Vector2i);
		//		VISIT("m_Vector2u", m_Vector2u);
		//		VISIT("m_Vector3f", m_Vector3f);
		//		VISIT("m_Vector3i", m_Vector3i);
		//		VISIT("m_Vector4f", m_Vector4f);
		//		VISIT("m_StructSimple", m_StructSimple);
		//		VISIT("m_StructRecursive", m_StructRecursive);
		//		VISIT("m_Optional", m_Optional);
		//		VISIT("m_Variant", m_Variant);
		//		VISIT("m_Array", m_Array);
		//		VISIT("m_Map", m_Map);
		//		VISIT("m_Set", m_Set);
		//		inspector.End();
		//	}

		//	ImGui::EndTabItem();
		//}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

#undef VISIT