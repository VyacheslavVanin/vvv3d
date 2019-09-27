#include "log.hpp"
#include <vvv3d/core/engine.hpp>
#include <vvv3d/core/hal/hal.hpp>

namespace vvv3d {

namespace impl_details {
void log(const char* message)
{
    vvv3d::Engine::getActiveEngine().getHAL().log(message);
}

void log(const std::string& message) { log(message.c_str()); }

void error(const std::string& message) { log("error: " + message); }
} // namespace impl_details

} // namespace vvv3d
