#pragma once


#include "Core/Maths/Vector/vector3.hpp"
#include "Resources/resourceType.hpp"

#include "fmod.hpp"
#include <string>

#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "EngineGenerated/audioSource.sld.hpp"


namespace Solid SLDNamespace()
{
    class SOLID_API SLDClass() AudioSource : public Components
    {
    private:
        bool isInit = false;

        SLDField()
        String name = "None";

        FMOD::Channel* audioChannel = nullptr;
        AudioResource* audioResource = nullptr;

        SLDField()
        bool loop = false;
	    SLDField()
	    bool is3D = false;
	    SLDField()
	    bool isPlaying = false;
	    SLDField()
	    String ChannelGroup = "DEFAULT";
	    SLDField()
	    float pitch = 1.f;
	    SLDField()
        float volume = 1.f;
	    SLDField()
	    float minDistance = 0;
	    SLDField()
        float maxDistance = 500;
	    SLDField()
        Vec3 velocity = {0,0,0};

    public:
        //public func
        AudioSource() = default;
        ~AudioSource();
        SLDMethod()
        void Init() override;
	    SLDMethod()
	    void Release() override;
	    SLDMethod()
        void SetAudio(AudioResource* _audioResource);
	    SLDMethod()
        void SetLoop(bool _loop);
	    SLDMethod()
	    void SetIs3D(bool _3D);
	    SLDMethod()
	    void SetVolume(float _vol);
	    SLDMethod()
	    void SetPitch(float _pitch);
	    SLDMethod()
	    void SetMinMaxDistance(float _maxDistance, float _minDistance);
	    SLDMethod()
	    void SetMusicVelocity(const Vec3& _velocity);
	    SLDMethod()
	    void SetPosition(const Vec3& _position);

	    SLDMethod()
	    std::string GetName() const;
	    SLDMethod()
	    bool GetIs3D() const ;
	    SLDMethod()
	    float GetVolume() const;
	    SLDMethod()
	    float GetPitch() const;
	    SLDMethod()
	    float GetMaxDistance() const;
	    SLDMethod()
	    float GetMinDistance() const;
	    SLDMethod()
	    Vec3  GetMusicVelocity() const;

	    SLDMethod()
	    void Play();
	    SLDMethod()
	    void Pause();
	    SLDMethod()
	    void Stop();
	    SLDMethod()
	    bool IsPlaying();
	    SLDMethod()
	    bool IsPaused();
	    SLDMethod()
	    bool IsLooping();
		AudioSource_GENERATED
    };
} //!namespace

File_GENERATED