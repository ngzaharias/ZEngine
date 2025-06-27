#pragma once

#include <Core/Defines.h>

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace core
{
	class Application;
}

namespace glfw
{
	class Window;
}

namespace vulkan
{
	class Model;
	class Pipeline;
	class Renderer;
	class SwapChain;
	class Texture;

	struct QueueFamily
	{
		uint32 m_GraphicsFamily = UINT_MAX;
		uint32 m_PresentFamily = UINT_MAX;
	};

	class Device final
	{
		friend class core::Application;
		friend class vulkan::Model;
		friend class vulkan::Pipeline;
		friend class vulkan::Renderer;
		friend class vulkan::SwapChain;
		friend class vulkan::Texture;
	
	public:
		Device(glfw::Window& window);
		~Device();

	private:
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;

	private:
		glfw::Window& m_Window;

		VkDevice m_LogicalDevice = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

		VkInstance m_Instance = VK_NULL_HANDLE;
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
		VkQueue m_PresentQueue = VK_NULL_HANDLE;

		QueueFamily m_QueueFamily = { };

#ifdef Z_VULKAN_VALIDATION_LAYERS
		VkDebugUtilsMessengerEXT m_DebugMessenger = VK_NULL_HANDLE;
#endif
	};
}

#endif