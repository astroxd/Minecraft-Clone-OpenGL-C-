#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init() {
	spdlog::set_pattern("%^[%T] %s:%#: %v%$");
	s_Logger = spdlog::stdout_color_mt("APP");
	s_Logger->set_level(spdlog::level::trace);
}