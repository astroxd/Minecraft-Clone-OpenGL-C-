#include "Engine.h"
#include "Log.h"





int main() {
	//Initialize Logger
	Log::Init();

	//Initialize Window instance
	Window::GetInstance();

	Engine engine = Engine();

	engine.run();

}

