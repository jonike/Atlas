#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "Base.h"
#include <SDL2/SDL.h>

struct ImDrawData;

class SDLWindow
{
    friend class SDLApp;
public:
    SDLWindow(const char *title,
              int x, int y, int w,
              int h, u32 flags);
    virtual ~SDLWindow();
    virtual void handleEvent(SDL_Event& e);
    virtual void update(float dt);

protected:
    // imgui related
    bool imguiInit();
    void imguiShutdown();
    void imguiNewFrame();

    // bgfx related
    bool bgfxInit();

private:
    void doUpdate(float dt);

private:
    SDL_Window* _window{nullptr};
    u32 _windowId{0};
    SDL_GLContext _glContext;

    u32 _width{0};
    u32 _height{0};
    u32 _debug{0};
    u32 _reset{0};
};

void imguiRenderDrawLists(ImDrawData* drawData);

#endif // SDLWINDOW_H
