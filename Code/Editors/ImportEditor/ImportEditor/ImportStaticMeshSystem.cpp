#include "ImportPCH.h"
#include "ImportEditor/ImportStaticMeshSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"
#include "ImportEditor/ImportAdvanceEvent.h"
#include "ImportEditor/ImportQueueComponent.h"
#include "ImportEditor/ImportStaticMeshComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_user.h"
#include "imgui/Inspector.h"

namespace
{
	const str::Name strStaticMesh = NAME("StaticMesh");
}

void editor::importer::StaticMeshSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar;

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	const bool isQueueAdded = world.HasAny<ecs::query::Added<const editor::importer::QueueComponent>>();
	const bool isQueueUpdated = world.HasAny<ecs::query::Updated<const editor::importer::QueueComponent>>();
	const bool isQueueRemoved = world.HasAny<ecs::query::Removed<const editor::importer::QueueComponent>>();
	if (isQueueAdded || isQueueUpdated)
	{
		const auto& queueComponent = world.ReadComponent<editor::importer::QueueComponent>();
		const str::Path sourcePath = !queueComponent.m_Files.IsEmpty() 
			? queueComponent.m_Files.GetFirst() 
			: str::Path{};
		const str::StringView extension = sourcePath.GetFileExtension();
		if (extension == ".glb" || extension == ".gltf")
		{
			const str::StringView filename = sourcePath.GetFileNameNoExtension();
			str::Path filepath = sourcePath.GetDirectory();
			filepath += filename;
			filepath += ".asset";

			auto& importComponent = !world.HasComponent<editor::importer::StaticMeshComponent>()
				? world.AddComponent<editor::importer::StaticMeshComponent>()
				: world.WriteComponent<editor::importer::StaticMeshComponent>();
			importComponent.m_Filepath = filepath;
			importComponent.m_Asset = {};
			importComponent.m_Asset.m_Guid = str::Guid::Generate();
			importComponent.m_Asset.m_Name = NAME(filename);
			importComponent.m_Asset.m_Type = strStaticMesh;
			importComponent.m_Asset.m_SourceFile = sourcePath;
		}
		else if (world.HasComponent<editor::importer::StaticMeshComponent>())
		{
			world.RemoveComponent<editor::importer::StaticMeshComponent>();
		}
	}
	else if (isQueueRemoved)
	{
		if (world.HasComponent<editor::importer::StaticMeshComponent>())
			world.RemoveComponent<editor::importer::StaticMeshComponent>();
	}

	for (auto&& view : world.Query<ecs::query::Include<editor::importer::StaticMeshComponent>>())
	{
		auto& importComponent = view.WriteRequired<editor::importer::StaticMeshComponent>();
		eng::StaticMeshAsset& asset = importComponent.m_Asset;
		str::Path& filepath = importComponent.m_Filepath;

		imgui::SetNextWindowPos(s_DefaultPos, ImGuiCond_FirstUseEver);
		imgui::SetNextWindowSize(s_DefaultSize, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Import: Static Mesh", nullptr, s_WindowFlags))
		{
			imgui::Inspector inspector;
			if (inspector.Begin("##staticmesh"))
			{
				inspector.Write("Guid", asset.m_Guid);
				inspector.Write("Name", asset.m_Name);
				inspector.Write("Source", asset.m_SourceFile);
				inspector.End();
			}

			if (ImGui::Button("Import"))
			{
				auto& assetManager = world.WriteResource<eng::AssetManager>();
				assetManager.SaveToFile(asset, filepath);

				world.AddEvent<editor::importer::AdvanceEvent>();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				world.AddEvent<editor::importer::AdvanceEvent>();
			}
		}
		ImGui::End();
	}
}
