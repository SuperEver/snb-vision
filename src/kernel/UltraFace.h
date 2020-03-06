//
// Created by Zhang,Yongchao(Duer) on 2020/3/4.
//

#pragma once

#include "ncnn/net.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#define num_featuremap 4
#define hard_nms 1
#define blending_nms 2 /* mix nms was been proposaled in paper blaze face, aims to minimize the temporal jitter*/
namespace duerVision {

typedef struct FaceInfo {
    float x1;
    float y1;
    float x2;
    float y2;
    float score;

    float *landmarks;
} FaceInfo;

class UltraFace {
public:
    UltraFace(const char *model_dir, const char *config_filename);

    ~UltraFace();

    // 输入图，返回人脸框，原图宽高（用于将人脸框映射到此尺寸)
    int detect(ncnn::Mat &img, std::vector<FaceInfo> &face_list, int ori_w, int ori_h);

    // 输入图像素数据，宽，高，通道类型（ncnn::Mat::PIXEL_RGB或ncnn::Mat::PIXEL_BGR)，人脸框
    int detect(const unsigned char *pixel_array, int w, int h, int type, std::vector<FaceInfo> &face_list);

private:
    void generateBBox(std::vector<FaceInfo> &bbox_collection, ncnn::Mat scores, ncnn::Mat boxes, float score_threshold,
                      int num_anchors, int ori_w, int ori_h);

    void nms(std::vector<FaceInfo> &input, std::vector<FaceInfo> &output, int type = blending_nms);

private:
    ncnn::Net ultraface;

    int num_thread;
    int image_w;
    int image_h;

    int in_w;
    int in_h;
    int num_anchors;

    int topk;
    float score_threshold;
    float iou_threshold;


    const float mean_vals[3] = {127, 127, 127};
    const float norm_vals[3] = {1.0 / 128, 1.0 / 128, 1.0 / 128};

    const float center_variance = 0.1;
    const float size_variance = 0.2;
    const std::vector<std::vector<float>> min_boxes = {
            {10.0f,  16.0f,  24.0f},
            {32.0f,  48.0f},
            {64.0f,  96.0f},
            {128.0f, 192.0f, 256.0f}};
    const std::vector<float> strides = {8.0, 16.0, 32.0, 64.0};
    std::vector<std::vector<float>> featuremap_size;
    std::vector<std::vector<float>> shrinkage_size;
    std::vector<int> w_h_list;

    std::vector<std::vector<float>> priors = {};
};

}