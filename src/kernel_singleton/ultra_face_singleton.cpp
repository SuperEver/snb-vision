//
// Created by zhangyongchao on 2020/3/8.
//

#include "ultra_face_singleton.h"
#include "../utils/snowboy_logging.h"

namespace duerVision {

bool UltraFaceSingleton::load_lib() {
    if (p_ultra_face) {
        return true;
    }
    p_ultra_face = new UltraFace(model_dir_, config_filename_);
    if (!p_ultra_face->is_initialized()) {
        delete p_ultra_face;
        p_ultra_face = NULL;
        SNOWBOY_LOG(LogLevel::ERROR) << "UltraFace initialization error\n";
        return false
    }
    b_initialized = true;
    return true;
}

bool UltraFaceSingleton::unload() {
    if (!p_ultra_face) {
        SNOWBOY_LOG(LogLevel::WARNING) << "UltraFace is NULL before unloading\n";
        b_initialized = false;
        return true;
    }

    bool check_unload_ok = true;

    // check if module is still been using
    pthread_mutex_lock(&mutex);
    if (ref_count != 0) {
        check_unload_ok = false;
        SNOWBOY_LOG(LogLevel::WARNING)<< "Module is been used: ref_count = "
            << ref_count << std::end;
    }
    pthread_mutex_unlock(&mutex);

    if (check_unload_ok) {
        delete p_ultra_face;
        p_ultra_face = NULL;
        b_initialized = false;
        return true;
    }
    return false;
}

bool UltraFaceSingleton::reset(const char *model_dir, const char *config_filename) {
    model_dir_ = model_dir;
    config_filename_ = config_filename;
    if (p_ultra_face) {
        delete p_ultra_face;
        p_ultra_face = NULL;
    }
}

int UltraFaceSingleton::detect(const unsigned char *pixel_array, int w, int h, int type,
        std::vector<FaceInfo> &face_list) {


    // add ref_count before detect
    pthread_mutex_lock(&mutex);
    if (!b_initialized) {
        SNOWBOY_LOG(LogLevel::INFO) << "UltraFace preparing lib\n";
        SNOWBOY_ASSERT(load_lib() == true);
    }
    ref_count += 1;
    pthread_mutex_unlock(&mutex);

    int res = p_ultra_face->detect(pixel_array, w, h, type, face_list);

    // restore ref_count after detect
    pthread_mutex_lock(&mutex);
    ref_count -= 1;
    pthread_mutex_unlock(&mutex);
    return res;
}

}