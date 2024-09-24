#include "EnginePCH.h"
#include "Engine/PhysicsComponent.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strChannel = "m_Channel";
	const str::StringView strExtents = "m_Extents";
	const str::StringView strRadius = "m_Radius";
	const str::StringView strRigidbody = "m_Rigidbody";
	const str::StringView strRotate = "m_Rotate";
	const str::StringView strShape = "m_Shape";
	const str::StringView strShapes = "m_Shapes";
	const str::StringView strTranslate = "m_Translate";

	const str::StringView strENABLE_CCD = "eENABLE_CCD";
	const str::StringView strKINEMATIC = "eKINEMATIC";
	const str::StringView strLOCK_ANGULAR_X = "eLOCK_ANGULAR_X";
	const str::StringView strLOCK_ANGULAR_Y = "eLOCK_ANGULAR_Y";
	const str::StringView strLOCK_ANGULAR_Z = "eLOCK_ANGULAR_Z";
	const str::StringView strLOCK_LINEAR_X = "eLOCK_LINEAR_X";
	const str::StringView strLOCK_LINEAR_Y = "eLOCK_LINEAR_Y";
	const str::StringView strLOCK_LINEAR_Z = "eLOCK_LINEAR_Z";
}

template<>
void eng::Visitor::ReadCustom(eng::PhysicsComponent& value) const
{
	Read(strRigidbody, value.m_Rigidbody, value.m_Rigidbody);
	Read(strShapes, value.m_Shapes, value.m_Shapes);
}

template<>
void eng::Visitor::WriteCustom(const eng::PhysicsComponent& value)
{
	Write(strRigidbody, value.m_Rigidbody);
	Write(strShapes, value.m_Shapes);
}

template<>
bool imgui::Inspector::WriteCustom(eng::PhysicsComponent& value)
{
	bool result = false;
	result |= Write("m_Rigidbody", value.m_Rigidbody);
	result |= Write("m_Shapes", value.m_Shapes);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::RigidDynamic& value) const
{
	Read(strENABLE_CCD, value.eENABLE_CCD, value.eENABLE_CCD);
	Read(strKINEMATIC, value.eKINEMATIC, value.eKINEMATIC);
	Read(strLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X);
	Read(strLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y);
	Read(strLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z);
	Read(strLOCK_LINEAR_X, value.eLOCK_LINEAR_X, value.eLOCK_LINEAR_X);
	Read(strLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y);
	Read(strLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z);
}

template<>
void eng::Visitor::WriteCustom(const eng::RigidDynamic& value)
{
	Write(strENABLE_CCD, value.eENABLE_CCD);
	Write(strKINEMATIC, value.eKINEMATIC);
	Write(strLOCK_ANGULAR_X, value.eLOCK_ANGULAR_X);
	Write(strLOCK_ANGULAR_Y, value.eLOCK_ANGULAR_Y);
	Write(strLOCK_ANGULAR_Z, value.eLOCK_ANGULAR_Z);
	Write(strLOCK_LINEAR_X, value.eLOCK_LINEAR_X);
	Write(strLOCK_LINEAR_Y, value.eLOCK_LINEAR_Y);
	Write(strLOCK_LINEAR_Z, value.eLOCK_LINEAR_Z);
}

template<>
bool imgui::Inspector::WriteCustom(eng::RigidDynamic& value)
{
	bool result = false;
	result |= Write("eENABLE_CCD", value.eENABLE_CCD);
	result |= Write("eKINEMATIC", value.eKINEMATIC);
	result |= Write("eLOCK_ANGULAR_X", value.eLOCK_ANGULAR_X);
	result |= Write("eLOCK_ANGULAR_Y", value.eLOCK_ANGULAR_Y);
	result |= Write("eLOCK_ANGULAR_Z", value.eLOCK_ANGULAR_Z);
	result |= Write("eLOCK_LINEAR_X", value.eLOCK_LINEAR_X);
	result |= Write("eLOCK_LINEAR_Y", value.eLOCK_LINEAR_Y);
	result |= Write("eLOCK_LINEAR_Z", value.eLOCK_LINEAR_Z);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::RigidStatic& value) const { }

template<>
void eng::Visitor::WriteCustom(const eng::RigidStatic& value) { }

template<>
bool imgui::Inspector::WriteCustom(eng::RigidStatic& value)
{
	return false;
}

template<>
void eng::Visitor::ReadCustom(eng::ShapeBox& value) const
{
	Read(strExtents, value.m_Extents, value.m_Extents);
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strRotate, value.m_Rotate, value.m_Rotate);
	Read(strChannel, value.m_Channel, value.m_Channel);
}

template<>
void eng::Visitor::WriteCustom(const eng::ShapeBox& value)
{
	Write(strExtents, value.m_Extents);
	Write(strTranslate, value.m_Translate);
	Write(strRotate, value.m_Rotate);
	Write(strChannel, value.m_Channel);
}

template<>
bool imgui::Inspector::WriteCustom(eng::ShapeBox& value)
{
	bool result = false;
	result |= Write("m_Extents", value.m_Extents);
	result |= Write("m_Translate", value.m_Translate);
	result |= Write("m_Rotate", value.m_Rotate);
	result |= Write("m_Channel", value.m_Channel);
	return result;
}

template<>
void eng::Visitor::ReadCustom(eng::ShapeSphere& value) const
{
	Read(strTranslate, value.m_Translate, value.m_Translate);
	Read(strChannel, value.m_Channel, value.m_Channel);
	Read(strRadius, value.m_Radius, value.m_Radius);
}

template<>
void eng::Visitor::WriteCustom(const eng::ShapeSphere& value)
{
	Write(strTranslate, value.m_Translate);
	Write(strChannel, value.m_Channel);
	Write(strRadius, value.m_Radius);
}

template<>
bool imgui::Inspector::WriteCustom(eng::ShapeSphere& value)
{
	bool result = false;
	result |= Write("m_Translate", value.m_Translate);
	result |= Write("m_Channel", value.m_Channel);
	result |= Write("m_Radius", value.m_Radius);
	return result;
}