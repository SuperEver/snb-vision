//
// Created by zhangyongchao on 2020/3/8.
//

#pragma once

#include "../kernel/UltraFace.h"
#include <pthread.h>
#include <string>

namespace duerVision {
class UltraFaceSingleton {
public:
    // thread safe singleton in c++11
    static UltraFaceSingleton &getInstance(const char *model_dir, const char *config_filename) {
        static UltraFaceSingleton ultra_face_singleton(model_dir, config_filename);
        return ultra_face_singleton;
    }

    // 预加载
    bool load_lib();

    // 卸载：若当前模块在被使用中，不进行卸载
    bool unload();

    // 重置, 可用于热加载
    bool reset(const char *model_dir, const char *config_filename);

    bool is_initialized() const { return b_initialized;}

    // 输入图像素数据，宽，高，通道类型（ncnn::Mat::PIXEL_RGB或ncnn::Mat::PIXEL_BGR)，人脸框. 返回人脸个数( 小于0则为error)
    int detect(const unsigned char *pixel_array, int w, int h, int type, std::vector<FaceInfo> &face_list);

private:
    UltraFaceSingleton(const char *model_dir,
            const char *config_filename): model_dir_(model_dir),
            config_filename_(config_filename) {}

    UltraFaceSingleton(const UltraFaceSingleton&) = delete;
    const UltraFaceSingleton& operator = (const UltraFaceSingleton&) = delete;

    std::string model_dir_ = "";
    std::string config_filename_ = "";

    // handle for UltraFace
    const UltraFace* p_ultra_face = NULL;

    bool b_initialized = false;
    // enter detect +1, leave detect -1
    unsigned long ref_count = 0;

    // mutex for ref_count
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};
}

