#include "ECS/Components/transform.hpp"

namespace Solid
{
    Transform::Transform():
    transMat(Mat4<>::Identity)
    {}

    Transform::Transform(const Vec3 &_position, const Quat &_rotation, const Vec3 &_scale):
    transMat(Mat4<>::Identity),
    position(_position),
    rotation(_rotation),
    scale(_scale)
    {}

    void Transform::SetPosition(const Vec3 &_vec)
    {
        position = _vec;
        hasToUpdateMat = true;
    }

    void Transform::SetRotation(const Quat &_quat)
    {
        rotation = _quat;
        euler = _quat.ToEuler();
        euler.x=Maths::RadToDeg(euler.x);
        euler.y=Maths::RadToDeg(euler.y);
        euler.z=Maths::RadToDeg(euler.z);

        hasToUpdateMat = true;
    }

    void Transform::SetEuler(const Vec3& _euler)
    {
        euler=_euler;
        Vec3 temp {Maths::DegToRad(_euler.x),Maths::DegToRad(_euler.y),Maths::DegToRad(_euler.z)};
        rotation = Quat(temp);
    }

    void Transform::SetScale(const Vec3 &_vec)
    {
        scale = _vec;
        hasToUpdateMat = true;
    }

    void Transform::Translate(const Vec3 &_vec)
    {
        position += _vec;
        hasToUpdateMat = true;
    }

    void Transform::Rotate(const Quat &_quat)
    {
        rotation.Rotate(_quat);

        hasToUpdateMat = true;
    }

    void Transform::Scale(const Vec3 &_vec)
    {
        scale += _vec;
        hasToUpdateMat = true;
    }

    Vec3 Transform::GetPosition() const
    {
        return position;
    }

    Quat Transform::GetRotation() const
    {
        return rotation;
    }

    Vec3 Transform::GetScale() const
    {
        return scale;
    }



    Vec3 Transform::GetEuler() const
    {
        return euler;
    }

    Mat4<float> Transform::GetMatrix()
    {
        if(hasToUpdateMat)
        {
            UpdateTransformMatrix();
            hasToUpdateMat = false;
        }

        return transMat;
    }

    void Transform::UpdateTransformMatrix()
    {
        transMat = Mat4<float>::Transform(position,rotation,scale);
    }

} //!namespace