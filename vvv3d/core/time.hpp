#pragma once
#include <chrono>

template <typename defaultTimeUnit = std::chrono::duration<double>,
          typename clock = std::chrono::system_clock>
class Time {
public:
    Time();
    template <typename T = defaultTimeUnit>
    typename T::rep sinceEpoch() const;

    template <typename T = defaultTimeUnit>
    typename T::rep sinceLastFrame() const;

    template <typename T = defaultTimeUnit>
    typename T::rep currentFrameTime() const;

    template <typename T = defaultTimeUnit>
    typename T::rep sinceStart() const;

    void updateFrameTime();

private:
    typename clock::duration lastFrameTime;
    typename clock::duration currFrameTime;
    typename clock::time_point startTime;
};

template <typename defaultTimeUnit, typename clock>
Time<defaultTimeUnit, clock>::Time()
    : lastFrameTime(clock::now().time_since_epoch()),
      currFrameTime(clock::now().time_since_epoch()), startTime(clock::now())
{
}

template <typename defaultTimeUnit, typename clock>
template <typename T>
typename T::rep Time<defaultTimeUnit, clock>::sinceEpoch() const
{
    using namespace std::chrono;
    const auto& now = clock::now().time_since_epoch();
    return duration_cast<T>(now).count();
}

template <typename defaultTimeUnit, typename clock>
template <typename T>
typename T::rep Time<defaultTimeUnit, clock>::sinceStart() const
{
    using namespace std::chrono;
    const auto& now = clock::now();
    return std::chrono::duration_cast<T>(now - startTime).count();
}

template <typename defaultTimeUnit, typename clock>
template <typename T>
typename T::rep Time<defaultTimeUnit, clock>::currentFrameTime() const
{
    return std::chrono::duration_cast<T>(currFrameTime).count();
}

template <typename defaultTimeUnit, typename clock>
template <typename T>
typename T::rep Time<defaultTimeUnit, clock>::sinceLastFrame() const
{
    return std::chrono::duration_cast<T>(currFrameTime - lastFrameTime).count();
}

template <typename defaultTimeUnit, typename clock>
void Time<defaultTimeUnit, clock>::updateFrameTime()
{
    lastFrameTime = currFrameTime;
    currFrameTime = clock::now().time_since_epoch();
}
