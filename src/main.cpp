#include <iostream>
#include "kernel/utils/snowboy_logging.h"

int main() {
    SNOWBOY_LOG(duerVision::LogLevel::INFO) << "Test for my log";
    std::cout << "Hello, World!" << std::endl;
    //SNOWBOY_ASSERT(2>3);
    return 0;
}
