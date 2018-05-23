#include "sdllayer.hpp"
#include <SDL.h>
#include <vvv3d/core/graphics/lowlevel/openglprovider.hpp>
//#include <SDL2/SDL_opengles2.hpp>

using namespace vvv3d;

class sdl_input {
public:
    sdl_input()
        : resizeFunction(), keys{0}, mousebuttons{0}, text(), x(0), y(0),
          xrel(0), yrel(0), exit(false)
    {
    }

    void pushQuitEvent()
    {
        SDL_Event e;
        e.type = SDL_QUIT;
        SDL_PushEvent(&e);
    }

    bool pollEvent()
    {
        SDL_Event e;
        if (!SDL_PollEvent(&e))
            return false;
        switch (e.type) {
        case SDL_WINDOWEVENT: onWindowEvent(e); break;
        case SDL_TEXTEDITING: onTextEdit(e); break;
        case SDL_TEXTINPUT: onTextInput(e); break;
        case SDL_QUIT: onQuit(e); break;
        case SDL_KEYDOWN: onKeyDown(e); break;
        case SDL_KEYUP: onKeyUp(e); break;
        case SDL_MOUSEBUTTONDOWN: onMouseButtonDown(e); break;
        case SDL_MOUSEBUTTONUP: onMouseButtonUp(e); break;
        case SDL_MOUSEMOTION: onMouseMove(e); break;
        }
        return true;
    }

    void pollEvents()
    {
        xrel = 0;
        yrel = 0;
        text.clear();
        events.clear();
        while (pollEvent())
            ;
    }

    void enableTextInput(bool enable = true)
    {
        if (enable)
            SDL_StartTextInput();
        else
            disableTextInput();
    }
    void disableTextInput() { SDL_StopTextInput(); }

    bool keyDown(uint16_t scancode) const
    {
        if (scancode >= SDL_NUM_SCANCODES)
            return false;
        return keys[scancode];
    }

    bool buttonDown(uint16_t button) const
    {
        if (button >= 32)
            return false;
        return mousebuttons[button];
    }

    int getMouseX() const { return x; }
    int getMouseY() const { return y; }
    int getMouseRelX() const { return xrel; }
    int getMouseRelY() const { return yrel; }
    bool quit() const { return exit; }

    bool hasText() const { return !text.empty(); }
    const std::string& getText() const { return text; }
    const std::vector<InputEvent>& getEvents() const { return events; }

    std::function<void(int x, int y)> resizeFunction;

private:
    char keys[SDL_NUM_SCANCODES];
    char mousebuttons[32];
    std::string text;
    std::vector<InputEvent> events;
    int x, y;
    int xrel, yrel;
    bool exit;

    void onKeyDown(const SDL_Event& e)
    {
        const auto scancode = e.key.keysym.scancode;
        keys[scancode] = 1;
        events.emplace_back(INPUT_EVENT_TYPE::KEY_DOWN, scancode);
    }

    void onKeyUp(const SDL_Event& e)
    {
        const auto scancode = e.key.keysym.scancode;
        keys[scancode] = 0;
        events.emplace_back(INPUT_EVENT_TYPE::KEY_UP, scancode);
    }

    void onMouseButtonDown(const SDL_Event& e)
    {
        mousebuttons[e.button.button] = 1;
    }
    void onMouseButtonUp(const SDL_Event& e)
    {
        mousebuttons[e.button.button] = 0;
    }

    void onMouseMove(const SDL_Event& e)
    {
        x = e.motion.x;
        y = e.motion.y;
        xrel += e.motion.xrel;
        yrel += e.motion.yrel;
    }

    void onTextInput(const SDL_Event& e) { text += e.text.text; }

    void onTextEdit(const SDL_Event&) { printf("Text edit: \n"); }

    void onWindowEvent(const SDL_Event& e)
    {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
            resizeFunction(e.window.data1, e.window.data2);
            break;
        default: break;
        }
    }

    void onQuit(const SDL_Event&) { exit = 1; }
};

