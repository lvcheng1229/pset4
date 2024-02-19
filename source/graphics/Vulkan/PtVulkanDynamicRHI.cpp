#include <string>
#include <unordered_map>
#include <iostream>
#include <format>

#include "PtVkCommon.h"
#include "PtVulkanResource.h"
#include "PtVulkanDynamicRHI.h"

static std::unordered_map<std::size_t, std::shared_ptr<CVulkanVertexShader>>gVertexShaderTable;
static std::unordered_map<std::size_t, std::shared_ptr<CVulkanPixelShader>>gPixelShaderTable;

CVulkanDynamicRHI::CVulkanDynamicRHI()
{
}

void CVulkanDynamicRHI::Init(void* windowHandle)
{
    m_device.Init(windowHandle);
}


std::shared_ptr<CRHIVertexShader> CVulkanDynamicRHI::RHICreateVertexShader(const std::vector<uint8_t>& code)
{
    std::size_t shaderHash = std::hash<std::string>{}(std::string((char*)code.data(), code.size()));
    auto iter = gVertexShaderTable.find(shaderHash);
    if (iter != gVertexShaderTable.end())
    {
        return iter->second;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    std::shared_ptr<CVulkanVertexShader> retShader = std::make_shared<CVulkanVertexShader>();
    VULKAN_VARIFY(vkCreateShaderModule(m_device.m_vkDevice, &createInfo, nullptr, &retShader->m_shaderModule));
    gVertexShaderTable[shaderHash] = retShader;
    return retShader;
}

std::shared_ptr<CRHIPixelShader> CVulkanDynamicRHI::RHICreatePixelShader(const std::vector<uint8_t>& code)
{
    std::size_t shaderHash = std::hash<std::string>{}(std::string((char*)code.data(), code.size()));
    auto iter = gPixelShaderTable.find(shaderHash);
    if (iter != gPixelShaderTable.end())
    {
        return iter->second;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    std::shared_ptr<CVulkanPixelShader> retShader = std::make_shared<CVulkanPixelShader>();
    VULKAN_VARIFY(vkCreateShaderModule(m_device.m_vkDevice, &createInfo, nullptr, &retShader->m_shaderModule));
    gPixelShaderTable[shaderHash] = retShader;
    return retShader;
}
