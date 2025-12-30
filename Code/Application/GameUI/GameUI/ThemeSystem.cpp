#include "GameUIPCH.h"
#include "GameUI/ThemeSystem.h"

#include "Core/Colour.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SettingsGameplaySingleton.h"
#include "Engine/TablesReloaded.h"
#include "Engine/ThemeTable.h"
#include "Engine/UIManager.h"

#include <NsDrawing/Color.h>
#include <NsGui/SolidColorBrush.h>

namespace
{
	Noesis::Color ToColour(const Colour& value)
	{
		return Noesis::Color(value.r, value.g, value.b, value.a);
	}

	void UpdateTheme(gui::ThemeSystem::World& world)
	{
		const auto& settings = world.ReadSingleton<eng::settings::GameplaySingleton>();
		const auto& themes = world.WriteResource<eng::ThemeTable>();
		auto& uiManager = world.WriteResource<eng::UIManager>();

		const eng::Theme& theme = themes.GetTheme(settings.m_Theme);

		Noesis::Ptr<Noesis::SolidColorBrush> background0 = Noesis::MakePtr<Noesis::SolidColorBrush>();
		Noesis::Ptr<Noesis::SolidColorBrush> background1 = Noesis::MakePtr<Noesis::SolidColorBrush>();
		Noesis::Ptr<Noesis::SolidColorBrush> background2 = Noesis::MakePtr<Noesis::SolidColorBrush>();
		Noesis::Ptr<Noesis::SolidColorBrush> highlight = Noesis::MakePtr<Noesis::SolidColorBrush>();
		Noesis::Ptr<Noesis::SolidColorBrush> outline = Noesis::MakePtr<Noesis::SolidColorBrush>();

		background0->SetColor(ToColour(theme.m_Background0));
		background1->SetColor(ToColour(theme.m_Background1));
		background2->SetColor(ToColour(theme.m_Background2));
		highlight->SetColor(ToColour(theme.m_Highlight));
		outline->SetColor(ToColour(theme.m_Outline));

		uiManager.SetResource("Brush.Background0", background0);
		uiManager.SetResource("Brush.Background1", background1);
		uiManager.SetResource("Brush.Background2", background2);
		uiManager.SetResource("Brush.Highlight", highlight);
		uiManager.SetResource("Brush.Outline", outline);
	}
}

void gui::ThemeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<eng::settings::GameplaySingleton>())
		UpdateTheme(world);
	if (world.HasAny<ecs::query::Added<const eng::TablesReloaded>>())
		UpdateTheme(world);
}