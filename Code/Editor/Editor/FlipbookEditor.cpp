#include "EditorPCH.h"
#include "Editor/FlipbookEditor.h"

#include "Editor/InspectorWidgets.h"

#include <Engine/AssetManager.h>
#include <Engine/FlipbookAsset.h>
#include <Engine/FlipbookComponent.h>
#include <Engine/SpriteAsset.h>
#include <Engine/Texture2DAsset.h>

#include <entt/entity/registry.hpp>
#include <imgui/imgui.h>

namespace
{
	constexpr float s_TimeMaxEpsilon = 0.000001f;

	template<typename Type>
	void Inspect(Type& value)
	{
		imgui::InspectorBegin();
		editor::InspectType(value);
		imgui::InspectorEnd();
	}
}

editor::FlipbookEditor::FlipbookEditor(asset::Manager& assetManager)
	: m_AssetManager(assetManager)
{
}

editor::FlipbookEditor::~FlipbookEditor()
{
}

void editor::FlipbookEditor::Initialise()
{
}

void editor::FlipbookEditor::Destroy()
{
}

void editor::FlipbookEditor::Update(const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// #todo: allow open/close of editor independent of having an entity selected

	if (m_Selected && !m_Entity)
	{
		m_Entity = m_World->CreateEntity();
		m_World->AddComponent<render::FlipbookComponent>(*m_Entity);
	}
	else if (!m_Selected && m_Entity)
	{
		m_World->DestroyEntity(*m_Entity);
		m_Entity = { };
	}

	if (m_Entity)
	{
		const auto& flipbookAsset = *m_AssetManager.LoadAsset<asset::FlipbookAsset>(*m_Selected);

		auto& component = m_World->GetComponent<render::FlipbookComponent>(*m_Entity);
		component.m_Flipbook = *m_Selected;

		// #todo: only trigger once per press/release
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		//	component.m_IsPlaying = !component.m_IsPlaying;

		Render(component);
	}
}

void editor::FlipbookEditor::OpenEditor(const str::Guid& guid)
{
	m_Selected = guid;
}

void editor::FlipbookEditor::CloseEditor()
{
	m_Selected = { };
	ImGui::CloseCurrentPopup();
}

void editor::FlipbookEditor::Render(render::FlipbookComponent& component)
{
	constexpr float s_PlaybackHeight = 100.f;
	constexpr float s_SettingsWidth = 400.f;
	const Vector2f s_FramePadding = Vector2f(8.f, 8.f);

	asset::FlipbookAsset flipbookAsset;

	const ImVec2 position = { ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f };
	ImGui::SetNextWindowPos(position, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	bool isOpen = true;
	ImGui::Begin("Flipbook Editor", &isOpen, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					m_AssetManager.SaveAsset<asset::FlipbookAsset>(*m_Selected);

				// #todo
				ImGui::MenuItem("Save As...");
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		const float s_PreviewWidth = ImGui::GetColumnWidth() - s_SettingsWidth - s_FramePadding.x;
		const float s_PreviewHeight = ImGui::GetContentRegionAvail().y - s_PlaybackHeight;
		if (ImGui::BeginChild("preview", { s_PreviewWidth, s_PreviewHeight }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImGui::Text(flipbookAsset.m_Path.string().c_str());
			ImGui::Separator();

			Render_Preview(component);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("settings", { s_SettingsWidth, s_PreviewHeight }, true))
		{
			if (ImGui::CollapsingHeader("Flipbook", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const bool wasLooping = flipbookAsset.m_IsLooping;
				Inspect(flipbookAsset);

				if (wasLooping != flipbookAsset.m_IsLooping)
					component.m_IsLooping = flipbookAsset.m_IsLooping;
			}
		}
		ImGui::EndChild();

		if (ImGui::BeginChild("playback", { 0, s_PlaybackHeight - 8.f }, true))
		{
			Render_Playback(component);
		}
		ImGui::EndChild();
	}
	ImGui::End();

	if (!isOpen)
		CloseEditor();
}

void editor::FlipbookEditor::Render_Playback(render::FlipbookComponent& component)
{
	if (!m_Selected)
		return;

	const asset::FlipbookAsset& flipbookAsset = *m_AssetManager.LoadAsset<asset::FlipbookAsset>(*m_Selected);

	const int32 indexCount = flipbookAsset.m_Frames.GetCount();
	const int32 indexMax = std::max(0, indexCount - 1);
	const float timeFrame = 1.f / flipbookAsset.m_FPS;
	const float timeMax = indexCount / flipbookAsset.m_FPS;

	Nullable<float> timeNew = { };
	Nullable<int32> indexNew = { };

	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 16.f);
	if (ImGui::SliderInt("##index", &component.m_Index, 0, indexMax))
		indexNew = component.m_Index;
	if (ImGui::SliderFloat("##time", &component.m_Time, 0.f, timeMax))
		timeNew = component.m_Time;
	ImGui::PopItemWidth();

	if (ImGui::Button("Frame First"))
		indexNew = 0;

	ImGui::SameLine();

	if (ImGui::Button("Frame Previous"))
	{
		component.m_IsPlaying = false;
		indexNew = std::max(0, component.m_Index - 1);
	}

	ImGui::SameLine();

	{
		if (component.m_IsPlaying && ImGui::Button("Pause"))
			component.m_IsPlaying = false;
		if (!component.m_IsPlaying && ImGui::Button("Play"))
			component.m_IsPlaying = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Next"))
	{
		component.m_IsPlaying = false;
		indexNew = std::min(indexMax, component.m_Index + 1);
	}

	ImGui::SameLine();

	if (ImGui::Button("Frame Last"))
		indexNew = indexMax;

	ImGui::SameLine();

	{
		if (component.m_IsLooping && ImGui::Button("Unloop"))
			component.m_IsLooping = false;
		if (!component.m_IsLooping && ImGui::Button("Loop"))
			component.m_IsLooping = true;
	}

	if (timeNew)
		component.m_Time = std::clamp(timeNew.value(), 0.f, timeMax);
	if (indexNew)
		component.m_Time = indexNew.value() * timeFrame;
}

void editor::FlipbookEditor::Render_Preview(render::FlipbookComponent& component)
{
	const asset::FlipbookAsset& flipbookAsset = *m_AssetManager.LoadAsset<asset::FlipbookAsset>(component.m_Flipbook);
	if (flipbookAsset.m_Frames.IsEmpty())
		return;

	const asset::FlipbookFrame& flipbookFrame = flipbookAsset.m_Frames[component.m_Index];
	if (!flipbookFrame.m_Sprite.IsValid())
		return;

	const asset::SpriteAsset& spriteAsset = *m_AssetManager.LoadAsset<asset::SpriteAsset>(flipbookFrame.m_Sprite);
	if (!spriteAsset.m_Texture2D.IsValid())
		return;

	//const asset::Texture2DAsset& Texture2DAsset = *m_AssetManager.LoadAsset<asset::Texture2DAsset>(spriteAsset.m_Texture);
	//const sf::Texture& texture = Texture2DAsset.m_Texture;

	//sf::Sprite sprite;
	//sprite.setTexture(texture);
	//sprite.setTextureRect(sf::IntRect(
	//	spriteAsset.m_RectanglePos.x, 
	//	spriteAsset.m_RectanglePos.y, 
	//	spriteAsset.m_RectangleSize.x, 
	//	spriteAsset.m_RectangleSize.y));

	//const sf::Vector2f size = sf::Vector2f(texture.getSize());
	//ImGui::Image(sprite, { size.x, size.y });
}
