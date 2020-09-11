#include "Application.h"
#include <Windows.h>

int main()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	ApplicationStartupSettings StartupSettings;
	StartupSettings.WindowWidth = desktop.right;
	StartupSettings.WindowHeight = desktop.bottom;
	StartupSettings.WindowTitle = "Demo";
	StartupSettings.Fullscreen = true;

	std::unique_ptr<Demo> app = std::make_unique<Demo>();
	app->SetApplicationStartupSettings(StartupSettings);
	app->Run();

	return 0;
}