class sdlLayer::SDLWraper {
public:
    SDLWraper(GLPROFILE profile, int major_ver, int minor_ver)
        : eventLoop(), mainwindow(), maincontext(), displayFunction(),
          idleFunction(), profile(profile), major_ver(major_ver),
          minor_ver(minor_ver)
    {
        switch(profile) {
            case GLPROFILE::CORE: printf("Use gl core profile\n"); break;
            case GLPROFILE::ES: printf("Use gl es profile\n"); break;
        }
    }

    SDLWraper(const SDLWraper&) = delete;
    SDLWraper& operator=(const SDLWraper&) = delete;

    void swap() { SDL_GL_SwapWindow(mainwindow); }

    ~SDLWraper()
    {
        SDL_GL_DeleteContext(maincontext);
        SDL_DestroyWindow(mainwindow);
        SDL_Quit();
    }

    void initContext()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            throw std::logic_error("SDL initialization failed...");

        int profile = SDL_GL_CONTEXT_PROFILE_CORE;
        GLPROFILE p = this->profile;
        const int major = major_ver;
        const int minor = minor_ver;
        switch (p) {
        case GLPROFILE::ES: profile = SDL_GL_CONTEXT_PROFILE_ES; break;
        case GLPROFILE::CORE: profile = SDL_GL_CONTEXT_PROFILE_CORE; break;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    }

    void createWindow(const char* name, int width, int height)
    {
        mainwindow = SDL_CreateWindow(
            name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!mainwindow)
            throw std::logic_error("Window creation failed");

        maincontext = SDL_GL_CreateContext(mainwindow);

        SDL_GL_SetSwapInterval(1);
#ifdef __GLEW_H__
        glewInit();
#endif
    }

    void run()
    {
        while (!eventLoop.quit()) {
            eventLoop.pollEvents();
            idleFunction();
            displayFunction();
        }
    }

    void stop() { eventLoop.pushQuitEvent(); }
    sdl_input eventLoop;
    SDL_Window* mainwindow;    /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    std::function<void()> displayFunction;
    std::function<void()> idleFunction;
    GLPROFILE profile;
    int major_ver;
    int minor_ver;
};

sdlLayer::sdlLayer(int argc, char** argv, GLPROFILE p, int major, int minor)
    : sdl(new SDLWraper(p, major, minor))
{
    (void)argc;
    (void)argv;
}

void sdlLayer::initContext(int argc, char** argv)
{
    sdl->initContext();
    (void)argc;
    (void)argv;
}

void sdlLayer::createWindow(int width, int height, const char* name)
{
    sdl->createWindow(name, width, height);
}

void sdlLayer::mainLoop() { sdl->run(); }
void sdlLayer::stopMainLoop() { sdl->stop(); }

void sdlLayer::swap() { sdl->swap(); }

void sdlLayer::setResizeFunction(
    const std::function<void(int, int)>& resizeFunction)
{
    sdl->eventLoop.resizeFunction = resizeFunction;
}

void sdlLayer::setDisplayFunction(const std::function<void()>& displayFunction)
{
    sdl->displayFunction = displayFunction;
}

void sdlLayer::setIdleFunction(const std::function<void()>& idleFunction)
{
    sdl->idleFunction = idleFunction;
}

size_t sdlLayer::getNumScanCodes() const { return SDL_NUM_SCANCODES; }

sdlLayer::~sdlLayer() = default;

bool sdlLayer::keyDown(uint16_t scancode) const
{
    return sdl->eventLoop.keyDown(scancode);
}

bool sdlLayer::mouseButtonDown(uint16_t button) const
{
    return sdl->eventLoop.buttonDown(button);
}

int sdlLayer::getMouseX() const { return sdl->eventLoop.getMouseX(); }

int sdlLayer::getMouseY() const { return sdl->eventLoop.getMouseY(); }

int sdlLayer::getMouseRelX() const { return sdl->eventLoop.getMouseRelX(); }

int sdlLayer::getMouseRelY() const { return sdl->eventLoop.getMouseRelY(); }

bool sdlLayer::hasText() const { return sdl->eventLoop.hasText(); }

const std::string& sdlLayer::getText() const
{
    return sdl->eventLoop.getText();
}

const std::vector<InputEvent>& sdlLayer::getEvents() const
{
    return sdl->eventLoop.getEvents();
}

void sdlLayer::setVSync(bool vsync) { SDL_GL_SetSwapInterval(vsync); }
