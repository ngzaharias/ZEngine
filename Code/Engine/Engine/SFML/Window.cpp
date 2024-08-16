#include "EnginePCH.h"
#include "Engine/SFML/Window.h"

#include "Core/GameTime.h"

#include <imgui/imgui.h>
#include <imgui/imgui_sfml.h>
#include <SFML/Window/Event.hpp>

sfml::Window::Window(const eng::WindowConfig& config)
	: eng::IWindow(config)
	, m_Config(config)
{
}

sfml::Window::~Window()
{
}

void sfml::Window::Initialize()
{
	m_Window.create(sf::VideoMode(m_Config.m_Size.x, m_Config.m_Size.y), m_Config.m_Name.c_str());

	const bool result = ImGui::SFML::Init(m_Window);
}

void sfml::Window::Shutdown()
{
	ImGui::SFML::Shutdown();
}

void sfml::Window::PreUpdate(const GameTime& gameTime)
{
	sf::Event eventData;
	while (m_Window.pollEvent(eventData))
	{
		if (eventData.type == sf::Event::Closed)
			m_Window.close();
	}

	sf::Time time = sf::seconds(gameTime.m_DeltaTime);
	ImGui::SFML::Update(m_Window, time);
}

void sfml::Window::PostUpdate(const GameTime& gameTime)
{
	m_Window.clear();
	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

Vector2u sfml::Window::GetSize() const
{
	const sf::Vector2u size = m_Window.getSize();
	return Vector2u(size.x, size.y);
}

bool sfml::Window::ShouldClose() const
{
	return !m_Window.isOpen();
}