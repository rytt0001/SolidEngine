#pragma once

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/rigidBody.sld.hpp"

namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() RigidBody : public Components
    {
    public:

    	SLDField()
        bool enableGravity = true;
        SLDField()
    	bool isKinematic   = false;
        float mass = 1.f;
        float drag = 0;
        float angularDrag = 0.05;

        RigidBody() = default;
        ~RigidBody() = default;

        RigidBody_GENERATED
    };
} //!namespace

File_GENERATED
