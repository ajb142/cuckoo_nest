#pragma once
#include "../spdlog.h"

// Mock basic file sink for testing
namespace spdlog {
namespace sinks {
    using basic_file_sink_mt = spdlog::logger;
}
}
