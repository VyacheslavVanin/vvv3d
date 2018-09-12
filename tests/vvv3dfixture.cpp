#include "vvv3dfixture.hpp"

namespace {
char name[] = "vvv3d";
char* argv[] = {name};
} // namespace
vvv3dFixture::vvv3dFixture() : e(std::make_unique<TestEngine>(1, argv)) {}

TestEngine::TestEngine(int argc, char** argv, const char* wname)
    : vvv3d::Engine(argc, argv, wname)
{
}
// Override Engine interface
void TestEngine::initialSetup() { frame_counter = 0; }
void TestEngine::onDraw()
{
    if (++frame_counter > 1)
        stop();
}
void TestEngine::onResize(int x, int y) {}
