#pragma once
#include <vector>
#include <stdint.h>
#include "vulkan\vulkan.h"
#include "graphics\RHI\RHIResource.h"

class CVulkanShader
{
public:
	VkShaderModule m_shaderModule;
};

class CVulkanVertexShader : public CRHIVertexShader, public CVulkanShader
{
public:

};

class CVulkanPixelShader : public CRHIPixelShader, public CVulkanShader
{
public:

};

class CVulkanGraphicsPipelineState : public CRHIGraphicsPipelineState
{
public:
	VkPipeline m_vkPipeline;
};