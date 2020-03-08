//
// Created by zhangyongchao on 2020/3/8.
//

#include "duer_face_predict_service.h"
#include "../utils/snowboy_logging.h"

namespace duerVision {
bool DuerFacePredictService::init_settings(const DuerVisionSettings& duer_vision_settings) {
    // 根据设备配置做一些初始化
    b_initialized = true;
    return true;
}


VisionErrorCode DuerFacePredictService::face_predict(UltraFaceSingleton* p_ultra_face_singleton,
                             const DuerImageInfo& duer_image_info,
                             std::vector<DuerFaceResult>* p_vt_duer_face_result) {

    SNOWBOY_ASSERT(b_initialized == true);
    std::vector<FaceInfo> face_list;
    SnowboyAssertFailure(p_ultra_face_singleton != NULL);
    p_vt_duer_face_result->clear();
    int face_num = p_ultra_face_singleton->detect(duer_image_info.imageData, duer_image_info.imageWidth,
            duer_image_info.imageHeight, face_list);
    if (face_num < 0) {
        return Res_Face_NULL;
    }
    for (auto face_info : face_list) {
        DuerFaceResult duer_face_result;
        duer_face_result.rect.x = int(face_info.x1)
        duer_face_result.rect.y = int(face_info.y1)
        duer_face_result.rect.width = int(face_info.x2 - face_info.x1);
        duer_face_result.rect.height = int(face_info.y2 - face_info.y1);
        duer_face_result.confidence = face_info.score;
        p_vt_duer_face_result->push_back(duer_face_result);
    }
    return Res_OK;
}
}