//
// Created by zhangyongchao on 2020/2/11.
//

#pragma once

#include <sstream>
#include <string>

namespace duerVision {

enum class LogLevel {
    ERROR = 0, WARNING, INFO, DEBUG
};
extern LogLevel global_snowboy_log_level;

inline const char* LogLevelToString(LogLevel level);

class SnowboyLog {
public:
    SnowboyLog() {};
    virtual ~SnowboyLog();
    std::ostringstream & get(LogLevel level = LogLevel::WARNING);

protected:
    std::ostringstream os;

private:
    SnowboyLog(const SnowboyLog &);
    SnowboyLog &operator=(const SnowboyLog &);
};

void SnowboyAssertFailure(const int line,
                          const std::string& file,
                          const std::string &func,
                          const std::string &cond);



} // namespace duerVision

// Note: "#define" is not bound by namespaces, it is a preprocessor directive.
// This macro will be expanded to something like the following:
// if (!SNOWBOY_DEBUG_MODE);
// else
//   std::cerr << ...
// Therefore logs will be turned off in production mode.
#define SNOWBOY_LOG(v) \
  if (v <= duerVision::global_snowboy_log_level) \
    duerVision::SnowboyLog().get(v) << __FILE__ << " : " \
        << __func__ << "(): Line_" << __LINE__ << " : "

#ifndef PRODUCTION
#define SNOWBOY_ASSERT(cond) if (!(cond)) \
  duerVision::SnowboyAssertFailure(__LINE__, __FILE__, __func__, #cond);
#else
#define SNOWBOY_ASSERT(cond)
#endif