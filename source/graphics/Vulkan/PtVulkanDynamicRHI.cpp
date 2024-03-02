#include <string>
#include <unordered_map>
#include <iostream>
#include <format>
#include <assert.h>

#include "core\PtUtil.h"

#include "vk_mem_alloc.h"

#include "graphics\AMD\pal\core\pal.h"
#include "graphics\RHI\RHICommandList.h"


#include "PtVkCommon.h"
#include "PtVulkanResource.h"
#include "PtVulkanDynamicRHI.h"


static std::unordered_map<std::size_t, std::shared_ptr<CVulkanVertexShader>>gVertexShaderTable;
static std::unordered_map<std::size_t, std::shared_ptr<CVulkanPixelShader>>gPixelShaderTable;
static std::unordered_map<std::size_t, std::shared_ptr<CVulkanGraphicsPipelineState>> gPipelineMap;
static std::unordered_map<std::size_t, std::shared_ptr<CVulkanRenderPass>> gRenderPass;


CVulkanDynamicRHI::CVulkanDynamicRHI()
{
}

void CVulkanDynamicRHI::Init(void* windowHandle)
{
    m_device.Init(windowHandle);
    gRHICommandList.SetRHIContext(m_device.GetContext());
}

std::shared_ptr<CRHITexture2D> CVulkanDynamicRHI::RHIGetDeviceDefaultTexture(EDeviceDefaultTex deviceDefaultTex)
{
    if (deviceDefaultTex == EDeviceDefaultTex::DDT_BackBuffer)
    {
        return m_device.GetCurrentBackBufferTexture();
    }
    else
    {
        return m_device.GetDefaultDSTexture();
    }
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

size_t HashGraphicsPSO(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)
{
    size_t seed = 42;
    
    const std::size_t shaderHash = std::hash<std::string>{}(std::string((const char*)&psoInitDesc, sizeof(CRHIVertexShader*) + sizeof(CRHIPixelShader*)));
    const std::size_t vtxEleHash = std::hash<std::string>{}(std::string((const char*)psoInitDesc.m_vertexElements.data(), psoInitDesc.m_vertexElements.size() * sizeof(SVertexElement)));
    const std::size_t vtxBinHash = std::hash<std::string>{}(std::string((const char*)psoInitDesc.m_vertexBindings.data(), psoInitDesc.m_vertexBindings.size() * sizeof(SVertexBinding)));
    const std::size_t otherHash = std::hash<std::string>{}(std::string((const char*)&psoInitDesc.m_rtNum,
        sizeof(uint32_t) +
        sizeof(PtGfx::CB_BLEND0_CONTROL) * Pal::MaxColorTargets +
        sizeof(SRenderTarget) * Pal::MaxColorTargets +
        sizeof(CRHIDepthStencilState) +
        sizeof(CRHIRenderPass*)
    ));

    THashCombine(seed, shaderHash);
    THashCombine(seed, vtxEleHash);
    THashCombine(seed, vtxBinHash);
    THashCombine(seed, otherHash);
    return seed;
}

std::shared_ptr<CRHIGraphicsPipelineState> CVulkanDynamicRHI::RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)
{
    size_t hash = HashGraphicsPSO(psoInitDesc);
    auto iter = gPipelineMap.find(hash);
    
    if (iter != gPipelineMap.end())
    {
        return iter->second;
    }

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
        InputAttributeDescription.format = GetVkBufferFormatFromAMDFormat(psoInitDesc.m_vertexElements[index].m_vertexDataFormat, psoInitDesc.m_vertexElements[index].m_vertexNumFormat);
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

    SVulkanPipelineDescSet pipelineDescSet;
    VkPipelineLayout pipelineLayout = PtCreateVulkanGraphicsPipelineLayout(pVkVertexShader, pVkPixelShader, &pipelineDescSet);

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
        //blendStates[index].blendEnable = psoInitDesc.m_colorBelndControls[index].bitfields.ENABLE ? VK_TRUE : VK_FALSE;
        blendStates[index].blendEnable = VK_FALSE;
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
    colorBlending.logicOpEnable = VK_FALSE; // todo:
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

    CVulkanRenderPass* p_VkRenderPass = static_cast<CVulkanRenderPass*>(psoInitDesc.m_rhiRenderPass);

    VkRenderPass renderPass = p_VkRenderPass->m_vkRenderPass;

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
    ptVkraphicsPipelineState->m_pipelineLayout = pipelineLayout;
    ptVkraphicsPipelineState->m_pipelineDescSet = pipelineDescSet;
    ptVkraphicsPipelineState->m_pVertexShader = pVkVertexShader;
    ptVkraphicsPipelineState->m_pPixelShader = pVkPixelShader;
    vkCreateGraphicsPipelines(m_device.m_vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &ptVkraphicsPipelineState->m_vkPipeline);
    gPipelineMap[hash] = ptVkraphicsPipelineState;
    return ptVkraphicsPipelineState;
}

