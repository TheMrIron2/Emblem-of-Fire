#include "Application.h"

Application::Application()
{

}

Application::~Application()
{
}

void Application::run()
{
    gsm = new GameStateManager();
}

void Application::update()
{
    gsm->update();
}

void Application::draw()
{
    gsm->draw();
}
