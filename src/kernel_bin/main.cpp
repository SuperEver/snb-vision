#include <iostream>
#include <cstdio>
#include "../utils/snowboy_logging.h"
#include "image_loader.h"

void test_image(const char* image_filename, const char* bin_filename) {
    duerVision::ImageLoader image_loader(image_filename);
    printf("image %s: height %lu, width %lu\n", image_filename, image_loader.height(), image_loader.width());
    FILE* stream_out = fopen(bin_filename, "wb");
    if (!stream_out) {
        return;
    }
    unsigned long num_pixels = image_loader.height() * image_loader.width();
    fwrite(image_loader.rgb_data(), sizeof(unsigned char), num_pixels * 3, stream_out);
    fclose(stream_out);
}

int main(int argc, char** argv) {
    duerVision::global_snowboy_log_level = duerVision::LogLevel::WARNING;
    SNOWBOY_LOG(duerVision::LogLevel::INFO) << "Test for my log";
    std::cout << "Hello, World!" << std::endl;
    test_image(argv[1], argv[2]);
    //SNOWBOY_ASSERT(2>3);
    return 0;
}
