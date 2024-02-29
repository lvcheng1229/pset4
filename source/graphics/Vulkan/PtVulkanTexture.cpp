#include <assert.h>

#include "PtVkCommon.h"
#include "PtVulkanDynamicRHI.h"

VkImageUsageFlags GetVulkanImageUsage(ETextureCreateFlags texFlag)
{
    VkImageUsageFlags imgUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (texFlag == TexCreate_ShaderResource)
    {
        imgUsage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    return imgUsage;
}

std::shared_ptr<CRHITexture2D> CVulkanDynamicRHI::RHICreateTexture2D(const void* pInitData, uint32_t width, uint32_t height, PtGfx::IMG_DATA_FORMAT dataFormat, PtGfx::IMG_NUM_FORMAT numFormat, ETextureCreateFlags texFlag)
{
	VkFormat texForamt = GetVkImgFormatFromAMDFormat(dataFormat, numFormat);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = texForamt;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = GetVulkanImageUsage(texFlag);
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    std::shared_ptr<CVulkanTexture2D> vkTexture2D = std::make_shared<CVulkanTexture2D>();
    
    VmaAllocationCreateInfo vmaAllocCreateInfo = {};
    vmaAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    VULKAN_VARIFY(vmaCreateImage(m_device.m_vmaAllocator, &imageInfo, &vmaAllocCreateInfo, &vkTexture2D->m_image, &vkTexture2D->m_vmaAlloc, &vkTexture2D->m_vmaAllocInfo));

    if (pInitData != nullptr)
    {
		VkBufferCreateInfo srcBufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		srcBufferInfo.size = width * height * GetImageDataFormatSizeInByte(dataFormat);
		srcBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VmaAllocationCreateInfo srcAllocInfo = {};
		srcAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        srcAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VkBuffer stageBuffer;
		VmaAllocation vmaAlloc;
		VmaAllocationInfo vmaAllocInfo;
		vmaCreateBuffer(m_device.m_vmaAllocator, &srcBufferInfo, &srcAllocInfo, &stageBuffer, &vmaAlloc, &vmaAllocInfo);
		memcpy(vmaAllocInfo.pMappedData, pInitData, srcBufferInfo.size);

        PtTransistionImageLayout(vkTexture2D->m_image, texForamt, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
       
        VkCommandBuffer copyCommandBuffer = PtBeginImmediateCommandBuffer();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width,height,1 };
        vkCmdCopyBufferToImage(copyCommandBuffer, stageBuffer, vkTexture2D->m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        PtEndImmediateCommandBuffer(copyCommandBuffer);

        PtTransistionImageLayout(vkTexture2D->m_image, texForamt, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = vkTexture2D->m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = texForamt;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VULKAN_VARIFY(vkCreateImageView(m_device.m_vkDevice, &viewInfo, nullptr, &vkTexture2D->m_view));
    }

	return vkTexture2D;
}

void CVulkanDynamicRHI::PtTransistionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = PtBeginImmediateCommandBuffer();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else 
    {
        assert(false);
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    PtEndImmediateCommandBuffer(commandBuffer);
}