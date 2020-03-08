//
// Created by zhangyongchao on 2020/3/8.
//

#pragma once

#include <string>
namespace duerVision {

//#define faceDetectModuleId       0
//#define mouthDetectModuleId      1
//#define liveDetectModueleId      2
//#define faceVerificationModuleId 3
//#define ageGanderModuleId        4
//#define distanceModuleId         5
//#define faceUnlockModlueId       6
//#define faceDotModuleId          7
//#define brightNessModuleId       8
//#define moveModuleId             9
#define MAX_MODULE_ID            20

#define FACE_FEATURE_ARRAY_LEN   128
#define IF_NEED_TIME_INFO        1

#define DUERVISION_MODULE_ALL             (0xFFFFFFFF)
#define DUERVISION_MODULE_FACE            (1<<1)
#define DUERVISION_MODULE_MOVING          (1<<2)
#define DUERVISION_MODULE_DISTANCE        (1<<3)
#define DUERVISION_MODULE_AGE             (1<<4)
#define DUERIVISON_MODULE_BRIGHTNESS      (1<<10)
#define DUERVISION_MODULE_MOUTH           (1<<5)
#define DUERVISION_MODULE_LIVING          (1<<6)
#define DUERVISION_MODULE_RECOGNIZE       (1<<7)
#define DUERVISION_MODULE_GESTURE         (1<<8)
#define DUERVISION_MODULE_DOT             (1<<9)
#define DUERVISION_MODULE_UNLOCK          (1<<12)
#define DUERVISION_MODULE_GAZE        	  (1<<11)
#define DUERVISION_MODULE_OBJDETECT        (1<<13)

#define MIN_IMAGE_HEIGHT          20
#define MIN_IMAGE_WIDTH           20

typedef unsigned int DUERVISION_MODULE_OPTION;
/**
     * 使用SDK的设备类型
     */
enum DeviceType {
    /**
     * 小度在家（小鱼）
     */
            DuerZaijia_1,
    /**
     * 小度在家1S
     */
            DuerZaijia_1S,
    /**
     * 小度在家1L
     */
            DuerZaijia_1L,
    /**
     * 小度在家1C
     */
            DuerZaijia_1C,

    DuerZaijia_X8,
    DuerZaijia_X5,
    DuerJavaTest
};

enum CameraDataFormat {
    /**
     * NV21中Y/U/V分量的顺序为：YYYY YYYY VUVU
    */
            NV21 = 2,
    /**
     * NV12中Y/U/V分量的顺序为：YYYY YYYY UVUV
     * NV12又称作 YUV420sp
     * 小度在家1L设备摄像头直接返回NV12数据格式，和硬件同学 方青 约定，以15为NV12格式的代号
     */
            NV12 = 3,
    /**
     * YV12中，Y/U/V分量的顺序为：YYYY YYYY VVUU
     */
            YV12 = 4,
    /**
     * I420中，Y/U/V分量的顺序为：YYYY YYYY UUVV
     * I420又称作 YUV420p, YU12
     */
            RGB = 0,

    RGBA = 1,

    I420 = 5,
    BGR = 6,

};

/**
* 解锁结果类型
*/
enum UnlockResultType {
    /**
     * 人脸解锁成功
     */
            SUCCESS,
    /**
    * 人脸解锁FirstStage成功
    */
            FirstStage_SUCCESS,

    /**
    * 人脸解锁SecondStage成功
    */
            SecondStage_SUCCESS,

    /**
     * 输入数据流解析为图像失败
     */
            ErrorCode_1,
    /**
     * face detect module load failed
     */
            ErrorCode_2,

    /**
     * 没有检测到人脸
     */
            ErrorCode_200,
    /**
     * 检测到的人脸质量和角度不够好
     */
            ErrorCode_201,
    /**
     * 人脸的距离不符合要求
     */
            ErrorCode_202,
    /**
     * 获取嘴部参数失败，或者注册人脸的嘴是张开的
     */
            ErrorCode_300,
    /**
     * 第二阶段的嘴没有张开
     */
            ErrorCode_301,
    /**
     * 静默活体检测失败
     */
            ErrorCode_400,
    /**
     * 成年人均可解锁模式下，年龄检测为儿童
     */
            ErrorCode_500,
    /**
     * 年龄检测失败，无返回结果
     */
            ErrorCode_501,
    /**
     * 人脸特征提取失败，无特征返回结果
     */
            ErrorCode_600,
    /**
     * 特定人解锁模式下，人脸比对，不在解锁用户组
     */
            ErrorCode_601,
    /**
     * 成年人均可解锁模式下，第二阶段和第一阶段不是同一个人
     */
            ErrorCode_602,

