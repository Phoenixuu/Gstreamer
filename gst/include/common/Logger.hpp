#ifndef Logger_hpp
#define Logger_hpp

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Must: define SPDLOG_ACTIVE_LEVEL before `#include "spdlog/spdlog.h"`
// #define SPDLOG_FMT_EXTERNAL

/**
 * @brief SPDLOG_LEVEL
 * 
 * @param SPDLOG_LEVEL_TRACE 0
 * @param SPDLOG_LEVEL_DEBUG 1
 * @param SPDLOG_LEVEL_INFO 2
 * @param SPDLOG_LEVEL_WARN 3
 * @param SPDLOG_LEVEL_ERROR 4
 * @param SPDLOG_LEVEL_CRITICAL 5
 * @param SPDLOG_LEVEL_OFF 6
 */

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// static std::shared_ptr<spdlog::logger> logger()
// {
//     static auto console_logger = spdlog::stdout_color_mt("console");
//     console_logger->set_level(spdlog::level::trace);
//     // console_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%s:%#] %v");
//     console_logger->set_pattern("[%H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
//     return spdlog::get("console");
// }

/**
 * @brief spdlog::level
 * 
 * @param spdlog::level::trace
 * @param spdlog::level::debug
 * @param spdlog::level::info
 * @param spdlog::level::warn
 * @param spdlog::level::err
 * @param spdlog::level::critical
 * @param spdlog::level::off
 */

inline void setup()
{
    auto console_logger = spdlog::stdout_color_mt("console");
    console_logger->set_level(spdlog::level::trace);
    console_logger->set_pattern("[%H:%M:%S.%e][%^%l%$][%s:%#] %v");
}

inline std::shared_ptr<spdlog::logger> logger()
{
    if(!spdlog::get("console"))
    {
        setup();
        return spdlog::get("console");
    }
    else
    {
        return spdlog::get("console");
    }
}

#define TRACE(...) SPDLOG_LOGGER_TRACE(logger(), __VA_ARGS__);
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(logger(), __VA_ARGS__);
#define INFO(...) SPDLOG_LOGGER_INFO(logger(), __VA_ARGS__);
#define WARN(...) SPDLOG_LOGGER_WARN(logger(), __VA_ARGS__);
#define ERROR(...) SPDLOG_LOGGER_ERROR(logger(), __VA_ARGS__);
#define CRITICAL(...) SPDLOG_LOGGER_CRITICAL(logger(), __VA_ARGS__);

#endif // Logger_hpp