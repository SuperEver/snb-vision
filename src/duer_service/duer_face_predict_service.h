//
// Created by zhangyongchao on 2020/3/8.
//

#pragma once

#include "duer_vision.h"
#include "../kernel_singleton/ultra_face_singleton.h"
#include <vector>

namespace duerVision {

class DuerFacePredictService {
public:
    DuerFacePredictService()= default;
    bool init_settings(const DuerVisionSettings& duer_vision_settings);
    VisionErrorCode face_predict(UltraFaceSingleton* p_ultra_face_singleton,
            const DuerImageInfo& duer_image_info,
            std::vector<DuerFaceResult>* p_vt_duer_face_result);
    bool is_initialized() const { return b_initialized;}

private:
    bool b_initialized = false;

    DuerFacePredictService(const DuerFacePredictService&) = delete;
    const DuerFacePredictService& operator = (const DuerFacePredictService&) = delete;

};

}