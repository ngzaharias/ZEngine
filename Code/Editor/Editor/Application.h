#pragma once

#include <Engine/Application.h>

namespace editor
{
	class Application final : public core::Application
	{
	public:
		Application();
		~Application();

	private:
		void Register() override;
		void Initialise() override;
		void Update(const GameTime& gameTime) override;
		void Destroy() override;

		void PlaySound();
	};
}
