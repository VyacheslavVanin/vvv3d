#include "engine.h"
static const int DEFAULT_SCREEN_WIDTH = 640;
static const int DEFAULT_SCREEN_HEIGHT = 480;



std::shared_ptr<Engine> Engine::activeEngine = std::shared_ptr<Engine>();


Engine::Engine(int argc, char** argv, const char* windowName)
    : scene(new SceneBase()),
      viewportWidth(DEFAULT_SCREEN_WIDTH),
      viewportHeight(DEFAULT_SCREEN_HEIGHT)
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
    glewInit();
    glutIgnoreKeyRepeat(true);

    glClearColor( 0.05f, 0.1f, 0.2f, 0);
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_MULTISAMPLE);
    initialSetup();

    glutIgnoreKeyRepeat(true);

    glutIdleFunc( [](void){glutPostRedisplay();} );
    glutDisplayFunc( [](){activeEngine->display();} );
    glutReshapeFunc( [](int x,int y){activeEngine->resize(x,y); } );
    glutKeyboardFunc(   [](unsigned char c, int s, int d){activeEngine->keyboardFunc(c,s,d);}   );
    glutKeyboardUpFunc( [](unsigned char c, int s, int d){activeEngine->keyboardUpFunc(c,s,d);} );
    activeEngine = std::shared_ptr<Engine>(this);
}

Engine::~Engine()
{
    std::cout << "Engine basse destruction" << std::endl;
}

void Engine::run()
{
    scene->start();
    glutMainLoop();
}

void Engine::display()
{
    auto t1 = std::chrono::system_clock::now();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DRAW_TRANSPARENT
            scene->draw();
    scene->update();
    glutSwapBuffers();
    auto t2 = std::chrono::system_clock::now();
    auto dt = t2 -t1;
    auto milis = (float)(std::chrono::duration_cast<std::chrono::microseconds>(dt).count())/1000;
    currentfps = 1000.0f / milis;
}

void Engine::onResize(int y, int y)
{
    (void)y, (void)y;
}

void Engine::initialSetup() { }

void Engine::keyboardFunc(unsigned char c, int s, int d)
{
    std::cout <<"pressed "<< c << " " << s << " " << d << std::endl;
}

void Engine::keyboardUpFunc(unsigned char c, int s, int d)
{
    std::cout <<"released "<< c << " " << s << " " << d << std::endl;
}

void Engine::addLayer(const std::string& layerName, std::shared_ptr<GraphicLayerBase> layer)
{
    scene->addGraphicsLayer(layerName, layer);
}

std::shared_ptr<GraphicLayerBase> Engine::getLayer(const std::string& layerName) const
{
    return scene->getLayer(layerName);
}

void Engine::addObject(std::shared_ptr<SceneObject> object, const std::string& layerName)
{
    scene->addObject(object, layerName);
}

TextureManager&Engine::getTextureManager(){return textureManager;}

GeometryManager &Engine::getGeometryManager() {return geometryManager;}

FontManager &Engine::getFontManager() {return fontManager;}

ShaderManager &Engine::getShaderManager() {return shaderManger;}

std::shared_ptr<Engine> Engine::getActiveEngine() {return activeEngine;}

void Engine::resize(int x, int y)
{
    y = (y!=0) ? y : 1;
    viewportHeight = y;
    viewportWidth  = x;
    glViewport( 0, 0, viewportWidth, viewportHeight);
    scene->setScreenSize(x,y);
    onResize(viewportWidth, viewportHeight);
}

float Engine::getCurrentFps() const {return currentfps;}
