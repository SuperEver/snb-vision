//
// Created by zhangyongchao on 2020/2/26.
//
#include "image_loader.h"
#include "opencv2/opencv.hpp"

namespace duerVision {

ImageLoader::ImageLoader(const char* filename) {
    read_image(filename);
}

ImageType ImageLoader::read_image(const char *filename) {
    ImageType image_type = read_type(filename);
    if (image_type != ImageType::JPG) {
        // currently only implement read jpeg
        return image_type;
    }
    cv::Mat frame = cv::imread(filename);
    output_components_ = frame.channels();
    height_ = frame.rows;
    width_ = frame.cols;
    unsigned long sz = frame.rows * frame.cols * channels();
    data.resize(sz);
    memcpy(data.data(), (void*)(frame.data), sz);
    return image_type;
}

}
