#pragma once
#if PSET_DEBUG
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
std::shared_ptr<spdlog::logger>& GetLogger();

#define PSET_LOG_TRACE(...)    GetLogger()->trace(__VA_ARGS__)
#define PSET_LOG_INFO(...)     GetLogger()->info(__VA_ARGS__)
#define PSET_LOG_WARN(...)     GetLogger()->warn(__VA_ARGS__)
#define PSET_LOG_ERROR(...)    GetLogger()->error(__VA_ARGS__)
#define PSET_LOG_CRITICAL(...) GetLogger()->critical(__VA_ARGS__)
#define PSET_LOG_UNIMPLEMENTED(...)     GetLogger()->info(__VA_ARGS__)

#define PSET_EXIT_AND_LOG_IF(expr,log)	\
{										\
	if (expr)							\
	{									\
		PSET_LOG_ERROR(log);			\
		exit(-1);						\
	}									\
}										\

#define PSET_EXIT_IF(expr)				\
{										\
	if (expr)							\
	{									\
		exit(-1);						\
	}									\
}										\

#define PSET_EXIT_AND_LOG(...)			\
{										\
	GetLogger()->error(__VA_ARGS__);	\
		exit(-1);						\
}										\

#else

#define PSET_LOG_TRACE(...)    
#define PSET_LOG_INFO(...)     
#define PSET_LOG_WARN(...)     
#define PSET_LOG_ERROR(...)    
#define PSET_LOG_CRITICAL(...) 

#define PSET_EXIT_IF(expr)
#define PSET_EXIT_AND_LOG_IF(expr,log)
#define PSET_EXIT_AND_LOG(...)	

#endif