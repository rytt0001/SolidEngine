#pragma once
#include <string>
#include "Build/solidAPI.hpp"
#include <Core/Maths/Utils/constants.hpp>

#include "EngineGenerated/vector2.sld.hpp"
#include <iostream>
#include <string>

namespace Solid SLDNamespace()
{
    class Vec3;

    class Vec4;

    struct SOLID_API SLDStruct() Vec2i
    {
    	SLDField()
        int x;
	    SLDField()
        int y;
        Vec2i_GENERATED
    };
    struct SOLID_API SLDStruct() Vec2d
    {
	    SLDField()
        double x;
	    SLDField()
        double y;
	    Vec2d_GENERATED
    };
    class SOLID_API SLDClass() Vec2
    {
    public:

	    SLDField()
	    float x = 0;
	    SLDField()
        float y = 0;

	    SLDMethod()
        Vec2() noexcept = default;

	    SLDMethod()
        Vec2(const float &_value) noexcept;

	    SLDMethod()
        Vec2(float _x, float _y) noexcept;

	    SLDMethod()
        Vec2(const Vec2 &_copy) noexcept;

	    SLDMethod()
        Vec2(const Vec3 &_copy) noexcept;

	    SLDMethod()
        Vec2(const Vec4 &_copy) noexcept;

        ~Vec2() = default;

#pragma region Static Methods

        static const Vec2 Zero;
        static const Vec2 Up;
        static const Vec2 Down;
        static const Vec2 Left;
        static const Vec2 Right;

		SLDMethod()
        static float Dot(const Vec2 &_v1,const Vec2 &_v2) noexcept;

		SLDMethod()
        static float Cross(const Vec2 &_v1,const Vec2 &_v2) noexcept;

		SLDMethod()
        static Vec2 Lerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

		SLDMethod()
        static Vec2 Nlerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

		SLDMethod()
        static Vec2 Slerp(const Vec2& _v1,const Vec2& _v2, float _r) noexcept;

#pragma endregion
#pragma region Methods

		SLDMethod()
        float SqrtLength() const noexcept;

		SLDMethod()
        float Length() const noexcept;

		SLDMethod()
        Vec2 &Scale(float _scale) noexcept;

		SLDMethod()
        Vec2 GetScaled(float _scale) const noexcept;

		SLDMethod()
        Vec2 &Unscale(float _scale) noexcept;

		SLDMethod()
        Vec2 GetUnscaled(float _scale) const noexcept;

		SLDMethod()
        Vec2 &Normalize() noexcept;

		SLDMethod()
        Vec2 GetNormalized() const noexcept;

		SLDMethod()
        bool IsNormalized() const noexcept;

		SLDMethod()
        bool IsEquals(const Vec2& vec) const noexcept;

		SLDMethod()
        float Dist(const Vec2& vec) const noexcept;

		SLDMethod()
        float SqrtDist(const Vec2& vec) const noexcept;

		SLDMethod()
        const std::string ToString() const noexcept;

#pragma endregion
#pragma region Operator

        Vec2 &operator = (const Vec2& vec) noexcept = default;

        bool operator == (const Vec2 &_vec) const noexcept;

        bool operator != (const Vec2 &_vec) const noexcept;

        Vec2 operator + (const Vec2 &_vec) const noexcept;

        Vec2 operator - (const Vec2 &_vec) const noexcept;

        Vec2 operator * (const Vec2 &_vec) const noexcept;

        Vec2 operator / (const Vec2 &_vec) const noexcept;

        Vec2 &operator += (const Vec2 &_vec) noexcept;

        Vec2 &operator -= (const Vec2 &_vec) noexcept;

        Vec2 &operator *= (const Vec2 &_vec) noexcept;

        Vec2 &operator /= (const Vec2 &_vec) noexcept;

        Vec2 operator + (float _value) const noexcept;

        Vec2 operator - (float _value) const noexcept;

        Vec2 operator * (float _value) const noexcept;

        Vec2 operator / (float _value) const noexcept;

        Vec2 &operator += (float _value) noexcept;

        Vec2 &operator -= (float _value) noexcept;

        Vec2 &operator *= (float _value) noexcept;

        Vec2 &operator /= (float _value) noexcept;

        Vec2 operator -() noexcept;

#pragma endregion
		Vec2_GENERATED
    };
    Vec2 operator * (float _value, const Vec2& vec) noexcept;
    Vec2 operator / (float _value, const Vec2& vec) ;

};

File_GENERATED
