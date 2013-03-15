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
    physx::apex::NxApexRenderSemanticData* srcData;
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
