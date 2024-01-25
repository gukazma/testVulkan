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
        std::cout << "test!!!!!!!!!!!!!!!!!!!" << std::endl;
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
        if (computeDevice.empty()) {
            vulkanInstance->Destroy();
            throw std::runtime_error("Compute Device is empty!");
        }

        VulkanDevice* testDevice;
        for (auto& device : computeDevice) {
            auto pdevice = VulkanDevice::Create(device, false, false);
            testDevice = pdevice;
            if(!pdevice->InitializeVKDevice()) throw std::runtime_error("Initialize vkDevice failed!");
            if(!pdevice->CalculateDeviceMemoryType()) throw std::runtime_error("Can't get calculate memory type!");
            if(!pdevice->CalculateImageMemoryType()) throw std::runtime_error("Can't get calculate image memory type!");
            if(!pdevice->CalculateHostMemoryType()) throw std::runtime_error("Can't get calculate host memory type!");
        }

        int m_size = 1073741824;
        VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        VkBuffer m_vkBuffer;
        VkDeviceMemory m_vkDeviceMemory;
        int m_offset = 0;
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = NULL;
        allocInfo.allocationSize = m_size;
        allocInfo.memoryTypeIndex = testDevice->m_hostMemoryType;

        VkResult nErr = vkAllocateMemory(testDevice->m_vkDevice, &allocInfo, nullptr, &m_vkDeviceMemory);
        if (nErr != VK_SUCCESS)
        {
            throw std::runtime_error("vkAllocateMemory failed!");
        }

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = m_size;
        bufferInfo.pNext = NULL;
        bufferInfo.flags = 0;
        bufferInfo.queueFamilyIndexCount = 0;
        bufferInfo.pQueueFamilyIndices = NULL;
        bufferInfo.usage = usageFlags;

        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        nErr = vkCreateBuffer(testDevice->m_vkDevice, &bufferInfo, nullptr, &m_vkBuffer);
        if (nErr != VK_SUCCESS)
        {
            throw std::runtime_error("vkCreateBuffer failed!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(testDevice->m_vkDevice, m_vkBuffer, &memRequirements);

        nErr = vkBindBufferMemory(testDevice->m_vkDevice, m_vkBuffer, m_vkDeviceMemory, m_offset);
        if (nErr != VK_SUCCESS)
        {
            throw std::runtime_error("vkAllocateMemory failed!");
        }

        uint64_t memorySize1,memorySize2; 
        memorySize1 = 1500;
        // memorySize1 = memorySize1 << 20;
        std::cout << "[Info] Input Memory Size:";
        
        std::cin >> memorySize1;
        memorySize1 = memorySize1 << 20;
        memorySize2 = 1500;
        memorySize2 = memorySize2 << 20;
        VkMemoryAllocateInfo allocInfo_image1 = {};
        allocInfo_image1.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo_image1.pNext = NULL;
        allocInfo_image1.allocationSize = memorySize1;
        allocInfo_image1.memoryTypeIndex = 2;

        VkMemoryAllocateInfo allocInfo_image2 = {};
        allocInfo_image2.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo_image2.pNext = NULL;
        allocInfo_image2.allocationSize = 1500 << 20;
        allocInfo_image2.memoryTypeIndex = 2;

        VkDeviceMemory m_memoryBlocks1;
        VkDeviceMemory m_memoryBlocks2;
        nErr = vkAllocateMemory(testDevice->m_vkDevice, &allocInfo_image1, nullptr, &m_memoryBlocks1);
        if (nErr != VK_SUCCESS)
        {
            throw std::runtime_error("1111111111111111vkAllocateMemory failed!!!!!");
        }
        // nErr = vkAllocateMemory(testDevice->m_vkDevice, &allocInfo_image2, nullptr, &m_memoryBlocks2);
        // if (nErr != VK_SUCCESS)
        // {
        //     throw std::runtime_error("22222222222222222vkAllocateMemory failed!!!!!");
        // }

    }
}
