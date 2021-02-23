#pragma once

#include "Types/extendedTypes.hpp"
#include "Core/Maths/Matrix/matrix4.hpp"
#include "Core/Maths/Quaternion/quaternion.hpp"
#include "transform.hpp"

namespace Solid
{
    class Camera
    {
    private:
        Mat4<float> projection;
        Mat4<float> view;

    public:
        Transform   transform;
        float fov;
        float near;
        float far;

        Camera();

        Mat4<float> GetView();
        Mat4<float> GetProjection();

        void UpdateFreeFly(const Int2 _spaceScreen);
    };
} //!namespace
