//
// Created by zhangyongchao on 2020/3/9.
//

#pragma once
#include "duer_vision.h"

namespace duerVision {
class DuerVisionServiceMangager {
public:
    static DuerVisionServiceMangager &getInstance(const char* config_file_path) {
        static DuerVisionServiceMangager manager(config_file_path);
        return manager;
    }

    VisionErrorCode
    facePredict(struct DuerImageInfo *duerImageInfo,
                std::vector<DuerFaceResult>* p_vt_duer_face_result);

private:
    DuerVisionServiceMangager(const char* config_file_path);
    DuerVisionServiceMangager(const DuerVisionServiceMangager&) = delete;
    const DuerVisionServiceMangager& operator = (const DuerVisionServiceMangager&) = delete;
    DuerVisionSettings duer_vision_settings;
};
}