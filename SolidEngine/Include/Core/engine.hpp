#pragma once
#include "Build/solidAPI.hpp"
#include "Rendering/renderer.hpp"
#include "Window/window.hpp"
#include "Audio/audio.hpp"
#include "solidMultiThread.hpp"
#include "Time/time.hpp"
#include "InputManager/inputManager.hpp"
#include "Core/Debug/debug.hpp"
#include "ECS/ecsManager.hpp"

#include "ECS/System/rendererSystem.hpp"
#include "ECS/System/audioSystem.hpp"
#include "ECS/System/physicsSystem.hpp"
#include "ECS/System/scriptSystem.hpp"
#include "ECS/System/particleEffectSystem.hpp"
#include "ECS/System/transformSystem.hpp"
#include "ECS/System/cameraSystem.hpp"
#include "ECS/System/animationSystem.hpp"

#include "Resources/graphicalResourceMgr.hpp"

#include "Physics/physics.hpp"

#include "std_wrapper.hpp"
#include "GameCompiler/gameCompiler.hpp"
#include "systemCmd.hpp"
#undef ERROR

namespace Solid
{
    /**
     * @brief Base engine class. Running the lifetime of application
     */
    class SOLID_API Engine
    {
    private:

        Log log;

        bool mtEnabled = false;
        bool engineContextInit = false;


        void InitEcs();
	    Engine();

	    static Engine* instance;

	    ~Engine();
	    static void DeleteInstance()
	    {
	    	delete instance;
	    };
	    friend class EngineCleanerInterface;
	    std::vector<std::function<void(Resource*)>> LoadedSceneCallbacks;

	    template<class T>
	    void AddComp(const std::string &className, std::vector<char> &buffer, std::uint64_t &readPos,
	                 GameObject *go, Components *cmp, std::size_t FieldNum, std::size_t cmpNameSize);

	    void AddAllComps(GameObject *elt, std::vector<char> &buffer, uint64_t &ReadPos);
    public:
    	//Engine** test = &instance;
        Window* window;
        Renderer* renderer = nullptr;
	    GameCompiler* Compiler;
        Audio audio;
        ECSManager ecsManager;
        std::shared_ptr<RendererSystem> rendererSystem;
        std::shared_ptr<AudioSystem> audioSystem;
	    std::shared_ptr<PhysicsSystem> physicsSystem;
	    std::shared_ptr<ScriptSystem> scriptSystem;
	    std::shared_ptr<ParticleEffectSystem> particleEffectSystem;
	    std::shared_ptr<TransformSystem> transformSystem;
	    std::shared_ptr<CameraSystem> cameraSystem;
	    std::shared_ptr<AnimationSystem> animSystem;
        ResourceManager resourceManager;
        TaskManager taskManager;
        ThreadManager threadPool;
        GraphicalResourceMgr graphicsResourceMgr;
        Physics physics;

        Camera* activeCamera = nullptr;
        Framebuffer PlayBuffer;

        InputManager* inputManager = nullptr;

        static Engine* GetInstance();
        bool MultiThreadEnabled()const {return mtEnabled;}
        void EnableMultiThread(bool _b);
        Engine(const Engine& _copy) = delete;
        Engine(Engine&& _move) = delete;

        /**
         * @brief Init GLFW window context and Renderer (Minimum required)
         */
        void InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams);

        /**
         * @brief Return if the engine context is initialized
         * @return Engine context init
         */
        bool IsEngineContextInitialized() const;

	    void LoadScene(const char *name);

	    void SaveScene(const fs::path& p);

	    void InitScript();

	    void DestroyScript();
	    SceneResource* SaveTempScene();

	    void LoadTempScene(SceneResource* scene);


        void Update();

        void FixedUpdate();

	    void LateUpdate();


	    void ForceUpdate();

	    void SetActiveCamera(Camera* _newCam);

	    void RenderToBuffer(const Vec2i& _size);

	    void AddLoadedSceneCallback(const std::function<void(Resource*)>& _func);

    };

	class SOLID_API EngineCleanerInterface
	{
	protected:
		EngineCleanerInterface()
		{

		}

		~EngineCleanerInterface()
		{
			Engine::DeleteInstance();
		}

	};


} //!namespace
