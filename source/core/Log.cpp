#if PSET_DEBUG
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "Log.h"
static std::shared_ptr<spdlog::logger> psetLogger;
std::shared_ptr<spdlog::logger>& GetLogger()
{
	if (psetLogger.get() == nullptr)
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("pset.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		psetLogger = std::make_shared<spdlog::logger>("PSET", begin(logSinks), end(logSinks));
		spdlog::register_logger(psetLogger);
		psetLogger->set_level(spdlog::level::trace);
		psetLogger->flush_on(spdlog::level::trace);
	}
	return psetLogger;
}
#endif