#include "core/Application.h"

int main() {
	Application app;
	if (app.StartUp("MyWindow", 1600, 900, false, true)) {
		app.Run();
	}
	app.ShutDown();
}