    /**
    *  unlock persion list is null
    */
            ErrorCode_603,

    /**
    *  unlock persion list is null
    */
            ErrorCode_604,

    /**
     * first Stage detect doesn't success
     */
            ErrorCode_700,

    /**
    * unlock type illegal
    */
            ErrorCode_800,
    ErrorCode_901, /* mouth Model Load Failed */
    ErrorCode_902, /* age Model Load Failed */
    ErrorCode_903, /* alive Model Load Failed */
    ErrorCode_904, /* recoginize Model LoadFailed */
    ErrorCode_905, /**/
};

/**
* 手势预测返回结果
*/
enum GestureResult {
    NOGesture = 0,
    /**
     * OK手势
     */
            GESTUREOK,
    /**
     * 单手掌向前手势
     */
            GESTUREPalm,
    /**
* ��ָ����
*/
            Left,
    /**
     * ��ָ����
     */
            Right,
    /**
     * ͬʱ��⵽�෴����
     */
            Opposite,
    /**
     * 其他手势
     */
            OtherGesture
};
/**
 * 目标检测返回结果
 */
enum ObjectDetectionResult {
    /**
     * 猫或狗
     */
            PET,
    /**
     * 人体
     */
            HUMANBODY,
    /**
     * 其他
     */
            OTHEROBJECT,
    OBJ_NOTHING,
};

enum EnvLightStatus {
    DATA_VALID = 0,
    /**
     * 极黑
     */
            TOO_DARK,
    /**
     * 极亮
     */
            TOO_LIGHT,
    /**
     * 中间态（不亮不黑）
     */
            MEDIUM,
};

/**
* 返回的人脸列表的排序策略
*/
enum FaceRankStrategy {
    /**
     * 按人脸的大小排序，由大到小
     */
            LARGEST,
    /**
     * 按到图片中心的距离排序，由近到远
     */
            CLOSEST_TO_CENTER,
    /**
     * 同时考虑人脸的大小和到中心的距离
     * 人脸越大、距离越近，则排序越靠前
     */
            LARGE_AND_CLOSE
};

enum VisionErrorCode {
    Res_OK = 0,
    Res_InputImage_Invalid,
    Res_BBoxSize_Invalid,
    Res_DetectNot_Run_Yet,
    Res_NoFace_Detected,
    Res_Face_NULL,
    Res_Face_Invalid,
    Res_Live_Ncnn_Error,
    Res_DetectObject_NULL,
    Res_LiveObject_NULL,
    Res_RecognizeObject_NULL,
    Res_MouthDetect_Error,
    Res_Feature_AllZeros,
    Res_Feature_ExtractFailed,
    Res_Dir_NotExist,
    Res_GestureUninited,
    Res_GestureIsBusying,
    Res_RotationError,
    Res_GestturePredictError,
    Res_Img_Detected,
    Res_MouthObject_NULL,
    Res_AgeObject_NULL,
    Res_MotionObject_NULL,
    Res_AgeGetFailed,
    Res_GazeUninited,
    Res_Gazetypeerror,
    Res_Gazenofacedetect,
    Res_ModelLoadFailed,
    Res_GAZE_initfailed,
    Res_GAZE_facedstatus_error,
    Res_GAZE_predict_error,
    Res_GAZE_predict_NOgaze,
    Res_GAZE_predict_Errorindex,
    Res_ReturnPtr_Invalid,
    Res_Nothingdetect,
    Res_DetectLenthError,
    Res_GazeModelload_failed,
    Res_GestureModelload_failed,
    Res_ObjectModelload_failed,
    Res_Getotherface_failed,
};


/**
 * 人脸解锁方式选项
 */
enum DuerUnlockMethodType {
    /**
     * 仅成年人可以解锁
     */
            ONLY_ADULT,
    /**
     * 仅管理员可以解锁
     */
            ONLY_ADMINISTRATORS,
};

struct DuerImageInfo {
    unsigned int timeStamp;      // timeStamp of camera frames
    unsigned char *imageData;    // pointer to frame data
    int imageWidth;              // width of frame
    int imageHeight;             // height of frame
};

#ifndef FaceRect
struct FaceRect {
    int x;
    int y;
    int width;
    int height;
};
#endif

struct DuerFaceResult {
    int index;
    FaceRect rect;               // Face position
    float confidence;            // Face confidence
    float faceQualityConfidence; // Face Quality Confidence, range from 0 to 1.0
    bool isGoodFace = false;     // isGoodFace, returned by facePredict
    bool isFrontalFace = false;  // isFrontalFace, returned by facePredict
    float faceBrightness = -1.0f;
    float distance = -1.0f;         // distance between face and camera, returned by distancePredict
    float age_exact = -1.0f;        // exact of age returned by agePredict function
    float age = -1.0f;              // 5 means children, 32 means adult. other value has no meaning. returned by agePredict
    float mouthscore = -1.0f;            // returned by mouthPredict
    bool isMouthOpened = false;          // 1 means open, 0 means closed,  returned by mouthPredict
    float liveScores = -1.0f;
    bool isAlive = false;        // true means alive.
    bool isGazing = false;               // returned by gazePredict
    float facePosDegreeFromCameraLeft = 1000.0f;
    float faceMouthHeightWidthRatio = -1;
};
//
//    struct DuerVisionResult {
//        int index;
//        int faceNum = 0;             // store face nums, returned by facePredict
//        bool isGoodFace = false;     // isGoodFace, returned by facePredict
//        bool isFrontalFace = false;  // isFrontalFace, returned by facePredict
//        float distance = -1;         // distance between face and camera, returned by distancePredict
//        float age = -1;              // 5 means children, 32 means adult. other value has no meaning. returned by agePredict
//        float mouthscore;            // returned by mouthPredict
//        bool isMouthOpened;          // 1 means open, 0 means closed,  returned by mouthPredict
//        bool isAlive = false;        // true means alive.
//        float brightness;            // returned by brightnessPredict
//        EnvLightStatus lightStatus;  // returned by brightnessPredict
//        UnlockResultType unlockresult;    // returned by unlock, TODO
//        bool moveStatus;     // returned by movePredict
//        bool isGazing;               // returned by gazePredict
//        GestureResult gesture;       // returned by gesturePredict
//    };
struct ObjectResponse {
    ObjectDetectionResult objectDetectionResult;
    float probability;
    float leftTopX;
    float leftTopY;
    float rightBottomX;
    float rightBottomY;
} ;

enum CameraDataFormat {
    /**
     * NV21中Y/U/V分量的顺序为：YYYY YYYY VUVU
    */
            NV21 = 2,
    /**
     * NV12中Y/U/V分量的顺序为：YYYY YYYY UVUV
     * NV12又称作 YUV420sp
     * 小度在家1L设备摄像头直接返回NV12数据格式，和硬件同学 方青 约定，以15为NV12格式的代号
     */
            NV12 = 3,
    /**
     * YV12中，Y/U/V分量的顺序为：YYYY YYYY VVUU
     */
            YV12 = 4,
    /**
     * I420中，Y/U/V分量的顺序为：YYYY YYYY UUVV
     * I420又称作 YUV420p, YU12
     */
            RGB = 0,

