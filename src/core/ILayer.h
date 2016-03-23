#ifndef ILAYER_H
#define ILAYER_H

class ILayer
{
public:
    virtual void initialSetup() = 0;
    virtual void onDraw() = 0;
    virtual void onResize(int x, int y) = 0;
    virtual ~ILayer();
};

inline ILayer::~ILayer() {}

#endif // ILAYER_H
