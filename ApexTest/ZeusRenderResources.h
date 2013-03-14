//ZeusRenderResources.h
#ifndef ZEUS_RENDER_RESOURCES
#define ZEUS_RENDER_RESOURCES
#include "ZeusRenderResourceManager.h"

#include <NxUserRenderer.h>
#include <NxUserRenderResourceManager.h>

#include <NxApexRenderContext.h>
#include <NxUserRenderBoneBuffer.h>
#include <NxUserRenderIndexBuffer.h>
#include <NxUserRenderInstanceBuffer.h>
#include <NxUserRenderResource.h>
#include <NxUserRenderSpriteBuffer.h>
#include <NxUserRenderSurfaceBuffer.h>
#include <NxUserRenderVertexBuffer.h>

#include <vector>
using namespace std;

//namespace ZuesRenderer
//{
//
//}

/*******************************
* ZeusVertexBuffer
*********************************/

class ZeusVertexBuffer : public physx::apex::NxUserRenderVertexBuffer
{
public:
    
    ZeusVertexBuffer(const physx::apex::NxUserRenderVertexBufferDesc& desc);
    virtual ~ZeusVertexBuffer(void);

    virtual bool getInteropResourceHandle(CUgraphicsResource& handle);

private:
    virtual void writeBuffer(const physx::NxApexRenderVertexBufferData& data, physx::PxU32 firstVertex, physx::PxU32 numVertices);
};


/*******************************
* ZeusIndexBuffer
*********************************/

class ZeusIndexBuffer : public physx::apex::NxUserRenderIndexBuffer
{
public:
	
	ZeusIndexBuffer(const physx::apex::NxUserRenderIndexBufferDesc& desc);
	virtual ~ZeusIndexBuffer(void);

    virtual bool getInteropResourceHandle(CUgraphicsResource& handle);

private:
	virtual void writeBuffer(const void* srcData, physx::PxU32 srcStride, physx::PxU32 firstDestElement, physx::PxU32 numElements);
};


/*******************************
* ZeusSurfaceBuffer
*********************************/

class ZeusSurfaceBuffer : public physx::apex::NxUserRenderSurfaceBuffer
{
public:
	
	ZeusSurfaceBuffer(const physx::apex::NxUserRenderSurfaceBufferDesc& desc);
	virtual ~ZeusSurfaceBuffer(void);


private:
	virtual void writeBuffer(const void* srcData, physx::PxU32 srcStride, physx::PxU32 firstDestElement, physx::PxU32 width, physx::PxU32 height, physx::PxU32 depth = 1);

};

/*******************************
* ZeusBoneBuffer
*********************************/

class ZeusBoneBuffer : public physx::apex::NxUserRenderBoneBuffer
{
public:
	
	ZeusBoneBuffer(const physx::apex::NxUserRenderBoneBufferDesc& desc);
	virtual ~ZeusBoneBuffer(void);

private:
	virtual void writeBuffer(const physx::apex::NxApexRenderBoneBufferData& data, physx::PxU32 firstBone, physx::PxU32 numBones);
};

/*******************************
* ZeusInstanceBuffer
*********************************/

class ZeusInstanceBuffer : public physx::apex::NxUserRenderInstanceBuffer
{
public:
	
	ZeusInstanceBuffer(const physx::apex::NxUserRenderInstanceBufferDesc& desc);
	virtual ~ZeusInstanceBuffer(void);

	virtual void writeBuffer(const physx::apex::NxApexRenderInstanceBufferData& data, physx::PxU32 firstInstance, physx::PxU32 numInstances);

	virtual bool getInteropResourceHandle(CUgraphicsResource& handle);
private:

};

/*******************************
* ZeusSpriteBuffer
*********************************/

class ZeusSpriteBuffer : public physx::apex::NxUserRenderSpriteBuffer
{
public:
	
	ZeusSpriteBuffer(const physx::apex::NxUserRenderSpriteBufferDesc& desc);
	virtual ~ZeusSpriteBuffer(void);

	virtual bool getInteropResourceHandle(CUgraphicsResource& handle);

private:
	virtual void writeBuffer(const physx::apex::NxApexRenderSpriteBufferData& data, physx::PxU32 firstSprite, physx::PxU32 numSprites);

};


/*******************************
* ZeusRenderResource
*********************************/

class ZeusRenderResource : public physx::apex::NxUserRenderResource
{
public:
	
	ZeusRenderResource(const physx::apex::NxUserRenderResourceDesc& desc);
	virtual ~ZeusRenderResource();

public:
	void setVertexBufferRange(physx::PxU32 firstVertex, physx::PxU32 numVerts);
	void setIndexBufferRange(physx::PxU32 firstIndex, physx::PxU32 numIndices);
	void setBoneBufferRange(physx::PxU32 firstBone, physx::PxU32 numBones);
	void setInstanceBufferRange(physx::PxU32 firstInstance, physx::PxU32 numInstances);
	void setSpriteBufferRange(physx::PxU32 firstSprite, physx::PxU32 numSprites);

	virtual void setMaterial(void* material);

	physx::PxU32 getNbVertexBuffers() const
	{
		return mVertexBuffers.size();
	}

	physx::apex::NxUserRenderVertexBuffer* getVertexBuffer(physx::PxU32 index) const
	{
		physx::apex::NxUserRenderVertexBuffer* buffer = 0;
		PX_ASSERT(index < mVertexBuffers.size());
		if (index < mVertexBuffers.size())
		{
			buffer = mVertexBuffers[index];
		}
		return buffer;
	}

	physx::apex::NxUserRenderIndexBuffer* getIndexBuffer() const
	{
		return mIndexBuffer;
	}

	physx::apex::NxUserRenderBoneBuffer* getBoneBuffer()	const
	{
		return mBoneBuffer;
	}

	physx::apex::NxUserRenderInstanceBuffer* getInstanceBuffer()	const
	{
		return mInstanceBuffer;
	}

	physx::apex::NxUserRenderSpriteBuffer* getSpriteBuffer()	const
	{
		return mSpriteBuffer;
	}

private:
	vector<ZeusVertexBuffer*>	mVertexBuffers;
	
	ZeusIndexBuffer*			mIndexBuffer;

	ZeusBoneBuffer*				mBoneBuffer;

	ZeusInstanceBuffer*			mInstanceBuffer;

	ZeusSpriteBuffer*			mSpriteBuffer;
};


#endif