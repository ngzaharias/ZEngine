#pragma once

#include <Core/Array.h>
#include <Core/Guid.h>
#include <Core/ObjectPool.h>

#include <ECS/System.h>

#include <SFML/Audio/Sound.hpp>

namespace sf
{
	class Time;
}

namespace eng
{
	class AssetManager;
	struct NameComponent;
	struct SoundComponent;

	struct SoundRequest
	{
		str::Guid m_Guid;
	};

	class SoundSystem final : public ecs::System
	{
	public:
		using World = ecs::WorldView<
			eng::AssetManager,
			eng::NameComponent,
			eng::SoundComponent>;

		void Update(World& world, const GameTime& gameTime);

		void PlaySound(const str::Guid& guid);

		float GetVolume() const { return m_Volume; }
		void SetVolume(float value) { m_Volume = value; }

	private:
		Array<eng::SoundRequest> m_Requests;
		ObjectPool<sf::Sound, 128> m_SoundPool;

		float m_Volume = 0.f;
	};
};