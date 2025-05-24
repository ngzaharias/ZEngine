#include "EditorPCH.h"
#include "Editor/GameModeSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "Engine/SettingsComponents.h"
#include "Engine/Texture2DAsset.h"
#include "Engine/Window.h"
#include "Engine/WindowManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"

namespace
{
	const str::Guid strIconPlay = GUID("fad14b87898b4c13aae829c3b8756aec");
	const str::Guid strIconStop = GUID("7b71eda0a582471e9531ec36671ae413");
}

void editor::GameModeSystem::Initialise(World& world)
{
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.RequestAsset<eng::Texture2DAsset>(strIconPlay);
	assetManager.RequestAsset<eng::Texture2DAsset>(strIconStop);
}

void editor::GameModeSystem::Shutdown(World& world)
{
	auto& assetManager = world.WriteResource<eng::AssetManager>();
	assetManager.ReleaseAsset<eng::Texture2DAsset>(strIconPlay);
	assetManager.ReleaseAsset<eng::Texture2DAsset>(strIconStop);
}

void editor::GameModeSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_Flags =
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoFocusOnAppearing |
		//ImGuiWindowFlags_NoMouseInputs |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoNavInputs |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoTitleBar;

	ImGuiWindowClass windowClass;
	windowClass.ViewportFlagsOverrideClear |= ImGuiViewportFlags_NoAutoMerge;
	ImGui::SetNextWindowClass(&windowClass);

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	const Vector2f viewportPos = viewport->Pos;
	const Vector2f viewportSize = viewport->Size;
	ImGui::SetNextWindowPos(viewportPos + Vector2f(viewportSize.x * 0.5f - 32.f, 32.f));
	ImGui::SetNextWindowSize(Vector2f(100.f, 100.f));
	if (ImGui::Begin("GameMode##editor", nullptr, s_Flags))
	{
		const auto& assetManager = world.ReadResource<eng::AssetManager>();
		const auto* iconPlay = assetManager.FetchAsset<eng::Texture2DAsset>(strIconPlay);
		const auto* iconStop = assetManager.FetchAsset<eng::Texture2DAsset>(strIconStop);
		if (iconPlay && iconStop)
		{
			auto& settings = world.WriteSingleton<eng::settings::DebugComponent>();
			if (settings.m_IsEditorModeEnabled)
			{
				if (ImGui::ImageButton("##play", iconPlay->m_TextureId, Vector2f(32.f)))
					settings.m_IsEditorModeEnabled = false;
			}

			if (!settings.m_IsEditorModeEnabled)
			{
				if (ImGui::ImageButton("##stop", iconStop->m_TextureId, Vector2f(32.f)))
					settings.m_IsEditorModeEnabled = true;
			}
		}
	}
	ImGui::End();
}
