//
// Created by zhangyongchao on 2020/2/11.
//

#include <ctime>
#include <iostream>
#include "snowboy_logging.h"

#ifdef ANDROID_LOG_ENABLE
#include "utils/snowboy-android-log.h"
#endif

namespace duerVision {

LogLevel global_snowboy_log_level = LogLevel::WARNING;

inline const char* LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR:
            return "ERROR";

        case LogLevel::WARNING:
            return "WARNING";

        case LogLevel::INFO:
            return "INFO";

        case LogLevel::DEBUG:
            return "DEBUG";
    }
}

std::ostringstream &SnowboyLog::get(LogLevel level) {
    time_t now = time(NULL);
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%c", std::localtime(&now))) {
        os << mbstr;
    }
    os << " " << LogLevelToString(level) << ": ";
    return os;
}

SnowboyLog::~SnowboyLog() {
    os << std::endl;
#ifdef ANDROID_LOG_ENABLE
    ANDROID_LOGE("%s", ss_.str().c_str());
#endif
    std::cerr << os.str() << std::endl;
    std::cerr.flush();
}

void SnowboyAssertFailure(const int line,
                          const std::string& file,
                          const std::string& func,
                          const std::string& cond) {
    std::ostringstream ss;
    ss << "ASSERT_FAILURE (" << file << " : " << func
        << "(): Line_" << line << "): " << cond;
#ifdef ANDROID_LOG_ENABLE
    ANDROID_LOGE("%s", ss_.str().c_str());
#endif
    std::cerr << ss.str() << std::endl;
    std::cerr.flush();
    throw std::runtime_error(ss.str());
}

}