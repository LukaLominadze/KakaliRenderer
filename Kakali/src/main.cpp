#include "core/Application.h"
#include "layers/SandboxLayer.h"

int main() {
	Application app;
	SandboxLayer sandboxLayer;
	if (app.StartUp("MyWindow", 1600, 900, false, true)) {
		app.PushLayer(&sandboxLayer);
		app.Run();
	}
	app.ShutDown();
}