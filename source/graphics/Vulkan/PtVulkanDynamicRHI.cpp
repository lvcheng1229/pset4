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

std::shared_ptr<CRHIGraphicsPipelineState> CVulkanDynamicRHI::RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)
{
    // TODO cache

    // create shader stage
    CVulkanVertexShader* pVkVertexShader = static_cast<CVulkanVertexShader*>(psoInitDesc.m_pVertexShader);
    CVulkanPixelShader* pVkPixelShader = static_cast<CVulkanPixelShader*>(psoInitDesc.m_pPixelShader);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = pVkVertexShader->m_shaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = pVkPixelShader->m_shaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // create vertex info
    std::vector<VkVertexInputAttributeDescription>attributeDescriptions;
    for (uint32_t index = 0; index < psoInitDesc.m_vertexElements.size(); index++)
    {
        VkVertexInputAttributeDescription InputAttributeDescription = {};
        InputAttributeDescription.binding = psoInitDesc.m_vertexElements[index].binding;
        InputAttributeDescription.location = psoInitDesc.m_vertexElements[index].location;
        InputAttributeDescription.offset = psoInitDesc.m_vertexElements[index].offset;
        InputAttributeDescription.format = GetVkFormatFromAMDFormat(psoInitDesc.m_vertexElements[index].m_vertexDataFormat, psoInitDesc.m_vertexElements[index].m_vertexNumFormat);
        attributeDescriptions.push_back(InputAttributeDescription);
    }

    std::vector< VkVertexInputBindingDescription> bindingDescriptions;
    for (uint32_t index = 0; index < psoInitDesc.m_vertexBindings.size(); index++)
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = index;
        bindingDescription.stride = psoInitDesc.m_vertexBindings[index].m_stride;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    {
        VkDescriptorSetLayoutBinding textureLayoutBinding{};
        textureLayoutBinding.binding = RemappingInfo.SetInfo.Types.size() - 1;
        textureLayoutBinding.descriptorCount = 1;
        textureLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        textureLayoutBinding.pImmutableSamplers = nullptr;
        textureLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        //vkCreateDescriptorSetLayout
        //vkCreatePipelineLayout
    }


    VkGraphicsPipelineCreateInfo PipelineInfo;
    PipelineInfo = {};
    PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    PipelineInfo.stageCount = 2;
    PipelineInfo.pStages = shaderStages;
    PipelineInfo.pVertexInputState = &vertexInputInfo;

    return std::shared_ptr<CRHIGraphicsPipelineState>();
}
