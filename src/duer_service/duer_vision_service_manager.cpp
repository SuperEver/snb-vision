//
// Created by zhangyongchao on 2020/3/9.
//

#include "../utils/INIReader.h"
#include "duer_vision_service_manager.h"
#include "../kernel_singleton/ultra_face_singleton.h"
#include "duer_face_predict_service.h"
#include <vector>
#include <string>

namespace duerVision {

static UltraFaceSingleton g_ultra_face_singleton("", "");
static DuerFacePredictService g_duer_face_pred_service;


DuerVisionServiceMangager::DuerVisionServiceMangager(const char* config_file_path) {
    // 1. 解析配置文件
    std::string str_to_parse_model_dir
    std::string str_to_parse_cfg;

    // 2. 设置kernel_singleton
    g_ultra_face_singleton.reset(str_to_parse_model_dir.c_str(), str_to_parse_cfg.c_str());

    // 3. 设置service
    g_duer_face_pred_service.init_settings(duer_vision_settings);
}

VisionErrorCode
DuerVisionServiceMangager::facePredict(struct DuerImageInfo *duerImageInfo,
                                       std::vector<DuerFaceResult>* p_vt_duer_face_result) {
    g_duer_face_pred_service.face_predict(&g_ultra_face_singleton, duerImageInfo, p_vt_duer_face_result);

}
}