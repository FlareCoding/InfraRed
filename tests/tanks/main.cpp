#include "Application.h"

int main()
{
	ApplicationStartupSettings StartupSettings;
	StartupSettings.WindowWidth = 1920;
	StartupSettings.WindowHeight = 1080;
	StartupSettings.Fullscreen = true;
	StartupSettings.WindowTitle = "Tanks";

	std::unique_ptr<App> app = std::make_unique<App>();
	app->SetApplicationStartupSettings(StartupSettings);
	app->Run();

	return 0;
}