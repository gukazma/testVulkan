#include "testVulkan.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "Core.hpp"

namespace testVulkan {
    void testVulkan()
    {
        testVulkan::VulkanInstace* vulkanInstance = new testVulkan::VulkanInstace();

        uint32_t deviceCount = 0;
        if (vulkanInstance->m_VulkanInstance == nullptr) {
            std::cout << "vulkan is nullptr" << std::endl;
        }
        VkResult rnt = vkEnumeratePhysicalDevices(vulkanInstance->m_VulkanInstance, &deviceCount, VK_NULL_HANDLE);
        AssertLog(rnt==VK_SUCCESS, "Enumerate Physical Devices number failed!");

        if (deviceCount == 0) {
            delete vulkanInstance;
            throw std::runtime_error("Can't find pyhsical devices!");
        }
        else {
            std::cout << "Physical Device number : " << deviceCount << std::endl;
        }

        std::vector<VkPhysicalDevice> pyhsicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(vulkanInstance->m_VulkanInstance, &deviceCount, pyhsicalDevices.data());
        AssertLog(rnt==VK_SUCCESS, "Enumerate Physical Devices vector failed!");

        std::vector<VkPhysicalDevice> computeDevice;
        for (auto const& device : pyhsicalDevices) {
            if (VulkanDevice::CanUseAsCompute(device)) {
                computeDevice.push_back(device);
            }
        }
        std::cout << "Compute Physical Device number : " << computeDevice.size() << std::endl;
    }
}