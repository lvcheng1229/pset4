#include <assert.h>
#include "PtVulkanDynamicRHI.h"

static VkBufferUsageFlags ConvertBufferUsage(EBufferUsage inUsage)
{
	VkBufferUsageFlags retVkUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	if ((inUsage & RHIBU_VB) != 0)
	{
		retVkUsage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	}

	if ((inUsage & RHIBU_IB) != 0)
	{
		retVkUsage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	}

	if ((inUsage & RHIBU_CB) != 0)
	{
		retVkUsage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	}
	
	return retVkUsage;
}

void CVulkanDynamicRHI::RHIUpdateBuffer(CRHIBuffer* pBuffer, uint8_t* pData, uint64_t bufferSize)
{
	CVulkanBuffer* pVkBuffer = static_cast<CVulkanBuffer*>(pBuffer);
	memcpy(pVkBuffer->m_vmaAllocInfo.pMappedData, pData, bufferSize);
}

std::shared_ptr<CRHIBuffer> CVulkanDynamicRHI::RHICreateBuffer(const void* pInitData, uint64_t nElementCount, uint64_t nStride, EBufferUsage bufferUsage)
{
	std::shared_ptr<CVulkanBuffer> pVkBuffer = std::make_shared<CVulkanBuffer>();
	pVkBuffer->m_elemCount = nElementCount;
	pVkBuffer->m_elemStride = nStride;

	{
		VkBufferCreateInfo dstBufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		dstBufferInfo.size = nElementCount * nStride;
		dstBufferInfo.usage = ConvertBufferUsage(bufferUsage);
		
		VmaAllocationCreateInfo vmaAllocCreateInfo = {};
		vmaAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

		if (bufferUsage == RHIBU_CB)
		{
			//Persistently mapped memory
			vmaAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
		}

		vmaCreateBuffer(m_device.m_vmaAllocator, &dstBufferInfo, &vmaAllocCreateInfo, &pVkBuffer->m_buffer, &pVkBuffer->m_vmaAlloc, &pVkBuffer->m_vmaAllocInfo);
	}
	if (pInitData != nullptr)
	{
		VkBufferCreateInfo srcBufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		srcBufferInfo.size = nElementCount * nStride;
		srcBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		VmaAllocationCreateInfo srcAllocInfo = {};
		srcAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
		srcAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
			VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VkBuffer stageBuffer;
		VmaAllocation vmaAlloc;
		VmaAllocationInfo vmaAllocInfo;
		vmaCreateBuffer(m_device.m_vmaAllocator, &srcBufferInfo, &srcAllocInfo, &stageBuffer, &vmaAlloc, &vmaAllocInfo);
		memcpy(vmaAllocInfo.pMappedData, pInitData, nElementCount * nStride);

		VkCommandBuffer commandBuffer = PtBeginImmediateCommandBuffer();

		VkBufferCopy copyRegion{};
		copyRegion.size = nElementCount * nStride;
		vkCmdCopyBuffer(commandBuffer, stageBuffer, pVkBuffer->m_buffer, 1, &copyRegion);

		PtEndImmediateCommandBuffer(commandBuffer);
	}

	return pVkBuffer;
}

void CVulkanContext::RHISetTexture2D(CRHITexture2D* tex, uint32_t imgIndex)
{
	CVulkanTexture2D* vkTexture2D = static_cast<CVulkanTexture2D*>(tex);
	if (m_gfxStateCache.m_texDesc[imgIndex].m_imgView != vkTexture2D->m_view)
	{
		m_gfxStateCache.m_texDesc[imgIndex].m_imgView = vkTexture2D->m_view;
	}
}

void CVulkanContext::RHISetConstantBuffer(CRHIBuffer* ctBuffer, uint32_t index)
{
	CVulkanBuffer* pVkBuffer = static_cast<CVulkanBuffer*>(ctBuffer);
	if (m_gfxStateCache.m_bufferDescs[index].m_buffer != pVkBuffer->m_buffer)
	{
		m_gfxStateCache.m_bufferDescs[index].m_buffer = pVkBuffer->m_buffer;
		m_gfxStateCache.m_bufferDescs[index].m_size = ctBuffer->m_elemCount * ctBuffer->m_elemStride;
	}
}

void CVulkanContext::RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset)
{
	if (m_gfxStateCache.m_vertexStreamings[bufferSlot].m_buffer != vtxBuffer)
	{
		m_gfxStateCache.m_vertexStreamings[bufferSlot].m_buffer = vtxBuffer;
		m_gfxStateCache.m_vertexStreamings[bufferSlot].offset = bufferOffset;
		m_gfxStateCache.m_bVertexBufferDirty[bufferSlot] = true;
	}
}

