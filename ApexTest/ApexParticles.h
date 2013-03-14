//***************************************************************************************
// ApexParticles.h
//
//
// 
//
//***************************************************************************************
#ifndef APEX_PARTICLES_H
#define APEX_PARTICLES_H

//#include "apex.h"
#include "NxApex.h"
#include "NxApexSDK.h"
#include "NxModuleIofx.h"
#include "NxModuleEmitter.h"
#include "NxModuleParticleIos.h"
#include "NxParamUtils.h"

#include <vector>

using namespace std;
using namespace physx;

class ApexParticles
{
public:
    ApexParticles();
    ~ApexParticles();

    void Init(NxApexSDK* gApexSDK);

private:
    NxModuleParticleIos*        mParticleIosModule;
    NxModuleEmitter*            mEmitterModule;
    NxModuleIofx*               mIofxModule;

    bool checkErrorCode(NxApexCreateError* err);
};

#endif