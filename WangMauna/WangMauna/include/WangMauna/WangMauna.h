#pragma once


#define IMPL_LOGGER(name) wm::Logger __logger__(name)
#define IMPL_LOGGER_PATTERN(name, pattern) wm::Logger __logger__(name, pattern)

#define LOG_TRACE(msg, ...) __logger__.Trace(msg, __VA_ARGS__)
#define LOG_DEBUG(msg, ...) __logger__.Debug(msg, __VA_ARGS__)
#define LOG_INFO(msg, ...) __logger__.Info(msg, __VA_ARGS__)
#define LOG_WARNING(msg, ...) __logger__.Warning(msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...) __logger__.Error(msg, __VA_ARGS__)
#define LOG_CRITICAL(msg, ...) __logger__.Critical(msg, __VA_ARGS__)

#define DEFINE_LOG(name)\
class _##name##__Logger											\
{																\
	template <typename... Args>									\
	static void Trace(std::string_view fmt, Args... args);		\
																\
	template <typename... Args>									\
	static void Debug(std::string_view fmt, Args... args);		\
																\
	template <typename... Args>									\
	static void Info(std::string_view fmt, Args... args);		\
																\
	template <typename... Args>									\
	static void Warning(std::string_view fmt, Args... args);	\
																\
	template <typename... Args>									\
	static void Error(std::string_view fmt, Args... args);		\
																\
	template <typename... Args>									\
	static void Critical(std::string_view fmt, Args... args);	\
}; extern _##name##__Logger __logger__

#define IMPL_LOG(name)\
static													\
_##name##__Logger __logger__;							\

void _##name##__Logger::Trace()							\
{														\
														\
}														\
