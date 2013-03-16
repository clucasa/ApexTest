#include "ZeusRenderResources.h"
// SampleApexRenderResources.cpp

/*******************************
* ZeusVertexBuffer
*********************************/

ZeusVertexBuffer::ZeusVertexBuffer(const physx::apex::NxUserRenderVertexBufferDesc& desc, ID3D11Device* dev, ID3D11DeviceContext* devcon) :
    mDevice(dev), mStride(0), mDevcon(devcon)
{
    
    for (physx::PxU32 i = 0; i < physx::apex::NxRenderVertexSemantic::NUM_SEMANTICS; i++)
	{
		physx::apex::NxRenderVertexSemantic::Enum apexSemantic = physx::apex::NxRenderVertexSemantic::Enum(i);
		physx::apex::NxRenderDataFormat::Enum apexFormat = desc.buffersRequest[i];
		
        if (apexFormat != physx::apex::NxRenderDataFormat::UNSPECIFIED)
        {
            mStride += physx::apex::NxRenderDataFormat::getFormatDataSize(apexFormat);
        }
	}


    D3D11_BUFFER_DESC d3ddesc;
    d3ddesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    d3ddesc.ByteWidth = desc.maxVerts * mStride;
    d3ddesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3ddesc.MiscFlags = 0;

    if(desc.hint == NxRenderBufferHint::STATIC)
    {
        d3ddesc.Usage = D3D11_USAGE_DEFAULT;
    }
    else if(desc.hint == NxRenderBufferHint::DYNAMIC)
    {
        d3ddesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else if(desc.hint == NxRenderBufferHint::STREAMING)
    {
        d3ddesc.Usage = D3D11_USAGE_DYNAMIC; //Until I know what better to use
    }
    else
        return;

    mDevice->CreateBuffer(&d3ddesc, NULL, &mVertexBuffer);
}

ZeusVertexBuffer::~ZeusVertexBuffer(void)
{
    if (mVertexBuffer)
	{
        mVertexBuffer->Release();
	}
}

bool ZeusVertexBuffer::getInteropResourceHandle(CUgraphicsResource& handle)
{
    return false; //For now we won't support CUDA
}

void ZeusVertexBuffer::writeBuffer(const physx::NxApexRenderVertexBufferData& data, physx::PxU32 firstVertex, physx::PxU32 numVertices)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT result;
    physx::apex::NxApexRenderSemanticData* verticesPtr;
    physx::apex::NxApexRenderSemanticData* srcData = (physx::apex::NxApexRenderSemanticData*) malloc(mStride*numVertices);
    // Lock the vertex buffer so it can be written to.
	result = mDevcon->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return;
	}

	// Get a pointer to the data in the vertex buffer.
    verticesPtr = (physx::apex::NxApexRenderSemanticData*)mappedResource.pData + (firstVertex * mStride);
    
	// Copy the data into the vertex buffer.
    

    for(physx::PxU32 i = 0; i < numVertices; i++)
    {
        for (physx::PxU32 j = 0; j < physx::apex::NxRenderVertexSemantic::NUM_SEMANTICS; j++)
	    {
		    physx::apex::NxRenderVertexSemantic::Enum apexSemantic = (physx::apex::NxRenderVertexSemantic::Enum)j;
		    const physx::apex::NxApexRenderSemanticData& semanticData = data.getSemanticData(apexSemantic);
		    if (semanticData.data)
		    {
                memcpy(srcData + (mStride * i), semanticData.data, semanticData.stride);
            }
	    }
    }

    memcpy(verticesPtr, srcData, (mStride * numVertices));

	// Unlock the vertex buffer.
	mDevcon->Unmap(mVertexBuffer, 0);

}



/*******************************
* ZeusIndexBuffer
*********************************/

