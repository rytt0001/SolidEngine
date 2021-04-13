#pragma once

#include "Core/engine.hpp"
#include "GameCompiler/gameCompiler.hpp"

#include "ECS/Components/camera.hpp"

namespace Solid
{
    struct Vec2d;
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

    class Editor
    {
    private:
	    class SOLID_API Engine_Cleaner : public EngineCleanerInterface
	    {
	    public:
		    Engine_Cleaner()
		    {

		    }

		    ~Engine_Cleaner()
		    {

		    }
	    }Clean;

        InputManager<int>* editorInputManager = nullptr;
        Engine* engine = nullptr;
        GameCompiler* Compiler;


        void UpdateEditorCamera(Vec2d mouse_pos);
        bool MouseInSceneInterface(const Vec2d& mousePos);
    public:

        static float camSpeed;
        static Framebuffer sceneFramebuffer;
        static Camera editorCamera;

        Editor();
        ~Editor();
	    void LoadResources(bool _solid);
        void Run();
        friend class EngineCleanerInterface;
    };


} //!namespace


