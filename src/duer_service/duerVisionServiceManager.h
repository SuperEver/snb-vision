//
// Created by zhangyongchao on 2020/3/7.
//

#pragma once

#include <string>
#include <pthread.h>

namespace duerVision {

class UltraFace;
class ServiceManager {
private:
    // 需加载的模型资源
    UltraFace* p_ultra_face = NULL;


};


class DuerVisionServiceMangager {

private:
    DuerVisionServiceMangager(const DuerVisionServiceMangager &) {};
    // private constructor function to be copyed

    DuerVisionServiceMangager &operator=(const DuerVisionServiceMangager &) {};
    // private constructor function to be changed

    DuerVisionServiceMangager(const char *conf_path);

    ~DuerVisionServiceMangager();

public:
    // thread safe singleton in c++11
    static DuerVisionServiceMangager &getInstance(const char *conf_path) {
        static DuerVisionServiceMangager duer_vision_serv_manager(conf_path);
        return duer_vision_serv_manager;
    }


    VisionErrorCode preloadVisionModule(DUERVISION_MODULE_OPTION moduleId);

    /* preload vision modules according to moduleOption */

    void unloadVisionModule(DUERVISION_MODULE_OPTION moduleId);

    /* unload vision modules according to moduleOption */



    VisionErrorCode
    facePredict(struct DuerImageInfo *duerImageInfo, struct DuerFaceResult **pDuerFaceResult,
                int *faceNum);

    VisionErrorCode faceExtractFeature(struct DuerImageInfo *duerImageInfo, float *faceFeature,
                                       int *mFaceFeatureLen, int faceIndex = 0);

    VisionErrorCode getFeatureListByDir(const char *dirPath);


    /**
     * 设定算法使用的线程数（1代设备需要2个线程，1S设备需要1个），默认1个
     * @param mThreadNum 要使用的线程数
     */
    void setThreadNum(int mThreadNum);

    /**
    * 设定输入数据流的格式
    * 可将Xiaoyu摄像头接口返回的数据format直接传入
    * @param imageType 数据流格式（目前支持NV21、YV12和NV12）：
    *                  NV21：CameraDataFormat.NV21.imageType(即android.graphics.ImageFormat.NV21)，
    *                  YV12：CameraDataFormat.YV12.imageType(即android.graphics.ImageFormat.YV12)，
    *                  NV12：CameraDataFormat.NV12.imageType(自定义为15)
    */
    void setCameraDataFormat(CameraDataFormat imageType);


    /**
     * 设定输入数据流的旋转角度，默认不旋转（1S设备需要旋转180度）
     * 可将Xiaoyu摄像头接口返回的数据orientation直接传入
     * @param rotateDegree 要顺时针旋转的角度
     */
    void setCameraDataRotateDegreeClockwise(int rotateDegree);

};

}