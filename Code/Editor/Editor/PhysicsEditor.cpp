#include "EditorPCH.h"
#include "Editor/PhysicsEditor.h"

#include "ECS/EntityWorld.h"
#include "ECS/NameComponent.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/FileHelpers.h"
#include "Engine/InputComponent.h"
#include "Engine/PhysicsPrototype.h"
#include "Engine/PrototypeManager.h"
#include "Engine/RigidStaticComponent.h"
#include "Engine/Visitor.h"
#include "Math/Quaternion.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_user.h>
#include <imgui/Inspector.h>

#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>
#include <PhysX/PxScene.h>

namespace
{
	using World = editor::PhysicsEditor::World;

	constexpr ImGuiDockNodeFlags s_DockNodeFlags =
		ImGuiDockNodeFlags_NoCloseButton |
		ImGuiDockNodeFlags_NoWindowMenuButton;
	constexpr ImGuiTableFlags s_InspectorFlags = 0;
	constexpr ImGuiWindowFlags s_WindowFlags =
		ImGuiWindowFlags_MenuBar;

	Vector3f ToVector3f(const physx::PxVec3& value)
	{
		return Vector3f(value.x, value.y, value.z);
	}

	Rotator ToRotator(const physx::PxQuat& value)
	{
		return Rotator::FromQuaternion(Quaternion(value.x, value.y, value.z, value.w));
	}

	str::String ToLabel(const char* label, const ecs::Entity& entity)
	{
		return std::format("{}: {}", label, entity.GetIndex());
	}

	const char* ToName(World& world, const ecs::Entity& entity)
	{
		if (world.HasComponent<ecs::NameComponent>(entity))
		{
			const auto& name = world.ReadComponent<ecs::NameComponent>(entity);
			return name.m_Name.c_str();
		}
		return "<unknown>";
	}

	bool HasInput_Save(World& world)
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::InputComponent>>())
		{
			const auto& input = world.ReadComponent<eng::InputComponent>(entity);
			return input.IsKeyHeld(input::EKeyboard::Control_L) && input.IsKeyPressed(input::EKeyboard::S);
		}
		return false;
	}

	void DrawMenuBar(World& world, const ecs::Entity& entity)
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl + S"))
					world.AddComponent<editor::PhysicsAssetSaveComponent>(entity);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	};

	void DrawEntities(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::PhysicsWindowComponent>(entity);

		using Query = ecs::query::Include<const eng::RigidStaticComponent>;
		for (auto&& view : world.Query<Query>())
		{
			const char* name = ToName(world, view);
			const bool isSelected = view == windowComponent.m_Selected;
			if (ImGui::Selectable(name, isSelected))
				windowComponent.m_Selected = view;
		}
	}

	void DrawInspector(World& world, const ecs::Entity& entity)
	{
		auto& windowComponent = world.WriteComponent<editor::PhysicsWindowComponent>(entity);

		const ecs::Entity selected = windowComponent.m_Selected;
		if (world.IsAlive(selected) && world.HasComponent<eng::RigidStaticComponent>(selected))
		{
			imgui::Inspector inspector;
			inspector.Write("eng::RigidStaticComponent", world.WriteComponent<eng::RigidStaticComponent>(selected));
		}
	}

	void DrawPopupSave(World& world, const ecs::Entity& entity)
	{
		constexpr Vector2f s_DefaultSize = Vector2f(500.f, 400.f);
		constexpr ImGuiPopupFlags s_PopupFlags = ImGuiPopupFlags_NoOpenOverExistingPopup;
		constexpr ImGuiWindowFlags s_WindowFlags = ImGuiWindowFlags_NoDocking;

		const auto& prototypeManager = world.ReadResource< eng::PrototypeManager>();

		if (world.HasComponent<editor::PhysicsAssetSaveComponent>(entity))
			world.RemoveComponent<editor::PhysicsAssetSaveComponent>(entity);

		if (world.HasComponent<editor::PhysicsAssetSaveComponent>(entity) || HasInput_Save(world))
		{
			auto& windowComponent = world.WriteComponent<editor::PhysicsWindowComponent>(entity);
			const ecs::Entity selected = windowComponent.m_Selected;

			const bool hasPrototype = world.HasComponent<eng::PrototypeComponent>(selected);
			const bool hasPhysics = world.HasComponent<eng::RigidStaticComponent>(selected);
			if (hasPrototype && hasPhysics)
			{
				const auto& prototype = world.ReadComponent<eng::PrototypeComponent>(selected);
				const auto& physics = world.ReadComponent<eng::RigidStaticComponent>(selected);

				eng::SaveFileSettings settings;
				settings.m_Filters = { "Prototypes (*.prototype)", "*.prototype" };
				settings.m_Path = prototype.m_Path;

				const str::Path filepath = eng::SaveFileDialog(settings);
				if (!filepath.IsEmpty())
				{
					eng::PhysicsPrototype physicsPrototype;
					physicsPrototype.m_Rigidbody = eng::RigidStatic();

					for (const physx::PxShape* shape : physics.m_Shapes)
					{
						const physx::PxTransform transform = shape->getLocalPose();

						switch (shape->getGeometryType())
						{
						case physx::PxGeometryType::eBOX:
						{
							physx::PxBoxGeometry geometry;
							shape->getBoxGeometry(geometry);

							eng::ShapeBox box;
							box.m_Translate = ToVector3f(transform.p);
							box.m_Rotate = ToRotator(transform.q);
							box.m_Extents = ToVector3f(geometry.halfExtents);
							box.m_Channel = shape->getSimulationFilterData().word0;
							physicsPrototype.m_Shapes.Append(box);

						} break;
						case physx::PxGeometryType::eSPHERE:
						{
							physx::PxSphereGeometry geometry;
							shape->getSphereGeometry(geometry);

							eng::ShapeSphere sphere;
							sphere.m_Translate = ToVector3f(transform.p);
							sphere.m_Radius = geometry.radius;
							sphere.m_Channel = shape->getSimulationFilterData().word0;
							physicsPrototype.m_Shapes.Append(sphere);
						} break;
						}
					}

					eng::Visitor visitor;
					visitor.LoadFromFile(filepath);
					visitor.Write("Physics", physicsPrototype);

					str::String string = visitor;
					visitor.SaveToFile(filepath);
				}
			}
		}
	};
}

