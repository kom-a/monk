#pragma once

#include "../../src/Logger.h"
#include "../../src/Pattern.h"

#define DEFINE_LOGGER() extern wm::Logger __logger__
#define IMPL_LOGGER(name) wm::Logger __logger__(name)
#define IMPL_LOGGER_PATTERN(name, pattern) wm::Logger __logger__(name, pattern)

#define LOG_TRACE(msg, ...) ::__logger__.Trace(msg, __VA_ARGS__)
#define LOG_DEBUG(msg, ...) ::__logger__.Debug(msg, __VA_ARGS__)
#define LOG_INFO(msg, ...) ::__logger__.Info(msg, __VA_ARGS__)
#define LOG_WARNING(msg, ...) ::__logger__.Warning(msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...) ::__logger__.Error(msg, __VA_ARGS__)
#define LOG_CRITICAL(msg, ...) ::__logger__.Critical(msg, __VA_ARGS__)