//***************************************************************************************
// ApexParticles.cpp
//***************************************************************************************
#include "ApexParticles.h"

ApexParticles::ApexParticles() :
    mParticleIosModule(0),
    mEmitterModule(0),
    mIofxModule(0)
{
	return;
}

ApexParticles::~ApexParticles()
{
	return;
}

void ApexParticles::Init(NxApexSDK* gApexSDK)
{
    PX_ASSERT(gApexSDK);
    NxApexCreateError            errorCode;


    mParticleIosModule = static_cast<NxModuleParticleIos*>(gApexSDK->createModule("ParticleIOS", &errorCode));
	checkErrorCode(&errorCode);
    PX_ASSERT(mParticleIosModule);
    if(mParticleIosModule)
    {
        NxParameterized::Interface* params = mParticleIosModule->getDefaultModuleDesc();
	    mParticleIosModule->init(*params);
    }

    mEmitterModule = static_cast<NxModuleEmitter*> ( gApexSDK->createModule("Emitter", &errorCode));
    checkErrorCode(&errorCode);
    PX_ASSERT(mEmitterModule);
    if(mEmitterModule)
    {
        NxParameterized::Interface* params = mEmitterModule->getDefaultModuleDesc();
	    mEmitterModule->init(*params);
        PxU32 numScalables = mEmitterModule->getNbParameters();
	    NxApexParameter** m_emitterModuleScalables = mEmitterModule->getParameters();
	    for (physx::PxU32 i = 0; i < numScalables; i++)
	    {
		    NxApexParameter& p = *m_emitterModuleScalables[i];
		    mEmitterModule->setIntValue(i, p.range.maximum);
	    }
    }

    mIofxModule = static_cast<NxModuleIofx*>(gApexSDK->createModule("IOFX", &errorCode));
    checkErrorCode(&errorCode);
	PX_ASSERT(mIofxModule);
	if (mIofxModule)
	{
		NxParameterized::Interface* params = mIofxModule->getDefaultModuleDesc();
		mIofxModule->init(*params);

		//m_apexIofxModule->disableCudaInterop();
		//m_apexIofxModule->disableCudaModifiers();
	}

}

void ApexParticles::CreateEmitter(NxApexSDK* gApexSDK, NxApexScene* gApexScene)
{
	NxApexAssetAuthoring* assetauthoring = gApexSDK->createAssetAuthoring(NX_APEX_EMITTER_AUTHORING_TYPE_NAME);
    //NxEmitterGeom sphereEmitter;
    //sphereEmitter->setRadius((PxF32)2.0);
    //sphereEmitter->setEmitterType(physx::apex::NxApexEmitterType::NX_ET_RATE);
    //
	NxParameterized::Interface* asParams = assetauthoring->releaseAndReturnNxParameterizedInterface();
	
	// Set the asset's type
	//NxParameterized::setParamEnum( *asParams, "geometryType", physx::apex::NxEmitterSphereGeom::getEmitterType() );

	NxApexEmitterAsset* emitterAsset = static_cast<NxApexEmitterAsset*> (gApexSDK->createAsset(asParams, "sphere_emitter"));
	
	NxParameterized::Interface* descParams = emitterAsset->getDefaultActorDesc();
	
	PX_ASSERT(descParams);
	if (!descParams)
	{
		return;
	}
	// Set Actor pose
	//NxParameterized::setParamMat44( *descParams, "initialPose", pose );
	NxApexEmitterActor* emitterActor;
	if(descParams->areParamsOK())
	{
		emitterActor = static_cast<NxApexEmitterActor*>(emitterAsset->createApexActor(*descParams,*gApexScene));
		if(emitterActor)
		{
			emitterActor->setCurrentPosition(PxVec3(0.0f, 5.0f, 0.0f));
			emitterActor->emitAssetParticles(true);
			emitterActor->startEmit( true );
		}
	}
}

bool ApexParticles::checkErrorCode(NxApexCreateError* err)
{
    bool retval = false;
    switch(*err)
    {
    case APEX_CE_NO_ERROR:
        retval =  true;
        break;
    case APEX_CE_NOT_FOUND:
        retval =  false;
        break;
    case APEX_CE_WRONG_VERSION:
        retval = false;
        break;
    case APEX_CE_DESCRIPTOR_INVALID:
        retval = false;
        break;
    }
    return retval;
}