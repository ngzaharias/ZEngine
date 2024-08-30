#include "EditorPCH.h"
#include "Editor/SettingsLocalSystem.h"

#include "Core/Path.h"
#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Editor/SettingsComponents.h"
#include "Engine/Visitor.h"

namespace
{
	const str::StringView strFilename = "EditorSettings.toml";

	const str::StringView strImport = "m_Import";
	const str::StringView strSave = "m_Save";
	const str::StringView strTexture = "m_Texture";
}

template<>
void eng::Visitor::ReadCustom(edit::settings::LocalComponent& value) const
{
	Read(strTexture, value.m_Texture, value.m_Texture);
}

template<>
void eng::Visitor::ReadCustom(edit::settings::Texture& value) const
{
	Read(strImport, value.m_Import, value.m_Import);
	Read(strSave, value.m_Save, value.m_Save);
}

template<>
void eng::Visitor::WriteCustom(const edit::settings::LocalComponent& value)
{
	Write(strTexture, value.m_Texture);
}

template<>
void eng::Visitor::WriteCustom(const edit::settings::Texture& value)
{
	Write(strImport, value.m_Import);
	Write(strSave, value.m_Save);
}

void edit::settings::LocalSystem::Initialise(World& world)
{
	const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

	eng::Visitor visitor;
	visitor.LoadFromFile(filepath);
	visitor.Read(world.WriteSingleton<edit::settings::LocalComponent>());
}

void edit::settings::LocalSystem::Update(World& world, const GameTime& gameTime)
{
	if (world.HasAny<ecs::query::Updated<const edit::settings::LocalComponent>>())
	{
		const str::Path filepath = str::Path(str::EPath::AppData, strFilename);

		eng::Visitor visitor;
		visitor.Write(world.ReadSingleton<edit::settings::LocalComponent>());
		visitor.SaveToFile(filepath);
	}
}

