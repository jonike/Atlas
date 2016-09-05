#ifndef SDLAPP_H
#define SDLAPP_H

#include "Base.h"

class SDLWindow;

class SDLApp
{
private:
    SDLApp() = default;
public:
    virtual ~SDLApp();
    static SDLApp& get() {
        static SDLApp _instance;
        return _instance;
    }

    int init(u32 flags);
    int addWindow(SDLWindow* window);
    int exec();
    void closeWindow(SDLWindow* window);
    void quit();

private:
    bool _running{true};
    std::list<std::unique_ptr<SDLWindow>> _windows;
};

#endif // SDLAPP_H
