#include "Sandbox.h"

int main()
{
	ApplicationStartupSettings StartupSettings;
	StartupSettings.WindowWidth = 1270;
	StartupSettings.WindowHeight = 820;
	StartupSettings.Fullscreen = false;

	std::unique_ptr<SandboxApp> demo = std::make_unique<SandboxApp>();
	demo->SetApplicationStartupSettings(StartupSettings);
	demo->Run();

    return 0;
}
