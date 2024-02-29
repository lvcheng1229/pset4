#pragma once
#include <vector>
#include <stdint.h>
#include "vk_mem_alloc.h"
#include "vulkan\vulkan.h"
#include "graphics\RHI\RHIResource.h"

struct SVulkanPipelineLayouts
{
	VkDescriptorSetLayout vtxDescSetLayout;
	VkDescriptorSetLayout pixDescSetLayout;
};

struct SVulkanPipelineDescSet
{
	bool bVtxHasBind = false;
	bool bPixHasBind = false;
	VkDescriptorSet vtxDescSet;
	VkDescriptorSet pixDescSet;
};

class CVulkanTexture2D : public CRHITexture2D
{
public:
	VkImage m_image;
	VkImageView m_view;
	VmaAllocation m_vmaAlloc;
	VmaAllocationInfo m_vmaAllocInfo;
};

class CVulkanBuffer : public CRHIBuffer
{
public:
	VkBuffer m_buffer;
	VmaAllocation m_vmaAlloc;
	VmaAllocationInfo m_vmaAllocInfo;
};

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
	SVulkanPipelineDescSet m_pipelineDescSet;
	VkPipelineLayout m_pipelineLayout;
};

class CVulkanRenderPass : public CRHIRenderPass
{
public:
	VkRenderPass m_vkRenderPass;
};