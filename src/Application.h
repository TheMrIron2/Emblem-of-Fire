#pragma once

class Application {
public:
    Application();
    ~Application();

    void run();

    void update();
    void draw();

    inline bool isRunning() {
        return gsm->isRunning();
    }

private:
    GameStateManager* gsm;
};

