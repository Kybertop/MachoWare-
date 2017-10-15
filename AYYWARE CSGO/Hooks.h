#pragma once

extern bool DoUnload;
extern bool bGlovesNeedUpdate;

#include "Utilities.h"

namespace Hooks
{
	void Initialise();
	void UndoHooks();


	extern Utilities::Memory::VMTManager VMTPanel; 
	extern Utilities::Memory::VMTManager VMTClient; 
	extern Utilities::Memory::VMTManager VMTClientMode;
	extern Utilities::Memory::VMTManager VMTModelRender;
	extern Utilities::Memory::VMTManager VMTPrediction; 
	extern Utilities::Memory::VMTManager VMTPlaySound;
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTEventManager;
};

namespace Resolver
{
	extern bool didhitHS;
}

extern bool flipAA;