#pragma once

#ifdef Z_VULKAN

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class Device;

	class Texture final
	{
	public:
		Texture(vulkan::Device& device);
		~Texture();

	private:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

	private:
		vulkan::Device& m_Device;

		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
	};
}

#endif