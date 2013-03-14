#include "ZeusRenderResourceManager.h"

ZeusRenderResourceManager::ZeusRenderResourceManager()
{

}

ZeusRenderResourceManager::~ZeusRenderResourceManager()
{

}

physx::apex::NxUserRenderVertexBuffer* ZeusRenderResourceManager::createVertexBuffer(const physx::apex::NxUserRenderVertexBufferDesc& desc)
{
    NxUserRenderVertexBuffer* buffer = 0;

	/*unsigned int numSemantics = 0;
	for (unsigned int i = 0; i < physx::apex::NxRenderVertexSemantic::NUM_SEMANTICS; i++)
	{
		numSemantics += desc.buffersRequest[i] != physx::apex::NxRenderDataFormat::UNSPECIFIED ? 1 : 0;
	}
	PX_ASSERT(desc.isValid());
	if (desc.isValid() && numSemantics > 0)
	{
		buffer = new ZeusVertexBuffer(desc);
		m_numVertexBuffers++;
	}*/

	return buffer;
}

void ZeusRenderResourceManager::releaseVertexBuffer(physx::apex::NxUserRenderVertexBuffer& buffer)
{
	/*PX_ASSERT(m_numVertexBuffers > 0);
	m_numVertexBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderIndexBuffer* ZeusRenderResourceManager::createIndexBuffer(const physx::apex::NxUserRenderIndexBufferDesc& desc)
{
    NxUserRenderIndexBuffer* buffer = 0;
	/*PX_ASSERT(desc.isValid());
	if (desc.isValid())
	{
		buffer = new ZeusIndexBuffer(desc);
		m_numIndexBuffers++;
	}*/
	return buffer;
}

void ZeusRenderResourceManager::releaseIndexBuffer(physx::apex::NxUserRenderIndexBuffer& buffer)
{
	/*PX_ASSERT(m_numIndexBuffers > 0);
	m_numIndexBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderSurfaceBuffer* ZeusRenderResourceManager::createSurfaceBuffer(const physx::apex::NxUserRenderSurfaceBufferDesc& desc)
{
    NxUserRenderSurfaceBuffer* buffer = 0;
	/*PX_ASSERT(desc.isValid());
	if (desc.isValid())
	{
		buffer = new ZeusSurfaceBuffer(desc);
		m_numSurfaceBuffers++;
	}*/
	return buffer;
}

void ZeusRenderResourceManager::releaseSurfaceBuffer(physx::apex::NxUserRenderSurfaceBuffer& buffer)
{
	/*PX_ASSERT(m_numSurfaceBuffers > 0);
	m_numSurfaceBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderBoneBuffer* ZeusRenderResourceManager::createBoneBuffer(const physx::apex::NxUserRenderBoneBufferDesc& desc)
{
    NxUserRenderBoneBuffer* buffer = 0;
	/*PX_ASSERT(desc.isValid());
	if (desc.isValid())
	{
		buffer = new ZeusBoneBuffer(desc);
		m_numBoneBuffers++;
	}*/
	return buffer;
}

void ZeusRenderResourceManager::releaseBoneBuffer(physx::apex::NxUserRenderBoneBuffer& buffer)
{
	/*PX_ASSERT(m_numBoneBuffers > 0);
	m_numBoneBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderInstanceBuffer* ZeusRenderResourceManager::createInstanceBuffer(const physx::apex::NxUserRenderInstanceBufferDesc& desc)
{
    NxUserRenderInstanceBuffer* buffer = 0;
	/*PX_ASSERT(desc.isValid());
	if (desc.isValid())
	{
		buffer = new ZeusInstanceBuffer(desc);
		m_numBoneBuffers++;
	}*/
	return buffer;
}

void ZeusRenderResourceManager::releaseInstanceBuffer(physx::apex::NxUserRenderInstanceBuffer& buffer)
{
	/*PX_ASSERT(m_numInstanceBuffers > 0);
	m_numInstanceBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderSpriteBuffer* ZeusRenderResourceManager::createSpriteBuffer(const physx::apex::NxUserRenderSpriteBufferDesc& desc)
{
    NxUserRenderSpriteBuffer* buffer = 0;
	//PX_ASSERT(desc.isValid());
	//if (desc.isValid())
	//{
	//	// convert SB to VB
	//	buffer = new ZeusSpriteBuffer(desc);
	//	m_numSpriteBuffers++;
	//}
	return buffer;
}

void ZeusRenderResourceManager::releaseSpriteBuffer(physx::apex::NxUserRenderSpriteBuffer& buffer)
{
	/*PX_ASSERT(m_numVertexBuffers > 0);
	m_numSpriteBuffers--;
	delete &buffer;*/
}


physx::apex::NxUserRenderResource* ZeusRenderResourceManager::createResource(const physx::apex::NxUserRenderResourceDesc& desc)
{
   	NxUserRenderResource* resource = 0;
	/*PX_ASSERT(desc.isValid());
	if (desc.isValid())
	{
		resource = new ZeusRenderResource(desc);
		m_numResources++;
	}*/
	return resource;
}

void ZeusRenderResourceManager::releaseResource(physx::apex::NxUserRenderResource& resource)
{
	/*PX_ASSERT(m_numResources > 0);
	m_numResources--;
	delete &resource;*/
}


physx::PxU32 ZeusRenderResourceManager::getMaxBonesForMaterial(void* material)
{
	return 0;
}

// change the material of a render resource
void setMaterial(physx::apex::NxUserRenderResource& resource, void* material)
{
	//static_cast<ZeusRenderResource&>(resource).setMaterial(material);
}