ZeusIndexBuffer::ZeusIndexBuffer(const physx::apex::NxUserRenderIndexBufferDesc& desc, ID3D11Device* dev, ID3D11DeviceContext* devcon) :
    mDevice(dev), mDevcon(devcon), mPrimitiveType(desc.primitives), mStride(0)
{
    mStride = physx::apex::NxRenderDataFormat::getFormatDataSize(desc.format);
    D3D11_BUFFER_DESC d3ddesc;
    d3ddesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    d3ddesc.ByteWidth = desc.maxIndices * mStride;
    d3ddesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    d3ddesc.MiscFlags = 0;
    
    if(desc.hint == NxRenderBufferHint::STATIC)
    {
        d3ddesc.Usage = D3D11_USAGE_DEFAULT;
    }
    else if(desc.hint == NxRenderBufferHint::DYNAMIC)
    {
        d3ddesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else if(desc.hint == NxRenderBufferHint::STREAMING)
    {
        d3ddesc.Usage = D3D11_USAGE_DYNAMIC; //Until I know what better to use
    }
    else
        return;

    mDevice->CreateBuffer(&d3ddesc, NULL, &mIndexBuffer);

}

ZeusIndexBuffer::~ZeusIndexBuffer(void)
{
    if(mIndexBuffer)
    {
        mIndexBuffer->Release();
    }
}

bool ZeusIndexBuffer::getInteropResourceHandle(CUgraphicsResource& handle)
{
    return false;
}

void ZeusIndexBuffer::writeBuffer(const void* srcData, physx::PxU32 srcStride, physx::PxU32 firstDestElement, physx::PxU32 numElements)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT result;
    physx::apex::NxApexRenderSemanticData* dstData;
    //physx::apex::NxApexRenderSemanticData* srcData;

    // Lock the vertex buffer so it can be written to.
	result = mDevcon->Map(mIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return;
	}

	// Get a pointer to the data in the vertex buffer.
    dstData = (physx::apex::NxApexRenderSemanticData*)mappedResource.pData + (firstDestElement * mStride);
    
	// Copy the data into the vertex buffer.
    

   /* for(physx::PxU32 i = 0; i < numVertices; i++)
    {
        for (physx::PxU32 j = 0; j < physx::apex::NxRenderVertexSemantic::NUM_SEMANTICS; j++)
	    {
		    physx::apex::NxRenderVertexSemantic::Enum apexSemantic = (physx::apex::NxRenderVertexSemantic::Enum)j;
		    const physx::apex::NxApexRenderSemanticData& semanticData = data.getSemanticData(apexSemantic);
		    if (semanticData.data)
		    {
                memcpy(srcData + (mStride * i), semanticData.data, semanticData.stride);
            }
	    }
    }*/

    memcpy(dstData, (physx::apex::NxApexRenderSemanticData*)srcData, (mStride * numElements));

	// Unlock the vertex buffer.
	mDevcon->Unmap(mIndexBuffer, 0);
}

/*******************************
* ZeusSurfaceBuffer
*********************************/

ZeusSurfaceBuffer::ZeusSurfaceBuffer(const physx::apex::NxUserRenderSurfaceBufferDesc& desc)
{

}

ZeusSurfaceBuffer::~ZeusSurfaceBuffer(void)
{

}

void ZeusSurfaceBuffer::writeBuffer(const void* srcData, physx::PxU32 srcStride, physx::PxU32 firstDestElement, physx::PxU32 width, physx::PxU32 height, physx::PxU32 depth)
{

}


/*******************************
* ZeusBoneBuffer
*********************************/

ZeusBoneBuffer::ZeusBoneBuffer(const physx::apex::NxUserRenderBoneBufferDesc& desc)
{

}

ZeusBoneBuffer::~ZeusBoneBuffer(void)
{

}

void ZeusBoneBuffer::writeBuffer(const physx::apex::NxApexRenderBoneBufferData& data, physx::PxU32 firstBone, physx::PxU32 numBones)
{

}


/*******************************
* ZeusInstanceBuffer
*********************************/

ZeusInstanceBuffer::ZeusInstanceBuffer(const physx::apex::NxUserRenderInstanceBufferDesc& desc)
{
	mMaxInstances = desc.maxInstances;
	mInstanceBuffer = new struct InstanceBuffer[mMaxInstances];
}

ZeusInstanceBuffer::~ZeusInstanceBuffer(void)
{
	if (mInstanceBuffer)
	{
		delete [] mInstanceBuffer;
	}
}

