#include "engine.h"
#include <iostream>
#include <chrono>
#include <core/graphics/lowlevel/openglprovider.h>
#include <core/camera.h>
#include <core/resourcemanager.h>

static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;
Engine* Engine::activeEngine = nullptr;

Engine::Engine(int argc, char** argv, const char* windowName)
    : currentfps(0), viewportWidth(DEFAULT_SCREEN_WIDTH),
      viewportHeight(DEFAULT_SCREEN_HEIGHT),
      resourceManager(new ResourceManager())
{
    glewExperimental = GL_TRUE;
    glutInit( &argc, argv);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextVersion( 3, 3 );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize( viewportWidth, viewportHeight);
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( windowName );
    glutIgnoreKeyRepeat(true);
    glutIdleFunc( [](void){glutPostRedisplay();} );
    glutDisplayFunc( [](){activeEngine->display();} );
    glutReshapeFunc( [](int x,int y){activeEngine->resize(x,y); } );

    glewInit();
    glClearColor( 0.05f, 0.1f, 0.2f, 0);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE);

    activeEngine = this;
}

Engine::~Engine() {}

void Engine::run()
{
    initialSetup();
    glutMainLoop();
}

ResourceManager& Engine::getResourceManager() {return *resourceManager;}

void Engine::display()
{
    const auto t1 = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    onDraw();

    glutSwapBuffers();
    const auto t2 = std::chrono::system_clock::now();
    const auto dt = t2 -t1;
    const auto milis = (float)(std::chrono::duration_cast<std::chrono::microseconds>(dt).count())/1000;
    currentfps = 1000.0f / milis;
}

void Engine::initialSetup() {}
void Engine::onDraw() {}
void Engine::onResize(int x, int y) {(void)x, (void)y;}


int Engine::getVieportWidth() const {return viewportWidth;}
int Engine::getVieportHeight() const {return viewportHeight;}

Engine& Engine::getActiveEngine()
{
    return *activeEngine;
}

void Engine::resize(int x, int y)
{
    y = (y!=0) ? y : 1;
    viewportHeight = y;
    viewportWidth  = x;
    glViewport( 0, 0, viewportWidth, viewportHeight);
    onResize(viewportWidth, viewportHeight);
}

float Engine::getCurrentFps() const {return currentfps;}
