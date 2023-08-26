#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace glfw
{
	class Window;
}

namespace render
{
	class IWindow;
}

namespace vulkan
{
	class Device;
	class Renderer;

	class SwapChain final
	{
		friend class Renderer;

	public:
		static constexpr int s_MaxFramesInFlight = 2;

		SwapChain(vulkan::Device& device, glfw::Window& window, vulkan::SwapChain* swapChain);
		~SwapChain();

		VkResult AcquireNextImage(const uint32 frame, uint32& out_ImageIndex);
		bool HasMatchingFormats(const SwapChain& swapChain) const;

		static uint32 FrameIndex(const uint32 frame) { return frame % s_MaxFramesInFlight; }

	private:
		vulkan::Device& m_Device;
		glfw::Window& m_Window;

		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

		VkFormat m_SwapChainDepthFormat = VK_FORMAT_UNDEFINED;
		VkFormat m_SwapChainImageFormat = VK_FORMAT_UNDEFINED;
		VkExtent2D m_SwapChainExtent = { };

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		Array<VkFramebuffer> m_SwapChainFramebuffers = { };

		Array<VkImage> m_DepthImages;
		Array<VkImageView> m_DepthImageViews;
		Array<VkDeviceMemory> m_DepthImagesMemory;

		Array<VkImage> m_SwapChainImages = { };
		Array<VkImageView> m_SwapChainImageViews = { };

		Array<VkSemaphore> m_ImageAvailableSemaphores = { };
		Array<VkSemaphore> m_RenderFinishedSemaphores = { };

		Array<VkFence> m_InFlightFences = { };
		Array<VkFence> m_InFlightImages = { };
	};
}

#endif