template<>
bool imgui::Inspector::WriteCustom(eng::RigidStaticComponent& value)
{
	Write("m_Shapes", value.m_Shapes);
	return false;
}

template<>
bool imgui::Inspector::WriteCustom(physx::PxShape*& value)
{
	physx::PxTransform transform = value->getLocalPose();
	Vector3f translate = ToVector3f(transform.p);
	Rotator rotate = ToRotator(transform.q);

	{
		bool hasChanged = false;
		hasChanged |= Write("m_Translate", translate);
		hasChanged |= Write("m_Rotate", rotate);

		if (hasChanged)
		{
			transform.p.x = translate.x;
			transform.p.y = translate.y;
			transform.p.z = translate.z;

			Quaternion quat = Quaternion::FromRotator(rotate);
			transform.q.x = quat.x;
			transform.q.y = quat.y;
			transform.q.z = quat.z;
			transform.q.w = quat.w;
			value->setLocalPose(transform);
		}
	}

	switch (value->getGeometryType())
	{
	case physx::PxGeometryType::eBOX:
	{
		physx::PxBoxGeometry geometry;
		value->getBoxGeometry(geometry);

		Vector3f extents = ToVector3f(geometry.halfExtents);
		if (Write("m_Extents", extents))
		{
			geometry.halfExtents.x = extents.x;
			geometry.halfExtents.y = extents.y;
			geometry.halfExtents.z = extents.z;
			value->setGeometry(geometry);
		}

	} break;
	case physx::PxGeometryType::eSPHERE:
	{
		physx::PxSphereGeometry geometry;
		value->getSphereGeometry(geometry);

		if (Write("m_Radius", geometry.radius))
			value->setGeometry(geometry);
	} break;
	}

	return false;
}

void editor::PhysicsEditor::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	constexpr Vector2f s_DefaultPos = Vector2f(400.f, 200.f);
	constexpr Vector2f s_DefaultSize = Vector2f(800, 600.f);

	for (const ecs::Entity& entity : world.Query<ecs::query::Added<const editor::PhysicsWindowRequestComponent>>())
	{
		const ecs::Entity windowEntity = world.CreateEntity();
		auto& windowComponent = world.AddComponent<editor::PhysicsWindowComponent>(windowEntity);
		windowComponent.m_DockspaceLabel = ToLabel("Physics Editor", windowEntity);
		windowComponent.m_EntitiesLabel = ToLabel("Entities", windowEntity);
		windowComponent.m_InspectorLabel = ToLabel("Inspector", windowEntity);
	}

	for (const ecs::Entity& windowEntity : world.Query<ecs::query::Include<editor::PhysicsWindowComponent>>())
	{
		auto& windowComponent = world.WriteComponent<editor::PhysicsWindowComponent>(windowEntity);

		bool isOpen = true;
		ImGui::SetNextWindowPos({ s_DefaultPos.x, s_DefaultPos.y }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ s_DefaultSize.x, s_DefaultSize.y }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin(windowComponent.m_DockspaceLabel.c_str(), &isOpen, s_WindowFlags))
		{
			DrawMenuBar(world, windowEntity);

			const ImGuiID dockspaceId = ImGui::GetID(windowComponent.m_DockspaceLabel.c_str());
			if (!ImGui::DockBuilderGetNode(dockspaceId))
			{
				ImGui::DockBuilderRemoveNode(dockspaceId);
				ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

				ImGuiID entitiesId, inspectorId;
				ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.6f, &entitiesId, &inspectorId);
				ImGui::DockBuilderDockWindow(windowComponent.m_EntitiesLabel.c_str(), entitiesId);
				ImGui::DockBuilderDockWindow(windowComponent.m_InspectorLabel.c_str(), inspectorId);
				ImGui::DockBuilderFinish(dockspaceId);
			}
			ImGui::DockSpace(dockspaceId, ImVec2(0, 0), s_DockNodeFlags);
		}
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_EntitiesLabel.c_str()))
			DrawEntities(world, windowEntity);
		ImGui::End();

		if (ImGui::Begin(windowComponent.m_InspectorLabel.c_str()))
			DrawInspector(world, windowEntity);
		ImGui::End();

		DrawPopupSave(world, windowEntity);

		if (!isOpen)
			world.DestroyEntity(windowEntity);
	}
}