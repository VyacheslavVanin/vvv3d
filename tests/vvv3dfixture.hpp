#pragma once
#include <gtest/gtest.h>
#include <memory>
#include <vvv3d/vvv3d.hpp>

class TestEngine : public vvv3d::Engine {
public:
    TestEngine(int argc, char** argv, const char* wname = "Vvv3d Hello World");
    void setDrawFunc(const std::function<void()>& func);
    // Engine interface
protected:
    void initialSetup() override;
    void onDraw() override;
    void onResize(int x, int y) override;
    int frame_counter = 0;
    std::function<void()> func;
};

class vvv3dFixture : public ::testing::Test {
public:
    vvv3dFixture();

    void SetUp() {}
    void TearDown() {}

    std::unique_ptr<TestEngine> e;
};
