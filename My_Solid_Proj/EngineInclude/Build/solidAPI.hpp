#pragma once

#ifndef SOLID_API
    #define SOLID_API //__declspec(dllexport)
#endif

#ifndef SOLID_SCRIPT_API
	#define SOLID_SCRIPT_API
#endif

#ifndef SOLID_COMPILED_STATE
#define SOLID_COMPILED_STATE "NONE"
#endif
#ifndef SOLID_EDITOR_API
#define SOLID_EDITOR_API //__declspec(dllexport)
#endif
using uint = unsigned int ;
#include "EngineGenerated/EntityMacros.h"
#include "Core/std_wrapper.hpp"