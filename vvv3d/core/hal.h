#ifndef HAL_H
#define HAL_H
#include <functional>

enum class GLPROFILE { ES, CORE };

class HAL
{
public:
    virtual void initContext(int argc, char** argv) = 0;
    virtual void createWindow(int width, int height, const char* name)=0;
    virtual void setResizeFunction(const std::function<void(int x, int y)>&
                                   resizeFunction) = 0;
    virtual void setDisplayFunction(const std::function<void()>&
                                    displayFunction) = 0;
    virtual void setIdleFunction(const std::function<void()>& idleFunction) = 0;
    virtual void mainLoop() = 0;
    virtual void swap() = 0;
};


#endif // HAL_H
