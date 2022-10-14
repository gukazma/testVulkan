#include "VulkanInstance.h"
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace testVulkan {

    VulkanInstace::VulkanInstace()
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vulkan for GPU";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        m_Extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef WIN32
        m_Extensions.push_back("VK_KHR_win32_surface");
#else
        m_Extensions.push_back("VK_HKR_xcb_surface");
#endif

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_Extensions.size());
        createInfo.ppEnabledExtensionNames = m_Extensions.data();
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }
    void VulkanInstace::Destroy()
    {
        if (m_VulkanInstance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_VulkanInstance, NULL);
        }
    }

    VulkanInstace::~VulkanInstace()
    {
        Destroy();
    }
}