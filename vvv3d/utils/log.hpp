#pragma once
#include <string>
#include <vvvstdhelper/containerhelper.hpp>
#include <vvvstdhelper/format.hpp>

namespace vvv3d {

namespace impl_details {
// TODO: Add logger with log levels
void log(const std::string& message);
void error(const std::string& message);
} // namespace impl_details

} // namespace vvv3d

#define LOG(message)                                                           \
    do {                                                                       \
        vvv3d::impl_details::log(message);                                     \
    } while (false)
#define LOG_ERROR(message)                                                     \
    do {                                                                       \
        vvv3d::impl_details::log(message);                                     \
    } while (false)
