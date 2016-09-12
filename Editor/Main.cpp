#define SDL_MAIN_HANDLED

#include "MainWindow.h"
#include "SDLApp.h"
#include "SDLWindow.h"
#include "fmt/format.h"
#include "imgui/imgui.h"

using namespace atlasEditor;


class Window1: public SDLWindow
{
public:
    Window1(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void onGUI() {
        ImGui::Text("Window1");
    }
};

class Window2: public SDLWindow
{
public:
    Window2(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void onGUI() {
        ImGui::Text("Window2");
        if (ImGui::Button("Test", ImVec2(50, 20)))
        {
            ImGui::Text("Clicked");
        }
    }
};

class Window3: public SDLWindow
{
public:
    Window3(const char *title,
            int x, int y, int w, int h) :
        SDLWindow(title, x, y, w, h) {}

    void onGUI() {
        ImGui::Text("Window3");
        if (ImGui::Button("Test", ImVec2(50, 20)))
        {
            ImGui::Text("Clicked");
        }
    }
};

int main(int argc, char **argv)
{
    SDLApp::get().init(SDL_INIT_VIDEO);

    new Window1(fmt::format("BGFX {}", 1).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);
    new Window2(fmt::format("BGFX {}", 2).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);
    new Window3(fmt::format("BGFX {}", 3).c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480);

    return SDLApp::get().exec();
}

