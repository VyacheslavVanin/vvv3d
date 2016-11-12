#include "gluthal.h"
#include <vvv3d/core/graphics/lowlevel/openglprovider.h>

std::function<void(int x, int y)> glutLayer::resizeFunction;
std::function<void()> glutLayer::displayFunction;
std::function<void()> glutLayer::idleFunction;

glutLayer::glutLayer(int argc, char** argv, GLPROFILE p, int major, int minor)
{
}

void glutLayer::initContext(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInit( &argc, argv);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextVersion( 3, 3);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
}

void glutLayer::createWindow(int width, int height, const char* name)
{
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(name);
    glutIgnoreKeyRepeat(true);
    glewInit();
}

void glutLayer::setResizeFunction(const std::function<void (int, int)>& resizeFunction)
{
    glutLayer::resizeFunction = resizeFunction;
    glutReshapeFunc( [](int x,int y){glutLayer::resizeFunction(x,y);} );
}

void glutLayer::setDisplayFunction(const std::function<void ()>& displayFunction)
{
    glutLayer::displayFunction = displayFunction;
    glutDisplayFunc( [](){glutLayer::displayFunction();} );
}

void glutLayer::setIdleFunction(const std::function<void ()>& idleFunction)
{
    glutLayer::idleFunction = idleFunction;
    glutIdleFunc( [](void){glutLayer::idleFunction(); glutPostRedisplay();} );
}

void glutLayer::mainLoop()
{
    glutMainLoop();
}

void glutLayer::swap()
{
    glutSwapBuffers();
}
