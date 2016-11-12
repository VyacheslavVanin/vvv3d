#ifndef GLUTHAL_H
#define GLUTHAL_H
#include "hal.h"

class glutLayer : public HAL
{
public:
    glutLayer(int argc, char** argv,
              GLPROFILE p = GLPROFILE::CORE,
              int major = 3, int minor = 3);

    void initContext(int argc, char** argv) override;
    void createWindow(int width, int height, const char* name) override;
    void mainLoop() override;
    void swap() override;

    void setResizeFunction(const std::function<void(int x, int y)>&
                           resizeFunction) override;

    void setDisplayFunction(const std::function<void()>&
                            displayFunction) override;

    void setIdleFunction(const std::function<void()>& idleFunction) override;

    static std::function<void(int x, int y)> resizeFunction;
    static std::function<void()> displayFunction;
    static std::function<void()> idleFunction;

    ~glutLayer();
};

#endif // GLUTHAL_H
