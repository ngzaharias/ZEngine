#pragma once

#include "Core/Set.h"
#include "Core/String.h"
#include "Engine/IWindow.h"
#include "Math/Vector.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace sfml
{
	class Window final : public eng::IWindow
	{
	public:
		explicit Window(const eng::WindowConfig& config);
		~Window() override;

		void Initialize() override;
		void Shutdown() override;

		void PreUpdate(const GameTime& gameTime) override;
		void PostUpdate(const GameTime& gameTime) override;

		Vector2u GetSize() const override;
		sf::RenderWindow& GetWindow() { return m_Window; }

		bool HasResized() const override { return m_HasResized; }
		void SetResized(bool value) { m_HasResized = value; }

		bool ShouldClose() const override;

	private:
		sf::RenderWindow m_Window;
		eng::WindowConfig m_Config = {};
		bool m_HasResized = false;
	};
}
