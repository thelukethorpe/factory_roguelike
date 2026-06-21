#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <cstdint>

using milliseconds_t = std::uint32_t;

static milliseconds_t now()
{
    const auto time_since_epoch = std::chrono::system_clock::now().time_since_epoch();
    const auto ms_since_epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count();
    return static_cast<milliseconds_t>(ms_since_epoch);
}

#endif
