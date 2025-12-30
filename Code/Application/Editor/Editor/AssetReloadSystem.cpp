#include "EditorPCH.h"
#include "Editor/AssetReloadSystem.h"

#include "Core/VariantHelpers.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/AssetManager.h"

#include <stdlib.h>
#include <windows.h>

// https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw.html
// https://stackoverflow.com/questions/12036150/losing-files-when-using-readdirectorychangesw
// https://learn.microsoft.com/en-gb/answers/questions/1021759/using-readdirectorychangesw()-to-get-information-a

namespace
{
	const str::Name strGuid = NAME("m_Guid");

	str::String ToString(const std::wstring& input)
	{
		// +1 for null terminator
		std::string output;
		output.resize(input.length() + 1);

		size_t charsConverted = 0;
		wcstombs_s(&charsConverted, output.data(), output.length(), input.c_str(), input.length());
		return output;
	}

	str::Path ToPath(const str::Path& root, FILE_NOTIFY_INFORMATION* tmp)
	{
		std::wstring w = L"";
		for (DWORD i = 0; i < tmp->FileNameLength / 2; i++)
			w += tmp->FileName[i];

		return str::Path(root, ToString(w));
	}
}

void editor::AssetReloadSystem::Initialise(World& world)
{
	PROFILE_FUNCTION();

	std::thread thread(&editor::AssetReloadSystem::Execute, this);
	thread.detach();
}

void editor::AssetReloadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (!m_Commands.IsEmpty())
	{
		m_Mutex.lock();

		auto& assetManager = world.WriteResource<eng::AssetManager>();
		for (const editor::Command& command : m_Commands)
		{
			// we need to support rename since visual studio does a 'safe save'
			// 1. saves changes in a temporary file
			// 2. rename original file to something else
			// 3. rename temporary file to original filename
			// 4. delete original file
			str::Path filepath;
			core::VariantMatch(command,
				[&](const editor::Modify& data) { filepath = data.m_Path; },
				[&](const editor::Rename& data) { filepath = data.m_New; },
				[&](const auto&) {});

			if (!filepath.IsEmpty() && !filepath.IsDirectory())
			{
				str::Guid guid;
				eng::Visitor visitor;
				visitor.LoadFromFile(filepath);
				visitor.Read(strGuid, guid, {});
				if (guid.IsValid())
					assetManager.ReloadAsset(guid);
			}
		}
		m_Commands.RemoveAll();

		m_Mutex.unlock();
	}
}

void editor::AssetReloadSystem::Execute()
{
	constexpr DWORD dwDesiredAccess =
		FILE_LIST_DIRECTORY;
	constexpr DWORD dwShareMode =
		FILE_SHARE_READ |
		FILE_SHARE_WRITE |
		FILE_SHARE_DELETE;
	constexpr DWORD dwCreationDisposition =
		OPEN_EXISTING;
	constexpr DWORD dwFlagsAndAttributes =
		FILE_FLAG_BACKUP_SEMANTICS |
		FILE_FLAG_OVERLAPPED;

	const str::Path assetPath = str::Path(str::EPath::Assets);

	const HANDLE hDirectory = CreateFileA(assetPath.ToChar(), dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (hDirectory == INVALID_HANDLE_VALUE)
		return;

	constexpr bool bWatchSubtree = true;
	constexpr DWORD dwNotifyFilter =
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_LAST_WRITE;

	constexpr DWORD dwBufferLen = 1024;
	FILE_NOTIFY_INFORMATION* buffer = new FILE_NOTIFY_INFORMATION[dwBufferLen];
	while (true)
	{
		DWORD lpBytesReturned;
		if (ReadDirectoryChangesW(hDirectory, buffer, dwBufferLen, bWatchSubtree, dwNotifyFilter, &lpBytesReturned, NULL, NULL))
		{
			m_Mutex.lock();

			DWORD OffsetToNext = 0;
			FILE_NOTIFY_INFORMATION* pbuffer = buffer;
			do
			{
				pbuffer = (FILE_NOTIFY_INFORMATION*)(((char*)pbuffer) + OffsetToNext);
				switch (pbuffer->Action)
				{
				//case FILE_ACTION_ADDED:
				//	m_Commands.Emplace(editor::Create{ .m_Path = ToPath(assetPath, pbuffer) });
				//	break;
				//case FILE_ACTION_REMOVED:
				//	m_Commands.Emplace(editor::Delete{ .m_Path = ToPath(assetPath, pbuffer) });
				//	break;
				case FILE_ACTION_MODIFIED:
					m_Commands.Emplace(editor::Modify{ .m_Path = ToPath(assetPath, pbuffer) });
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					m_Commands.Emplace(editor::Rename{ .m_Old = ToPath(assetPath, pbuffer) });
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					auto& command = std::get<editor::Rename>(m_Commands.GetLast());
					command.m_New = ToPath(assetPath, pbuffer);
					break;
				}
				OffsetToNext = pbuffer->NextEntryOffset;
			} while (OffsetToNext > 0);

			m_Mutex.unlock();
		}
	}
	delete[] buffer;
}