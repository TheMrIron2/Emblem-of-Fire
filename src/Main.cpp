#include <Platform/Platform.h>
#include <GFX/RenderCore.h>
#include <Platform/PC/Window.h>
#include <GFX/GFXWrapper.h>
#include <GFX/2D/TilemapAnim.h>
#include <GFX/UI/TextRenderer.h>
#include <Utilities/Input.h>

PSP_MODULE_INFO("Emblem of Fire", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
    Platform::initPlatform();

    Application* app = new Application();
    app->run();

    Graphics::g_RenderCore.Set2DMode();
    Graphics::g_RenderCore.SetClearColor(0, 0, 0, 255);

    while (app->isRunning()) {
        Graphics::g_RenderCore.BeginCommands();
        Graphics::g_RenderCore.Clear();

        app->update();
        app->draw();

        Platform::platformUpdate();
        Graphics::g_RenderCore.EndCommands();
    }

    delete app;
    Platform::exitPlatform();
    return 0;
}
