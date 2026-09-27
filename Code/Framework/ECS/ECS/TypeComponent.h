#pragma once

#include "Core/Name.h"
#include "Core/MemBuffer.h"
#include "Core/String.h"
#include "Core/TypeInfo.h"
#include "ECS/QueryId.h"

class MemBuffer;

namespace ecs
{
	class EntityBuffer;
	class EntityStorage;
	struct Entity;
}

namespace ecs
{
	struct TypeComponent
	{
		str::Name m_Name = {};
		uint16 m_Bytes = 0;

		TypeId m_GlobalId = -1;
		TypeId m_LocalId = -1;

		ecs::QueryId m_AddedId = -1;
		ecs::QueryId m_UpdatedId = -1;
		ecs::QueryId m_RemovedId = -1;
		ecs::QueryId m_IncludeId = -1;

		bool m_IsReplicated = false;
		bool m_IsTemplate = false;

		using Constructor = void(void*);
		Constructor* m_Constructor = nullptr;
		using Copystructor = void(void*, void*);
		Copystructor* m_Copystructor = nullptr;
		using Destructor = void(void*);
		Destructor* m_Destructor = nullptr;

		using HasComponent = bool(ecs::EntityStorage&, const ecs::Entity&);
		HasComponent* m_HasComponent = nullptr;

		using AddComponentData = void(ecs::EntityBuffer&, const ecs::Entity&, const MemBuffer&);
		AddComponentData* m_AddComponentData = nullptr;
		using AddComponentSolo = void(ecs::EntityBuffer&, const ecs::Entity&);
		AddComponentSolo* m_AddComponentSolo = nullptr;

		using UpdateComponentData = void(ecs::EntityBuffer&, const ecs::Entity&, const MemBuffer&);
		UpdateComponentData* m_UpdateComponentData = nullptr;
		using UpdateComponentSolo = void(ecs::EntityBuffer&, const ecs::Entity&);
		UpdateComponentSolo* m_UpdateComponentSolo = nullptr;

		using ReadComponentData = void(ecs::EntityBuffer&, const ecs::Entity&, MemBuffer&);
		ReadComponentData* m_ReadComponentData = nullptr;
		using RemoveComponentSolo = void(ecs::EntityBuffer&, const ecs::Entity&);
		RemoveComponentSolo* m_RemoveComponentSolo = nullptr;

		using WriteComponentData = void(ecs::EntityBuffer&, const ecs::Entity&, const MemBuffer&);
		WriteComponentData* m_WriteComponentData = nullptr;
	};

	template<typename TComponent>
	static void CreateComponent(void* data);
	template<typename TComponent>
	static void CopyComponent(void* source, void* target);
	template<typename TComponent>
	static void DestroyComponent(void* data);

	template<typename TComponent>
	static bool HasComponent(ecs::EntityStorage& storage, const ecs::Entity& entity);

	template<typename TComponent>
	static void AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity);
	template<typename TComponent>
	static void AddComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, const MemBuffer& data);

	template<typename TComponent>
	static void RemoveComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity);

	template<typename TComponent>
	static void UpdateComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity);
	template<typename TComponent>
	static void UpdateComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, const MemBuffer& data);

	template<typename TComponent>
	static void ReadComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, MemBuffer& data);

	template<typename TComponent>
	static void WriteComponent(ecs::EntityBuffer& buffer, const ecs::Entity& entity, const MemBuffer& data);
}

#include "ECS/TypeComponent.inl"