std::shared_ptr<CRHIRenderPass> CVulkanDynamicRHI::RHICreateRenderPass(const CRHIRenderPassInfo& rhiRenderPassInfo)
{
    size_t hash = std::hash<std::string>{}(std::string((const char*)&rhiRenderPassInfo, sizeof(CRHIRenderPassInfo)));
    auto iter = gRenderPass.find(hash);
    if (iter != gRenderPass.end())
    {
        return iter->second;
    }

    std::vector<VkAttachmentReference>colorAttachmentRefs;
    for (uint32_t index = 0; index < rhiRenderPassInfo.m_rtNum; index++)
    {
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachmentRefs.push_back(colorAttachmentRef);
    }


    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


    VkAttachmentDescription attachmentDesc[8];
    for (uint32_t index = 0; index < rhiRenderPassInfo.m_rtNum; index++)
    {
        attachmentDesc[index] = {};
        attachmentDesc[index].format = GetVkRenderTargetFormatFromAMDFormat(rhiRenderPassInfo.renderTargets[index].INFO.bitfields.FORMAT, rhiRenderPassInfo.renderTargets[index].INFO.bitfields.NUMBER_TYPE);
        attachmentDesc[index].samples = VK_SAMPLE_COUNT_1_BIT;
        //todo
        //attachmentDesc[index].loadOp = rhiRenderPassInfo.renderTargets[index].INFO.bitfields.FAST_CLEAR != 0 ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
        attachmentDesc[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDesc[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentDesc[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDesc[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        //TODO: current is one pass pipeline
        assert(rhiRenderPassInfo.m_rtNum == 1);
        attachmentDesc[index].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDesc[index].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }

    uint32_t totalRtNum = rhiRenderPassInfo.m_rtNum;
    if (rhiRenderPassInfo.m_dsState.bDepthTestEnable || rhiRenderPassInfo.m_dsState.bDepthWriteEnable)
    {
        totalRtNum++;

        uint32_t dsIndex = rhiRenderPassInfo.m_rtNum;
        attachmentDesc[dsIndex] = {};
        attachmentDesc[dsIndex].format = VK_FORMAT_D24_UNORM_S8_UINT;
        attachmentDesc[dsIndex].samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentDesc[dsIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDesc[dsIndex].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDesc[dsIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDesc[dsIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        attachmentDesc[dsIndex].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDesc[dsIndex].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = rhiRenderPassInfo.m_rtNum;
    subpass.pColorAttachments = colorAttachmentRefs.data();
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = totalRtNum;
    renderPassInfo.pAttachments = attachmentDesc;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    std::shared_ptr<CVulkanRenderPass> pVulkanRenderPass = std::make_shared<CVulkanRenderPass>();
    VULKAN_VARIFY(vkCreateRenderPass(m_device.m_vkDevice, &renderPassInfo, nullptr, &pVulkanRenderPass->m_vkRenderPass));
    gRenderPass[hash] = pVulkanRenderPass;

    return pVulkanRenderPass;
}

VkCommandBuffer CVulkanDynamicRHI::PtBeginImmediateCommandBuffer()
{
    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_device.m_commandPool;
    allocInfo.commandBufferCount = 1;
    vkAllocateCommandBuffers(m_device.m_vkDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void CVulkanDynamicRHI::PtEndImmediateCommandBuffer(VkCommandBuffer vkCmdBuffer)
{
    vkEndCommandBuffer(vkCmdBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkCmdBuffer;

    vkQueueSubmit(m_device.m_vkQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_device.m_vkQueue);

    vkFreeCommandBuffers(m_device.m_vkDevice, m_device.m_commandPool, 1, &vkCmdBuffer);
}