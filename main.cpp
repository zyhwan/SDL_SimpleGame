#include <SDL3/SDL_main.h>
#include "Core/Application.h"
#include "Scene/TitleScene.h"

int SDL_main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    Application app;
    if (!app.Init()) return 1;

    app.SetScene(std::make_unique<TitleScene>());
    app.Run();
    return 0;
}