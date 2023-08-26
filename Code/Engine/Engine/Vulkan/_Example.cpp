#include "EnginePCH.h"

#include "Engine/Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Vulkan/vulkan_core.h>

#include <direct.h>
#include <fstream>
#include <iostream>

namespace
{
	constexpr float s_QueuePriority = 1.f;
	constexpr int s_MaxFramesInFlight = 2;
	constexpr Vector2u s_WindowSize = { 800, 600 };

	const Array<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const Array<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	static bool framebufferResized = false;

	struct QueueFamily
	{
		Nullable<uint32> m_GraphicsFamily = {};
		Nullable<uint32> m_PresentFamily = {};
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{
		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		//auto app = reinterpret_cast<core::Application*>(glfwGetWindowUserPointer(window));	
		//app->framebufferResized = true;

		framebufferResized = true;
	}

	QueueFamily FindQueueFamily(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
	{
		QueueFamily result;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		uint32 i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				result.m_GraphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (queueFamily.queueCount > 0 && presentSupport) 
				result.m_PresentFamily = i;

			if (result.m_GraphicsFamily && result.m_PresentFamily)
				break;
			i++;
		}
		return result;
	}

	bool IsValidationLayerSupported() 
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		Array<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) 
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) 
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;
	}

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Array<VkSurfaceFormatKHR>& availableFormats) 
	{
		for (const auto& availableFormat : availableFormats) 
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB 
				&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
				return availableFormat;
		}
		return availableFormats[0];
	}

	VkPresentModeKHR ChooseSwapPresentMode(const Array<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes) 
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
				return availablePresentMode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) 
	{
		if (capabilities.currentExtent.width != UINT32_MAX) 
			return capabilities.currentExtent;

		VkExtent2D actualExtent = { s_WindowSize.x, s_WindowSize.y };
		actualExtent.width = 
			std::max(capabilities.minImageExtent.width, 
				std::min(capabilities.maxImageExtent.width,
					actualExtent.width));
		actualExtent.height =
			std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height,
					actualExtent.height));
		return actualExtent;
	}

	Array<char> ReadFile(const std::string& filename) 
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			return { };

		const size_t fileSize = (size_t)file.tellg();
		Array<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

}

