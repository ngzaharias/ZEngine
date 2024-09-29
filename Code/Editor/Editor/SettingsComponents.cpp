#include "EditorPCH.h"
#include "Editor/SettingsComponents.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strEntity = "m_Entity";
	const str::StringView strGizmos = "m_Gizmos";
	const str::StringView strImport = "m_Import";
	const str::StringView strIsEnabled = "m_IsEnabled";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
	const str::StringView strShowCoordinates = "m_ShowCoordinates";
	const str::StringView strShowTransforms = "m_ShowTransforms";
	const str::StringView strSprite = "m_Sprite";
	const str::StringView strTexture = "m_Texture";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::LocalComponent& value) const
{
	Read(strEntity, value.m_Entity, value.m_Entity);
	Read(strGizmos, value.m_Gizmos, value.m_Gizmos);
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strTexture, value.m_Texture, value.m_Texture);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::LocalComponent& value)
{
	Write(strEntity, value.m_Entity);
	Write(strGizmos, value.m_Gizmos);
	Write(strSprite, value.m_Sprite);
	Write(strTexture, value.m_Texture);
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Entity& value) const
{
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Entity& value)
{
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Gizmos& value) const
{
	Read(strIsEnabled, value.m_IsEnabled, value.m_IsEnabled);
	Read(strShowCoordinates, value.m_ShowCoordinates, value.m_ShowCoordinates);
	Read(strShowTransforms, value.m_ShowTransforms, value.m_ShowTransforms);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Gizmos& value)
{
	Write(strIsEnabled, value.m_IsEnabled);
	Write(strShowCoordinates, value.m_ShowCoordinates);
	Write(strShowTransforms, value.m_ShowTransforms);
}
template<>
bool imgui::Inspector::WriteCustom(editor::settings::Gizmos& value)
{
	bool result = false;
	result |= Write("IsEnabled", value.m_IsEnabled);
	result |= Write("ShowCoordinates", value.m_ShowCoordinates);
	result |= Write("ShowTransforms", value.m_ShowTransforms);
	return result;
}

template<>
void eng::Visitor::ReadCustom(editor::settings::Sprite& value) const
{
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Sprite& value)
{
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}


template<>
void eng::Visitor::ReadCustom(editor::settings::Texture& value) const
{
	Read(strImport, value.m_Import, value.m_Import);
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Texture& value)
{
	Write(strImport, value.m_Import);
	Write(strOpen, value.m_Open);
	Write(strSave, value.m_Save);
}
