#pragma once
#include <vvvstdhelper/format.hpp>
#include <string>

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
