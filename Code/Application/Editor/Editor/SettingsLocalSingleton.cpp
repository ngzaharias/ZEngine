#include "EditorPCH.h"
#include "Editor/SettingsLocalSingleton.h"

#include "Engine/Visitor.h"
#include "imgui/Inspector.h"

namespace
{
	const str::StringView strEntity = "m_Entity";
	const str::StringView strExtract = "m_Extract";
	const str::StringView strGizmos = "m_Gizmos";
	const str::StringView strImport = "m_Import";
	const str::StringView strOpen = "m_Open";
	const str::StringView strSave = "m_Save";
	const str::StringView strSprite = "m_Sprite";
	const str::StringView strTexture = "m_Texture";
}

template<>
void eng::Visitor::ReadCustom(editor::settings::LocalSingleton& value) const
{
	Read(strEntity, value.m_Entity, value.m_Entity);
	Read(strGizmos, value.m_Gizmos, value.m_Gizmos);
	Read(strSprite, value.m_Sprite, value.m_Sprite);
	Read(strTexture, value.m_Texture, value.m_Texture);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::LocalSingleton& value)
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
void eng::Visitor::ReadCustom(editor::settings::Sprite& value) const
{
	Read(strExtract, value.m_Extract, value.m_Extract);
	Read(strOpen, value.m_Open, value.m_Open);
	Read(strSave, value.m_Save, value.m_Save);
}
template<>
void eng::Visitor::WriteCustom(const editor::settings::Sprite& value)
{
	Write(strExtract, value.m_Extract);
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
