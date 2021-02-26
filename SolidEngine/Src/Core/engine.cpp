#include "Core/engine.hpp"

#include <iostream>
#include "Rendering/OpenGL45/openGl45Renderer.hpp"
#include "UI/solidUI.hpp"
#include "Ressources/ressources.hpp"

#include "ECS/Components/transform.hpp"
#include "ECS/Components/meshRenderer.hpp"
#include "ECS/Components/camera.hpp"

namespace Solid
{
    Engine::Engine() :
            threadPool(&taskManager)
    {
        resourceManager = ResourceManager::Initialize(this);
        InitEcs();
    }

    Engine::~Engine()
    {

        UIContext::ReleaseSolidUI();
        delete window;
        threadPool.TerminateAllThreads();
    }

    void Engine::InitEcs()
    {
        ecsManager.Init();

        //Components Registration
        ecsManager.RegisterComponent<Transform>();
        ecsManager.RegisterComponent<MeshRenderer>();
        ecsManager.RegisterComponent<Camera>();

        //Register Signature
        rendererSystem = ecsManager.RegisterSystem<RendererSystem>();
        {
            Signature signature;
            signature.set(ecsManager.GetComponentType<Transform>());
            signature.set(ecsManager.GetComponentType<MeshRenderer>());
            ecsManager.SetSystemSignature<RendererSystem>(signature);
        }
    }

    void Engine::InitEngineContext(const WindowParams& _windowParams, const RendererParams& _rendererParams)
    {
        window   = new Window(_windowParams);
        switch (_rendererParams.rendererType)
        {
            case ERendererType::OpenGl45:
                renderer = OpenGL45Renderer::InitRenderer();
                break;
        }

        /// TEMPORARY
        UIContext::InitializeSolidUI(window->GetHandle());
        ///

        if(window != nullptr && renderer != nullptr)
            engineContextInit = true;
    }

    bool Engine::IsEngineContextInitialized() const
    {
        return engineContextInit;
    }
} //!namespace
