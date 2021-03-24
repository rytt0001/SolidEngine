#pragma once

#include <PxPhysicsAPI.h>

namespace Solid
{
    class Physics
    {
    private:
        static physx::PxDefaultErrorCallback gDefaultErrorCallback;
        static physx::PxDefaultAllocator     gDefaultAllocatorCallback;

        static physx::PxCudaContextManager* gCudaContextManager;
    public:

        physx::PxFoundation* pxFoundation = nullptr;
        physx::PxPhysics*    pxPhysics    = nullptr;
        physx::PxScene*      pxScene        = nullptr;

        Physics();
        ~Physics();
    };
}