#include <string>
#include <unordered_map>
#include <iostream>
#include <format>

#include "graphics\AMD\pal\core\pal.h"

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
        bindingDescriptions.push_back(bindingDescription);
    }
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineLayout pipelineLayout = PtCreateVulkanGraphicsPipelineLayout(pVkVertexShader, pVkPixelShader);

    // inputAssembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // viewportState
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    // multi sampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // depth stencil
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.depthTestEnable = psoInitDesc.m_dsState.bDepthTestEnable;
    depthStencilInfo.depthWriteEnable = psoInitDesc.m_dsState.bDepthWriteEnable;
    depthStencilInfo.depthCompareOp = GetVkCmpFuncFromAMD(Pal::CompareFunc(psoInitDesc.m_dsState.zFunc));
    depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilInfo.stencilTestEnable = VK_FALSE;

    // blend states
    VkPipelineColorBlendAttachmentState blendStates[Pal::MaxColorTargets];
    for (int32_t index = 0; index < Pal::MaxColorTargets; index++)
    {
        blendStates[index] = {};
        blendStates[index].blendEnable = psoInitDesc.m_colorBelndControls[index].bitfields.ENABLE ? VK_TRUE : VK_FALSE;
        blendStates[index].srcColorBlendFactor = VkBlendFactor(Pal::BlendOp(psoInitDesc.m_colorBelndControls[index].bitfields.COLOR_SRCBLEND));
        blendStates[index].dstColorBlendFactor= VkBlendFactor(Pal::BlendOp(psoInitDesc.m_colorBelndControls[index].bitfields.COLOR_DESTBLEND));
        blendStates[index].colorBlendOp = VkBlendOp(Pal::SX_OPT_COMB_FCN(psoInitDesc.m_colorBelndControls[index].bitfields.COLOR_COMB_FCN));;
        blendStates[index].srcAlphaBlendFactor = VkBlendFactor(Pal::BlendOp(psoInitDesc.m_colorBelndControls[index].bitfields.ALPHA_SRCBLEND));
        blendStates[index].dstAlphaBlendFactor = VkBlendFactor(Pal::BlendOp(psoInitDesc.m_colorBelndControls[index].bitfields.ALPHA_DESTBLEND));
        blendStates[index].alphaBlendOp = VkBlendOp(Pal::SX_OPT_COMB_FCN(psoInitDesc.m_colorBelndControls[index].bitfields.ALPHA_COMB_FCN));;
        blendStates[index].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    }

    // color blending
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = psoInitDesc.m_rtNum;
    colorBlending.pAttachments = blendStates;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    
    std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkRenderPass renderPass = PtCreateVulkanRenderPass(psoInitDesc);

    VkGraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.flags = 0;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencilInfo;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    std::shared_ptr<CVulkanGraphicsPipelineState> ptVkraphicsPipelineState = std::make_shared<CVulkanGraphicsPipelineState>();
    vkCreateGraphicsPipelines(m_device.m_vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ptVkraphicsPipelineState->m_vkPipeline);
    return ptVkraphicsPipelineState;
}