int main3()
{
	glfwInit();

	// extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	Array<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	// window
	GLFWwindow* window = nullptr;
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


		window = glfwCreateWindow(s_WindowSize.x, s_WindowSize.y, "ZEngine", nullptr, nullptr);
		// glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	// vulkan::Device
	// instance
	VkInstance instance = VK_NULL_HANDLE;
	{
		if (!IsValidationLayerSupported())
			return 0;

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "LittleVulkanEngine App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = debugCallback;
		debugCreateInfo.pUserData = nullptr;
		debugCreateInfo.pNext = nullptr;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

		vkCreateInstance(&createInfo, nullptr, &instance);

		if (instance == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create an instance!");
	}

	// vulkan::Device
	// extensions
	{
		uint32 extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		Array<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		Set<str::String> availableExtensions;
		for (const auto& extension : extensions) 
			availableExtensions.insert(extension.extensionName);

		for (const auto& extension : requiredExtensions)
		{
			if (availableExtensions.find(extension) == availableExtensions.end())
				throw std::runtime_error("Missing required glfw extension!");
		}
	}

	// vulkan::Device
	// surface
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	{
		glfwCreateWindowSurface(instance, window, nullptr, &surface);

		if (surface == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create a surface!");
	}

	// vulkan::Device
	// physical device
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	{
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		Array<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const VkPhysicalDevice& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				continue;
			if (!deviceFeatures.geometryShader)
				continue;

			const QueueFamily queueFamily = FindQueueFamily(device, surface);
			if (!queueFamily.m_GraphicsFamily || !queueFamily.m_PresentFamily)
				continue;

			uint32 extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
			Array<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			auto HasRequiredExtensions = [&]() -> bool
			{
				Set<str::String> missingExtensions(deviceExtensions.begin(), deviceExtensions.end());
				for (const auto& extension : availableExtensions) 
					missingExtensions.erase(extension.extensionName);
				return missingExtensions.empty();
			};

			if (!HasRequiredExtensions())
				break;

			uint32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			Array<VkSurfaceFormatKHR> formats(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());

			if (formats.empty())
				continue;

			uint32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			Array<VkPresentModeKHR> presentModes(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());

			if (presentModes.empty())
				continue;

			physicalDevice = device;
			break;
		}

		if (physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to select a physical device!");
	}

	const QueueFamily queueFamily = FindQueueFamily(physicalDevice, surface);

	// vulkan::Device
	// logical device
	VkDevice logicalDevice = VK_NULL_HANDLE;
	{
		const Set<uint32> queueFamilies = { *queueFamily.m_GraphicsFamily, *queueFamily.m_PresentFamily };

		Array<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (uint32 queueFamilyIndex : queueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo;
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &s_QueuePriority;
			queueCreateInfo.flags = 0;
			queueCreateInfo.pNext = nullptr;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures;
		deviceFeatures.alphaToOne = VK_FALSE;
		deviceFeatures.depthBiasClamp = VK_FALSE;
		deviceFeatures.depthBounds = VK_FALSE;
		deviceFeatures.depthClamp = VK_FALSE;
		deviceFeatures.drawIndirectFirstInstance = VK_FALSE;
		deviceFeatures.dualSrcBlend = VK_FALSE;
		deviceFeatures.fillModeNonSolid = VK_FALSE;
		deviceFeatures.fragmentStoresAndAtomics = VK_FALSE;
		deviceFeatures.fullDrawIndexUint32 = VK_FALSE;
		deviceFeatures.geometryShader = VK_FALSE;
		deviceFeatures.imageCubeArray = VK_FALSE;
		deviceFeatures.independentBlend = VK_FALSE;
		deviceFeatures.inheritedQueries = VK_FALSE;
		deviceFeatures.largePoints = VK_FALSE;
		deviceFeatures.logicOp = VK_FALSE;
		deviceFeatures.multiDrawIndirect = VK_FALSE;
		deviceFeatures.multiViewport = VK_FALSE;
		deviceFeatures.occlusionQueryPrecise = VK_FALSE;
		deviceFeatures.pipelineStatisticsQuery = VK_FALSE;
		deviceFeatures.robustBufferAccess = VK_FALSE;
		deviceFeatures.samplerAnisotropy = VK_FALSE;
		deviceFeatures.sampleRateShading = VK_FALSE;
		deviceFeatures.shaderClipDistance = VK_FALSE;
		deviceFeatures.shaderCullDistance = VK_FALSE;
		deviceFeatures.shaderFloat64 = VK_FALSE;
		deviceFeatures.shaderImageGatherExtended = VK_FALSE;
		deviceFeatures.shaderInt16 = VK_FALSE;
		deviceFeatures.shaderInt64 = VK_FALSE;
		deviceFeatures.shaderResourceMinLod = VK_FALSE;
		deviceFeatures.shaderResourceResidency = VK_FALSE;
		deviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
		deviceFeatures.shaderStorageImageMultisample = VK_FALSE;
		deviceFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
		deviceFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
		deviceFeatures.shaderTessellationAndGeometryPointSize = VK_FALSE;
		deviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
		deviceFeatures.sparseBinding = VK_FALSE;
		deviceFeatures.sparseResidency16Samples = VK_FALSE;
		deviceFeatures.sparseResidency2Samples = VK_FALSE;
		deviceFeatures.sparseResidency4Samples = VK_FALSE;
		deviceFeatures.sparseResidency8Samples = VK_FALSE;
		deviceFeatures.sparseResidencyAliased = VK_FALSE;
		deviceFeatures.sparseResidencyBuffer = VK_FALSE;
		deviceFeatures.sparseResidencyImage2D = VK_FALSE;
		deviceFeatures.sparseResidencyImage3D = VK_FALSE;
		deviceFeatures.tessellationShader = VK_FALSE;
		deviceFeatures.textureCompressionASTC_LDR = VK_FALSE;
		deviceFeatures.textureCompressionBC = VK_FALSE;
		deviceFeatures.textureCompressionETC2 = VK_FALSE;
		deviceFeatures.variableMultisampleRate = VK_FALSE;
		deviceFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
		deviceFeatures.wideLines = VK_FALSE;

		VkDeviceCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		createInfo.flags = 0;
		createInfo.pNext = nullptr;

		vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);

		if (logicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create a logical device!");
	}

	// vulkan::Device
	// graphics queues
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	{
		vkGetDeviceQueue(logicalDevice, *queueFamily.m_GraphicsFamily, 0,&graphicsQueue);

		if (graphicsQueue == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to get graphics queue!");
	}

	// vulkan::Device
	// present queue
	VkQueue presentQueue = VK_NULL_HANDLE;
	{
		vkGetDeviceQueue(logicalDevice, *queueFamily.m_PresentFamily, 0, &presentQueue);

		if (presentQueue == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to get present queue!");
	}

	// vulkan::SwapChain
	// swap-chain
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	VkExtent2D swapChainExtent;
	VkFormat swapChainImageFormat;
	{
		VkSurfaceCapabilitiesKHR capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

		uint32 formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		Array<VkSurfaceFormatKHR> formats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

		uint32 presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		Array<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

		const VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(formats);
		const VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);
		swapChainExtent = ChooseSwapExtent(capabilities);
		swapChainImageFormat = surfaceFormat.format;

		uint32 imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0) 
			imageCount = std::min(imageCount, capabilities.maxImageCount);

		VkSwapchainCreateInfoKHR createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = swapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamily queueFamily = FindQueueFamily(physicalDevice, surface);
		uint32 queueFamilyIndices[] = { *queueFamily.m_GraphicsFamily, *queueFamily.m_PresentFamily };
		if (*queueFamily.m_GraphicsFamily != *queueFamily.m_PresentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else 
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		createInfo.flags = 0;
		createInfo.pNext = nullptr;

		vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain);

		if (swapChain == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create swap-chain!");
	}

	// vulkan::SwapChain
	// images
	Array<VkImage> swapChainImages;
	Array<VkImageView> swapChainImageViews;
	{
		uint32 swapChainImageCount;
		vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainImageCount, nullptr);
		swapChainImages.resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(logicalDevice, swapChain, &swapChainImageCount, swapChainImages.data());

		swapChainImageViews.resize(swapChainImageCount, VK_NULL_HANDLE);
		for (size_t i = 0; i < swapChainImages.size(); ++i) 
		{
			VkImageViewCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			createInfo.flags = 0;
			createInfo.pNext = nullptr;

			vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]);

			if (swapChainImageViews[i] == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create swap-chain image!");
		}
	}

	// vulkan::Pipeline
	// shaders
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	{
		const auto vertShader = ReadFile("Assets/Shaders/SimpleShader.vert.spv");
		const auto fragShader = ReadFile("Assets/Shaders/SimpleShader.frag.spv");

		auto LoadShaderModule = [&](const Array<char>& shaderData) -> VkShaderModule
		{
			VkShaderModuleCreateInfo createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shaderData.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());
			createInfo.flags = 0;
			createInfo.pNext = nullptr;

			VkShaderModule shaderModule = VK_NULL_HANDLE;
			vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule);
			return shaderModule;
		};

		vertShaderModule = LoadShaderModule(vertShader);
		fragShaderModule = LoadShaderModule(fragShader);

		if (vertShaderModule == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create vert shader module!");
		if (fragShaderModule == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create frag shader module!");
	}

	// vulkan::SwapChain
	// render pass
	VkRenderPass renderPass = VK_NULL_HANDLE;
	{
		VkAttachmentDescription colorAttachment;
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		colorAttachment.flags = 0;

		VkAttachmentReference colorAttachmentRef;
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.pDepthStencilAttachment = nullptr;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;
		subpass.pResolveAttachments = nullptr;
		subpass.flags = 0;

		VkSubpassDependency dependency;
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = 0;

		VkRenderPassCreateInfo renderPassInfo;
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		renderPassInfo.flags = 0;
		renderPassInfo.pNext = nullptr;

		vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass);

		if (renderPass == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create render pass!");
	}

	// vulkan::Pipeline
	// pipeline
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	{
		// Shaders
		VkPipelineShaderStageCreateInfo shaderStages[] = { {}, {} };

		VkPipelineShaderStageCreateInfo vertShaderStageInfo;
		vertShaderStageInfo.sType =	VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main"; // #note: shader entry point
		vertShaderStageInfo.pSpecializationInfo = nullptr;
		vertShaderStageInfo.flags = 0;
		vertShaderStageInfo.pNext = nullptr;
		shaderStages[0] = vertShaderStageInfo;

		VkPipelineShaderStageCreateInfo fragShaderStageInfo;
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main"; // #note: shader entry point
		fragShaderStageInfo.pSpecializationInfo = nullptr;
		fragShaderStageInfo.flags = 0;
		fragShaderStageInfo.pNext = nullptr;
		shaderStages[1] = fragShaderStageInfo;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.sType =	VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.flags = 0;
		vertexInputInfo.pNext = nullptr;

		// Input Assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly;
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
		inputAssembly.flags = 0;
		inputAssembly.pNext = nullptr;

		// Viewport
		VkViewport viewport;
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = (float)swapChainExtent.width;
		viewport.height = (float)swapChainExtent.height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		VkRect2D scissor;
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState;
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		viewportState.flags = 0;
		viewportState.pNext = nullptr;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer;
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.f;
		rasterizer.depthBiasClamp = 0.f;
		rasterizer.depthBiasSlopeFactor = 0.f;
		rasterizer.flags = 0;
		rasterizer.pNext = nullptr;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling;
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;
		multisampling.flags = 0;
		multisampling.pNext = nullptr;

		// Depth and Stencil
		// VkPipelineDepthStencilStateCreateInfo

		// Colour Blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		colorBlendAttachment.colorWriteMask = 
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT | 
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending;
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.f;
		colorBlending.blendConstants[1] = 0.f;
		colorBlending.blendConstants[2] = 0.f;
		colorBlending.blendConstants[3] = 0.f;
		colorBlending.flags = 0;
		colorBlending.pNext = nullptr;

		//// Dynamic State
		//VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };

		//VkPipelineDynamicStateCreateInfo dynamicState;
		//dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		//dynamicState.dynamicStateCount = 2;
		//dynamicState.pDynamicStates = dynamicStates;
		//dynamicState.flags = 0;
		//dynamicState.pNext = nullptr;

		// Uniform Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo;
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		pipelineLayoutInfo.flags = 0;
		pipelineLayoutInfo.pNext = nullptr;

		vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout);

		if (pipelineLayout == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr; // &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.flags = 0;
		pipelineInfo.pNext = nullptr;

		vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

		if (graphicsPipeline == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create graphics pipeline!");
	}

	// vulkan::SwapChain
	// swap-chain frame-buffers
	Array<VkFramebuffer> swapChainFramebuffers(swapChainImageViews.size(), VK_NULL_HANDLE);
	{
		for (size_t i = 0; i < swapChainImageViews.size(); i++) 
		{
			VkImageView attachments[] = { swapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo;
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;
			framebufferInfo.flags = 0;
			framebufferInfo.pNext = nullptr;

			vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);

			if (swapChainFramebuffers[i] == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create frame-buffer!");
		}
	}

	// vulkan::Device
	// command pool
	VkCommandPool commandPool = VK_NULL_HANDLE;
	{
		VkCommandPoolCreateInfo poolInfo;
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex =	*queueFamily.m_GraphicsFamily;
		poolInfo.flags = 0;
		poolInfo.pNext = nullptr;

		vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);

		if (commandPool == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to create command pool!");
	}

	// vulkan::Renderer
	// command buffers
	Array<VkCommandBuffer> commandBuffers(swapChainFramebuffers.size(), VK_NULL_HANDLE);
	{
		VkCommandBufferAllocateInfo allocInfo;
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32)commandBuffers.size();
		allocInfo.pNext = nullptr;

		vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data());

		for (int32 i = 0; i < commandBuffers.size(); ++i)
		{
			if (commandBuffers[i] == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create command buffer!");

			VkCommandBufferBeginInfo beginInfo;
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;
			beginInfo.pNext = nullptr;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) !=	VK_SUCCESS)
				throw std::runtime_error("Failed to begin command buffer!");

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			VkRenderPassBeginInfo renderPassInfo;
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapChainExtent;
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;
			renderPassInfo.pNext = nullptr;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to end command buffer!");
		}
	}

	// vulkan::SwapChain
	// semaphores
	Array<VkSemaphore> imageAvailableSemaphores(s_MaxFramesInFlight, VK_NULL_HANDLE);
	Array<VkSemaphore> renderFinishedSemaphores(s_MaxFramesInFlight, VK_NULL_HANDLE);
	Array<VkFence> inFlightFences(s_MaxFramesInFlight, VK_NULL_HANDLE);
	Array<VkFence> imagesInFlight(swapChainImages.size(), VK_NULL_HANDLE);
	{
		VkSemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = 0;
		semaphoreInfo.pNext = nullptr;

		for (auto& semaphore : imageAvailableSemaphores)
		{
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &semaphore);
			if (semaphore == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create available semaphore!");
		}

		for (auto& semaphore : renderFinishedSemaphores)
		{
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &semaphore);
			if (semaphore == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create finished semaphore!");
		}

		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		fenceInfo.pNext = nullptr;

		for (auto& fence : inFlightFences)
		{
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &fence);
			if (fence == VK_NULL_HANDLE)
				throw std::runtime_error("Failed to create fence!");
		}
	}

	// Update ////////////////////////////////////////////////////////////////

	uint32 currentFrame = 0;
	while (!glfwWindowShouldClose(window))
	{
		const uint32 syncIndex = currentFrame % s_MaxFramesInFlight;

		glfwPollEvents();

		vkWaitForFences(logicalDevice, 1, &inFlightFences[syncIndex], VK_TRUE, UINT64_MAX);

		uint32 imageIndex;
		const VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, imageAvailableSemaphores[syncIndex], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			framebufferResized = false;
			{
				int width = 0, height = 0;
				glfwGetFramebufferSize(window, &width, &height);
				while (width == 0 || height == 0) 
				{
					glfwGetFramebufferSize(window, &width, &height);
					glfwWaitEvents();
				}

				vkDeviceWaitIdle(logicalDevice);
			}

			continue;
		}
		else if (result != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(logicalDevice, 1, &imagesInFlight[imageIndex],	VK_TRUE, UINT64_MAX);

		 // Mark the image as now being in use by this frame
		imagesInFlight[imageIndex] = inFlightFences[syncIndex];

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[syncIndex] };
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[syncIndex] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSwapchainKHR swapChains[] = { swapChain };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(logicalDevice, 1, &inFlightFences[syncIndex]);

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[syncIndex]) != VK_SUCCESS)
			throw std::runtime_error("Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(presentQueue, &presentInfo);

		currentFrame++;
	}

	vkDeviceWaitIdle(logicalDevice);

	// Cleanup ///////////////////////////////////////////////////////////////
	for (const auto& fence : inFlightFences)
		vkDestroyFence(logicalDevice, fence, nullptr);
	for (const auto& semaphore : renderFinishedSemaphores)
		vkDestroySemaphore(logicalDevice, semaphore, nullptr);
	for (const auto& semaphore : imageAvailableSemaphores)
		vkDestroySemaphore(logicalDevice, semaphore, nullptr);
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	for (const auto& framebuffer : swapChainFramebuffers)
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
	for (const auto& imageView : swapChainImageViews) 
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}