    RGBA = 1,

    I420 = 5,
    BGR = 6,

};

struct DuerVisionSettings {

    const std::string TAG_SETTING = "DUER_FACE_SETTING";
    const std::string DUER_VISION_VERSION = "v0.2.3.0";
    bool checkTime = false;

    int image_width;
    int image_height;
    // 设备类型
    //DeviceType deviceType = DuerZaijia_1S;
    // 输入数据流格式，默认是YV12（即小度在家1S）
    CameraDataFormat CameraDataFormat = YV12;
    // 输入图像旋转角度
    int rotateDegreeClockwise = 180; //默认旋转180度
    // 算法执行可用的线程数
    int threadNum = 1; //默认单线程

    // 人脸列表的排序策略，默认按照人脸的大小排序
    FaceRankStrategy faceRankStrategy = LARGEST;

    GestureResult gestureResultStrategy = GESTUREOK;

    // 距离检测比例参数，默认是1S设备的参数
    float DISTANCE_SQRTAREA_RATIO_CHILD = 0.133f;
    float DISTANCE_SQRTAREA_RATIO_ADULT = 0.18f;

    // 环境亮度检测阈值设定，默认是1S设备的参数
    // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
    float ENV_LIGHT_THRESHOLD = 0.4353f;
    float ENV_DARK_THRESHOLD = 0.1900f;

    //TODOupda
    /* Paratemeters needed when run  */
};

}
