#pragma once

#include <filesystem>

#include "Log.h"

#define MONK_EXPAND_MACRO(x) x
#define MONK_STRINGIFY_MACRO(x) #x

#define MONK_ASSERT_IMPL(condition, msg, ...) { if(!(condition)) { LOG_CRITICAL(msg, __VA_ARGS__); __debugbreak(); } }
#define MONK_ASSERT_WITH_MSG(condition, ...) MONK_ASSERT_IMPL(condition, "Assertion failed: {0}", __VA_ARGS__)
#define MONK_ASSERT_NO_MSG(condition) MONK_ASSERT_IMPL(condition, "Assertion '{0}' failed at {1}:{2}", MONK_STRINGIFY_MACRO(condition), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define MONK_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define MONK_ASSERT_GET_MACRO(...) MONK_EXPAND_MACRO(MONK_ASSERT_GET_MACRO_NAME(__VA_ARGS__, MONK_ASSERT_WITH_MSG, MONK_ASSERT_NO_MSG))

#define MONK_ASSERT(...) MONK_EXPAND_MACRO(MONK_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__))
