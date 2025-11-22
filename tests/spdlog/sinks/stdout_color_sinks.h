#pragma once
#include "../spdlog.h"

// Mock stdout color sinks for testing
namespace spdlog {
namespace sinks {
    using stdout_color_sink_mt = spdlog::logger;
}
}
