#include "SDLWindow.h"
#include <SDL2/SDL.h>
#include "SDLApp.h"
#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <SDL2/SDL_syswm.h>
#include <ImGUI/imgui.h>
#include <fmt/printf.h>

bool SDLWindow::_initialized = false;
//SDL_GLContext SDLWindow::_glContext = 0;
u32 SDLWindow::_debug  = BGFX_DEBUG_TEXT;
u32 SDLWindow::_reset  = BGFX_RESET_VSYNC;
u8 SDLWindow::_windowCount = 0;

SDLWindow::SDLWindow(const char *title, int x, int y, int w, int h, u32 flags)
{
    _width = w;
    _height = h;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    _window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (_window == nullptr)
    {
        fmt::print("SLD Window creation failed, err: {}\n", SDL_GetError());
        return;
    }
    _glContext = SDL_GL_CreateContext(_window);

    _windowId = SDL_GetWindowID(_window);

    if (!_initialized)
    {
        if (!bgfxInit()) fmt::print("Failed to initialize bgfx\n");
        if (!imguiInit()) fmt::print("Failed to initialize imgui\n");
        _initialized = true;
        _isDefault = true;
        const bgfx::Caps* caps = bgfx::getCaps();
        bool swapChainSupported = 0 != (caps->supported & BGFX_CAPS_SWAP_CHAIN);
        fmt::print("Swapchain supported: {}\n", swapChainSupported);
    }

    if (!_isDefault)
    {
        _framebuffer = bgfx::createFrameBuffer(nativeHandle(), _width, _height);
    }
    else
    {
        _framebuffer = BGFX_INVALID_HANDLE;
    }

    _viewId = _windowCount++;
    bgfx::setViewName(_viewId, title);
    bgfx::setViewFrameBuffer(_viewId, _framebuffer);
    bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height) );
    bgfx::setViewClear(_viewId
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x303030ff
                       , 1.0f
                       , 0
                       );


    SDLApp::get().addWindow(this);
}

SDLWindow::~SDLWindow()
{
    if (bgfx::isValid(_framebuffer))
        bgfx::destroyFrameBuffer(_framebuffer);

    if (_isDefault)
    {
        imguiShutdown();
        bgfx::shutdown();
    }

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
}

void SDLWindow::handleEvent(SDL_Event &e)
{
    //If an event was detected for this window
    if( e.type == SDL_WINDOWEVENT && e.window.windowID == _windowId )
    {
        switch(e.window.event)
        {
        //Get new dimensions and recreate framebuffer
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            if (_width != e.window.data1 ||
                    _height != e.window.data2)
            {
            _width = e.window.data1;
            _height = e.window.data2;

            if (bgfx::isValid(_framebuffer))
            {
                bgfx::destroyFrameBuffer(_framebuffer);
                bgfx::createFrameBuffer(nativeHandle(), _width, _height);
            }

            bgfx::reset(_width, _height);
            bgfx::setViewRect(_viewId, 0, 0, uint16_t(_width), uint16_t(_height) );
            bgfx::setViewFrameBuffer(_viewId, _framebuffer);
            }
            break;
        }
    }
}

void SDLWindow::update(float dt)
{
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 1, 0x4f, fmt::format("Bgfx window {}", _viewId).c_str());
}

void SDLWindow::doUpdate(float dt)
{
    bgfx::touch(_viewId);
    update(dt);
}

bool SDLWindow::imguiInit()
{
    ImGuiIO& io = ImGui::GetIO();

#if BX_PLATFORM_WINDOWS
    io.ImeWindowHandle =  nativeHandle();
#endif

    io.RenderDrawListsFn = &imguiRenderDrawLists;

    return true;
}

void SDLWindow::imguiShutdown()
{

}

void SDLWindow::imguiNewFrame()
{

}

bool SDLWindow::bgfxInit()
{
    bgfx::PlatformData pd;
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
        return false;

#if BX_PLATFORM_WINDOWS
    pd.ndt          = NULL;
    pd.nwh          = wmi.info.win.window;
    pd.context      = _glContext;
#endif
    setPlatformData(pd);

    if (!bgfx::init(bgfx::RendererType::OpenGL))
        return false;

    bgfx::reset(_width, _height, _reset);

    // Enable debug text.
    bgfx::setDebug(_debug);

    return true;
}

void *SDLWindow::nativeHandle()
{
    // get native  window handle
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
        return nullptr;

#if BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#endif

    return nullptr;
}

void imguiRenderDrawLists(ImDrawData *drawData)
{
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    drawData->ScaleClipRects(io.DisplayFramebufferScale);
}


