//
// Created by zhangyongchao on 2020/2/26.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace duerVision {
enum class ImageType {
    BMP = 0,
    JPG,
    PNG,
    EMPTY,
    UNKNOWN
};

inline ImageType read_type(const std::string &file_name) {
    std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!file) {
        return ImageType::EMPTY;
    }

    char buffer[9];
    file.read((char *) buffer, 8);
    buffer[8] = 0;

    // Determine the true image type using link:
    // http://en.wikipedia.org/wiki/List_of_file_signatures

    if (strcmp(buffer, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A") == 0)
        return ImageType::PNG;
    else if (buffer[0] == '\xff' && buffer[1] == '\xd8' && buffer[2] == '\xff')
        return ImageType::JPG;
    else if (buffer[0] == 'B' && buffer[1] == 'M')
        return ImageType::BMP;

    return ImageType::UNKNOWN;
}

class ImageLoader {

public:
    ImageLoader(const char* filename);
    ~ImageLoader() = default;
    ImageLoader(const ImageLoader&) = delete;
    const ImageLoader& operator = (const ImageLoader&) = delete;

    unsigned long height() {return height_;}
    unsigned long width() {return width_;}
    unsigned long channels() {return output_components_;}
    const unsigned char* rgb_data();

private:
    unsigned long height_ = 0;
    unsigned long width_ = 0;
    unsigned long output_components_;
    std::vector<unsigned char> data;

    ImageType read_image(const char* filename);
};

}