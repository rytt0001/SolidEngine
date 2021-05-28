#include "ECS/types.hpp"
#include "Build/solidAPI.hpp"
#include "ECS/Components/script.hpp"
#include "ECS/Components/rigidBody.hpp"
#include "EngineGenerated/EntityMacros.h"
#include "EngineGenerated/PlayerController.sld.hpp"
namespace Solid SLDNamespace()
{
    class Engine;
	class SOLID_SCRIPT_API SLDClass() PlayerController : public Script
	{
	public:
        SLDField()
        float moveSpeed = 0;
        SLDField()
        float jumpForce = 0;
        SLDField()
        int health = 0;
	private:
		Engine* engine = nullptr;
	    RigidBody* rigidBody = nullptr;

	public:
		PlayerController();
		~PlayerController() override = default;

		void Init() override;

		void Update() override;

	private:

	    void MoveForward();
	    void MoveBack();
	    void MoveLeft();
	    void MoveRight();
	    void Jump();

	    void Fire();

		PlayerController_GENERATED
	};
	
	
	
}

File_GENERATED