void ZeusInstanceBuffer::writeBuffer(const physx::apex::NxApexRenderInstanceBufferData& data, physx::PxU32 firstInstance, physx::PxU32 numInstances)
{
	for(physx::PxU32 j = 0; j < numInstances; j++)
	{
		for (physx::PxU32 i = 0; i < physx::apex::NxRenderInstanceSemantic::NUM_SEMANTICS; i++)
		{
			physx::apex::NxRenderInstanceSemantic::Enum semantic = (physx::apex::NxRenderInstanceSemantic::Enum)i;
			const physx::apex::NxApexRenderSemanticData& semanticData = data.getSemanticData(semantic);
			if (semanticData.data)
			{
				
				switch (i)
				{
				case physx::apex::NxRenderInstanceSemantic::POSITION:
					//mInstanceBuffer[j + firstInstance].Position = semanticData.data;
					break;
				case physx::apex::NxRenderInstanceSemantic::ROTATION_SCALE:
					
					break;
				case physx::apex::NxRenderInstanceSemantic::VELOCITY_LIFE:
					
					break;
				case physx::apex::NxRenderInstanceSemantic::DENSITY:

					break;
				case physx::apex::NxRenderInstanceSemantic::UV_OFFSET:

					break;
				case physx::apex::NxRenderInstanceSemantic::LOCAL_OFFSET:

					break;
				}
			}
		}
	}
}

bool ZeusInstanceBuffer::getInteropResourceHandle(CUgraphicsResource& handle)
{
	return false;
}


/*******************************
* ZeusSpriteBuffer
*********************************/

ZeusSpriteBuffer::ZeusSpriteBuffer(const physx::apex::NxUserRenderSpriteBufferDesc& desc)
{

}

ZeusSpriteBuffer::~ZeusSpriteBuffer(void)
{

}

bool ZeusSpriteBuffer::getInteropResourceHandle(CUgraphicsResource& handle)
{
	return false;
}

void ZeusSpriteBuffer::writeBuffer(const physx::apex::NxApexRenderSpriteBufferData& data, physx::PxU32 firstSprite, physx::PxU32 numSprites)
{

}


/*******************************
* ZeusRenderResource
*********************************/

ZeusRenderResource::ZeusRenderResource(const physx::apex::NxUserRenderResourceDesc& desc)
{
	mBoneBuffer = static_cast<ZeusBoneBuffer*>(desc.boneBuffer);
	mIndexBuffer = static_cast<ZeusIndexBuffer*>(desc.indexBuffer);
	mInstanceBuffer = static_cast<ZeusInstanceBuffer*>(desc.instanceBuffer);
	mSpriteBuffer = static_cast<ZeusSpriteBuffer*>(desc.spriteBuffer);

	mNumVertexBuffers = desc.numVertexBuffers;
	for(PxU32 i = 0; i < mNumVertexBuffers; i++)
	{
		mVertexBuffers[i] = static_cast<ZeusVertexBuffer*>(desc.vertexBuffers[i]);
	}

	setVertexBufferRange(desc.firstVertex, desc.numVerts);
	setIndexBufferRange(desc.firstIndex, desc.numIndices);
	setBoneBufferRange(desc.firstBone, desc.numBones);
	setInstanceBufferRange(desc.firstInstance, desc.numInstances);
	setSpriteBufferRange(desc.firstSprite, desc.numSprites);
	setMaterial(desc.material);
}

ZeusRenderResource::~ZeusRenderResource()
{
	if (mVertexBuffers)
	{
		delete [] mVertexBuffers;
	}
}

void ZeusRenderResource::setVertexBufferRange(physx::PxU32 firstVertex, physx::PxU32 numVerts)
{

}

void ZeusRenderResource::setIndexBufferRange(physx::PxU32 firstIndex, physx::PxU32 numIndices)
{

}

void ZeusRenderResource::setBoneBufferRange(physx::PxU32 firstBone, physx::PxU32 numBones)
{

}

void ZeusRenderResource::setInstanceBufferRange(physx::PxU32 firstInstance, physx::PxU32 numInstances)
{

}

void ZeusRenderResource::setSpriteBufferRange(physx::PxU32 firstSprite, physx::PxU32 numSprites)
{

}

void ZeusRenderResource::setMaterial(void* material)
{

}


