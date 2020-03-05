//
// Created by Zhang,Yongchao(Duer) on 2020/3/5.
//

#include "../kernel/UltraFace.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include <cstdio>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ncnn bin> <ncnn param> list[src_image dst_image]\n", argv[0]);
        return 1;
    }

    std::string bin_path = argv[1];
    std::string param_path = argv[2];
    FILE* stream_lst = fopen(argv[3], "r");
    if (!stream_lst) {
        fprintf(stderr, "List file %s does not exist\n", argv[3]);
        return 1;
    }
    UltraFace ultraface(bin_path, param_path, 320, 240, 1, 0.7); // config model input

    char src_filename[1024], dst_filename[1024];
    while(fscanf(stream_lst, "%s%s", src_filename, dst_filename) == 2) {
        std::cout << "Processing " << src_filename << std::endl;
        cv::Mat frame = cv::imread(src_filename);
        ncnn::Mat inmat = ncnn::Mat::from_pixels(frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);
        std::vector<FaceInfo> face_info;
        ultraface.detect(inmat, face_info);
        for (int i = 0; i < face_info.size(); i++) {
            auto face = face_info[i];
            cv::Point pt1(face.x1, face.y1);
            cv::Point pt2(face.x2, face.y2);
            cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
        }
        cv::imwrite(dst_filename, frame);
    }

    return 0;
}