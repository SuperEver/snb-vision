//
// Created by zhangyongchao on 2020/3/7.
//

#include "duerVisionServiceManager.h"
#include <android/log.h>
#include <vector>
#include <algorithm>
#include <map>
#include "net.h"
#include <sys/types.h>
#include <unistd.h>
//#inlcude <limits.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "duerVision.h"
#include  "sdkVersion.h"
#include "string.h"

/* add other class header here */
//#include "facedetect.h"
#include "ultraFace.h"
#include "mouthstatus.h"
#include "recognize.h"
#include "get_distance.h"
#include "age_gender.h"
#include "motiondetect.h"
#include "livedetect.h"
#include "get_envlightstatus.h"
#include "duerFaceUnlock.h"
#include "./visionUtils/visionImageUtils.h"
#include "./visionUtils/visionTimeUtils.h"
#include "./visionUtils/matDataCompare.h"
#include "duerVision.h"
#include "visionDataTypes.h"
#include "imageInternalInfo.h"
#include "duerVisionServiceManager.h"
#include "duergesture.h"
#include "duergaze.h"
#include "objectdetect.h"
#include "mouthHwRatio.h"

#define DUER_TAG_C "DUER_VISION_SERVICE_C"
#define DUERLOGE(...) __android_log_print(ANDROID_LOG_ERROR,DUER_TAG_C,__VA_ARGS__)
//#define DUERLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,DUER_TAG_C,__VA_ARGS__)
#define DUERLOGI(...) __android_log_print(ANDROID_LOG_INFO,DUER_TAG_C,__VA_ARGS__)
#define DUERLOGD
//#define DUERLOGI
namespace duerVision {
static std::vector<Face> FaceArray;
static std::vector<Face> FaceArrayOld;
static std::vector<Face> UnlockPersionList;
static std::string featureListPath = "/data/camera";
static std::string DUER_VISION_SDK_VERSION = SDK_VERSION;
//DuerVisionSettings currentSetings;
/* other object  */

/* 确定需要释放和创建的的新对象，并加载相应的模型 */

static void initDuerVisionSettings(DeviceType deviceType);

/* just for test */

//static int run(void *frameData, struct DuerVisionResult *visionResult);

/* camera数据流中调用的进程，camera数据格式转换，依次执行相应的检测功能 */



static struct DuerVisionSettings duerVisionSettings;
static std::string modulePath;

static class imageInternalInfo *pImgInterInfo = NULL;

/**
 * when add a new vision functional class ,
 * first step: declare here
 *
 */

DuerVisionServiceMangager *DuerVisionServiceMangager::mServiceManagerInstance = NULL;

static class duerVision::UltraFace *ultraFaceDetectObj = NULL;

/* MouthDetectObject declare to NULL */
static class duerVision::mouthStatus *mouthObj = NULL;

/* faceVerifcationDetectObject declare to NULL */
static class duerVision::Recognize *recognizeObj = NULL;

/* liveDetectObject declare to NULL */
static class duerVision::LiveDetect *mlivedetectObj = NULL;

/* MouthDetectObject declare to NULL */
static class duerVision::MotionDetect *motionDetectObj = NULL;

/* AgeGenderDetectObject declare to NULL */
static class duerVision::AgeGender *magegenderObj = NULL;

/* distanceDetectObject declare to NULL */
static class duerVision::get_distance *mgetdistanceObj = NULL;

static class duerVision::get_envlightstatus *menvLightObj = NULL;

static class duerVision::DuerFaceUnlock *duerFaceUnlockObj = NULL;

/* GestureObject declare to NULL */
static class duerVision::Gesture *gestureDetectObj = NULL;

/* GazeObject declare to NULL */
static class duerVision::DuerGaze *gazeObject = NULL;

/* ObjectDetect declare to NULL */
static class duerVision::ObjectDetect *objectdetect = NULL;

unsigned long faceDetectTime = 0;
unsigned long mouthDetectTime = 0;
unsigned long recognizeDetectTime = 0;
unsigned long liveDetectTime = 0;
unsigned long ageDetectTime = 0;
unsigned long gazeDetectTime = 0;
unsigned long gestureDetectTime = 0;

static pthread_mutex_t duerVisionMutex;

class DuerVisionParams duerVisionParams;

void resetObject();

void resetObject() {
    /* reset object to NULL, only called in DuerVisionServiceMangager constructor function */
    ultraFaceDetectObj = NULL;
    mouthObj = NULL;
    recognizeObj = NULL;
    mlivedetectObj = NULL;
    motionDetectObj = NULL;
    magegenderObj = NULL;
    mgetdistanceObj = NULL;
    menvLightObj = NULL;
    gestureDetectObj = NULL;
    duerFaceUnlockObj = NULL;
    gazeObject = NULL;
    objectdetect = NULL;
    return;
}

VisionErrorCode DuerVisionServiceMangager::preloadVisionModule(DUERVISION_MODULE_OPTION moduleOption) {
    /* TODO, to be completed later */
    /*
    * when add a new vision functional class ,
    * third step: create new obj if it is NULL
    *
    */
    VisionErrorCode returnCode = Res_OK;

    pthread_mutex_lock(&duerVisionMutex);

    DUERLOGE("===================preloadVisionModule  IN");
    int threadNum = duerVisionSettings.threadNum;

    if ((ultraFaceDetectObj != NULL) && (!ultraFaceDetectObj->isInitSuccess())) {
        DUERLOGI("ultraFaceDetectObj Created Failed Last Time, Create a New One");
        delete ultraFaceDetectObj;
        ultraFaceDetectObj = NULL;
    }
    if (((moduleOption & DUERVISION_MODULE_FACE) != 0) && (ultraFaceDetectObj == NULL)) {
        ultraFaceDetectObj = new UltraFace(modulePath);
        DUERLOGI("ultraFaceDetectObj Create Here");
        if (!ultraFaceDetectObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
        ultraFaceDetectObj->SetThreadNum(threadNum);
//            ultraFaceDetectObj->SetMinFace(40);
    }

    if ((mouthObj != NULL) && (!mouthObj->isInitSuccess())) {
        DUERLOGI("mouthObj Created Failed Last Time, Create a New One");
        delete mouthObj;
        mouthObj = NULL;
    }
    /* MouthDetectObject created if needed here  */
    if (((moduleOption & DUERVISION_MODULE_MOUTH) != 0) && (mouthObj == NULL)) {
        mouthObj = new mouthStatus(modulePath);
        DUERLOGI("mouthObj Create Here");
        if (!mouthObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
        mouthObj->SetThreadNum(threadNum);
    }

    if ((recognizeObj != NULL) && (!recognizeObj->isInitSuccess())) {
        DUERLOGI("recognizeObj Created Failed Last Time, Create a New One");
        delete recognizeObj;
        recognizeObj = NULL;
    }
    /* faceVerifcationDetectObject created if needed here  */
    if (((moduleOption & DUERVISION_MODULE_RECOGNIZE) != 0) && (recognizeObj == NULL)) {
        recognizeObj = new Recognize(modulePath);
        DUERLOGI("recognizeObj Create Here");
        if (!recognizeObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
        recognizeObj->SetThreadNum(threadNum);
    }

    if ((mlivedetectObj != NULL) && (!mlivedetectObj->isInitSuccess())) {
        DUERLOGI("mlivedetectObj Created Failed Last Time, Create a New One");
        delete mlivedetectObj;
        mlivedetectObj = NULL;
    }
    /* LiveDetectObject created if needed here */
    if (((moduleOption & DUERVISION_MODULE_LIVING) != 0) && (mlivedetectObj == NULL)) {
        mlivedetectObj = new LiveDetect(modulePath);
        DUERLOGI("aliveObj Create Here");
        if (!mlivedetectObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
        mlivedetectObj->SetThreadNum(threadNum);
    }

    if ((magegenderObj != NULL) && (!magegenderObj->isInitSuccess())) {
        DUERLOGI("magegenderObj Created Failed Last Time, Create a New One");
        delete magegenderObj;
        magegenderObj = NULL;
    }
    /* AgeGenderDetectObject created if needed here */
    if (((moduleOption & DUERVISION_MODULE_AGE) != 0) && (magegenderObj == NULL)) {
        magegenderObj = new AgeGender(modulePath);
        DUERLOGI("ageGenderObj Create Here");
        if (!magegenderObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
        magegenderObj->SetThreadNum(threadNum);
    }

    /* distanceDetectObject created if needed here  */
    /* no modeles need to loads, impossible created failed */
    if (((moduleOption & DUERVISION_MODULE_DISTANCE) != 0) && (mgetdistanceObj == NULL)) {
        mgetdistanceObj = new get_distance();
        DUERLOGI("distanceObj Create Here");
        mgetdistanceObj->setparameters(&duerVisionSettings);
    }

    /* motionDetectObject created if needed here */
    /* no modeles need to loads, impossible created failed */
    if (((moduleOption & DUERVISION_MODULE_MOVING) != 0) && (motionDetectObj == NULL)) {
        motionDetectObj = new MotionDetect();
        DUERLOGI("motionObj Create Here");
    }
    if ((objectdetect != NULL) && (!objectdetect->isInitSuccess())) {
        DUERLOGI("mlivedetectObj Created Failed Last Time, Create a New One");
        delete objectdetect;
        objectdetect = NULL;
    }

    if (((moduleOption & DUERVISION_MODULE_OBJDETECT) != 0) && (objectdetect == NULL)) {
        objectdetect = new ObjectDetect(modulePath);
        DUERLOGI("objectdetect Create Here");
        objectdetect->SetThreadNum(threadNum);
        if (!objectdetect->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
    }

    if ((gestureDetectObj != NULL) && (!gestureDetectObj->isInitSuccess())) {
        DUERLOGI("gestureDetectObj Created Failed Last Time, Create a New One");
        delete gestureDetectObj;
        gestureDetectObj = NULL;
    }
    if (((moduleOption & DUERVISION_MODULE_GESTURE) != 0) && (gestureDetectObj == NULL)) {
        gestureDetectObj = new Gesture(modulePath);
        DUERLOGI("gestureDetectObj Create Here");
        if (!gestureDetectObj->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
    }

    /* no modeles need to loads, impossible created failed */
    if (((moduleOption & DUERIVISON_MODULE_BRIGHTNESS) != 0) && (menvLightObj == NULL)) {
        menvLightObj = new get_envlightstatus();
        DUERLOGI("envLightObj Create Here");
    }

    if (((moduleOption & DUERVISION_MODULE_UNLOCK) != 0) && (duerFaceUnlockObj == NULL)) {
        duerFaceUnlockObj = new DuerFaceUnlock();
        DUERLOGI("unlockObj Create Here");
    }

    if ((gazeObject != NULL) && (!gazeObject->isInitSuccess())) {
        DUERLOGI("gazeObject Created Failed Last Time, Create a New One");
        delete gazeObject;
        gazeObject = NULL;
    }
    if (((moduleOption & DUERVISION_MODULE_GAZE) != 0) && (gazeObject == NULL)) {
        gazeObject = new DuerGaze(modulePath);
        DUERLOGI("gazeObject Create Here");
        if (!gazeObject->isInitSuccess()) {
            returnCode = Res_ModelLoadFailed;
        }
    }
    /* Create other object if it will be run and not created yet */
    /* add other class object declare here later */

    pthread_mutex_unlock(&duerVisionMutex);

    return returnCode;
}

void DuerVisionServiceMangager::unloadVisionModule(DUERVISION_MODULE_OPTION moduleOption) {
    /* TODO, to be completed later */
    /*
    * when add a new vision functional class ,
    * third step: create new obj if it is NULL
    *
    */

    pthread_mutex_lock(&duerVisionMutex);

    if (((moduleOption & DUERVISION_MODULE_FACE) != 0) && (ultraFaceDetectObj != NULL)) {
        DUERLOGI("delete ultraFaceDetectObj Module");
        delete ultraFaceDetectObj;
        ultraFaceDetectObj = NULL;
    }
    if (((moduleOption & DUERVISION_MODULE_MOUTH) != 0) && (mouthObj != NULL)) {
        DUERLOGI("delete mouthObj Module");
        delete mouthObj;
        mouthObj = NULL;
    }

    if (((moduleOption & DUERVISION_MODULE_RECOGNIZE) != 0) && (recognizeObj != NULL)) {
        DUERLOGI("delete recognizeObj Module");
        delete recognizeObj;
        recognizeObj = NULL;
    }

    /* LiveDetectObject destroy if needed here */
    if (((moduleOption & DUERVISION_MODULE_LIVING) != 0) && (mlivedetectObj != NULL)) {
        DUERLOGI("delete mliveObj Module");
        delete mlivedetectObj;
        mlivedetectObj = NULL;
    }

    /* AgeGenderDetectObject destroy if needed here */
    if (((moduleOption & DUERVISION_MODULE_AGE) != 0) && (magegenderObj != NULL)) {
        DUERLOGI("delete ageGenderObj Module");
        delete magegenderObj;
        magegenderObj = NULL;
    }

    /* distanceDetectObject destroy if needed here  */
    if (((moduleOption & DUERVISION_MODULE_DISTANCE) != 0) && (mgetdistanceObj != NULL))  {
        DUERLOGI("delete mgetdistanceObj Module");
        delete mgetdistanceObj;
        mgetdistanceObj = NULL;
    }

    /* motionDetectObject destroy if needed here */
    if (((moduleOption & DUERVISION_MODULE_MOVING) != 0) && (motionDetectObj != NULL)) {
        DUERLOGI("delete motionDetectObj Module");
        delete motionDetectObj;
        motionDetectObj = NULL;
    }

    /* objectdetect destroy if needed here */
    if (((moduleOption & DUERVISION_MODULE_OBJDETECT) != 0) && (objectdetect != NULL)) {
        DUERLOGI("delete objectdetect Module");
        delete objectdetect;
        objectdetect = NULL;
    }

    if (((moduleOption & DUERVISION_MODULE_GESTURE) != 0) && (gestureDetectObj != NULL)) {
        DUERLOGI("delete gestureDetectObj Module");
        delete gestureDetectObj;
        gestureDetectObj = NULL;
    }

    if (((moduleOption & DUERIVISON_MODULE_BRIGHTNESS) != 0) && (menvLightObj != NULL)) {
        DUERLOGI("delete menvLightObj Module");
        delete menvLightObj;
        menvLightObj = NULL;
    }

    if (((moduleOption & DUERVISION_MODULE_UNLOCK) != 0) && (duerFaceUnlockObj != NULL)) {
        DUERLOGI("delete duerFaceUnlockObj Module");
        delete duerFaceUnlockObj;
        duerFaceUnlockObj = NULL;
    }

    if (((moduleOption & DUERVISION_MODULE_GAZE) != 0) && (gazeObject != NULL)) {
        DUERLOGI("delete gazeObject Module");
        delete gazeObject;
        gazeObject = NULL;
    }

    /* Create other object if it will be run and not created yet */
    /* add other class object declare here later */
    pthread_mutex_unlock(&duerVisionMutex);

    return;
}

extern unsigned long faceDetectTime;
extern unsigned long mouthDetectTime;
extern unsigned long recognizeDetectTime;
extern unsigned long liveDetectTime;
extern unsigned long ageDetectTime;
extern unsigned long gazeDetectTime;
extern unsigned long gestureDetectTime;

static unsigned timeInterval = 120; // 60 mean 1min
static timer_t timerid;

static void duerVisionTimerThread (union sigval v) {

    if (v.sival_int != 111) {
        DUERLOGE("DuerVison Timer Occured But doesn't belong to DuerVision");
        return;
    }
    unsigned long curTimeSec = getTimeStampSecond();

    if (pthread_mutex_trylock(&duerVisionMutex) != 0) {
        DUERLOGE("DuerVison Timer Get Mutex Failed, Return Now");
        return;
    }
    DUERLOGI("DuerVison Timer Occured Here in Mutex");

    if (((curTimeSec - faceDetectTime) > timeInterval) && (ultraFaceDetectObj != NULL)) {
        DUERLOGI("delete ultraFaceDetectObj Module in Monitor Thread %d, %d", curTimeSec, faceDetectTime);
        delete ultraFaceDetectObj;
        ultraFaceDetectObj = NULL;
    }

    if (((curTimeSec - mouthDetectTime) > timeInterval) && (mouthObj != NULL)) {
        DUERLOGI("delete mouthObj Module in Monitor Thread %d, %d", curTimeSec, mouthDetectTime);
        delete mouthObj;
        mouthObj = NULL;
    }

    if (((curTimeSec - recognizeDetectTime) > timeInterval) && (recognizeObj != NULL)) {
        DUERLOGI("delete recognizeObj Module in Monitor Thread %d, %d", curTimeSec, recognizeDetectTime);
        delete recognizeObj;
        recognizeObj = NULL;
    }

    /* LiveDetectObject destroy if needed here */
    if (((curTimeSec - liveDetectTime) > timeInterval) && (mlivedetectObj != NULL)) {
        DUERLOGI("delete mliveObj Module in Monitor Thread %d, %d", curTimeSec, liveDetectTime);
        delete mlivedetectObj;
        mlivedetectObj = NULL;
    }

    /* AgeGenderDetectObject destroy if needed here */
    if (((curTimeSec - ageDetectTime) > timeInterval) && (magegenderObj != NULL)) {
        DUERLOGI("delete ageGenderObj Module in Monitor Thread %d, %d", curTimeSec, ageDetectTime);
        delete magegenderObj;
        magegenderObj = NULL;
    }

//        if (((curTimeSec - gestureDetectTime) > timeInterval) && (gestureDetectObj != NULL)) {
//            DUERLOGI("delete gestureDetectObj Module in Monitor Thread %d, %d", curTimeSec, gestureDetectTime);
//            delete gestureDetectObj;
//            gestureDetectObj = NULL;
//        }
//
//        if (((curTimeSec - gazeDetectTime) > timeInterval)  && (gazeObject != NULL)) {
//            DUERLOGI("delete gazeObject Module in Monitor Thread %d, %d", curTimeSec, gazeDetectTime);
//            delete gazeObject;
//            gazeObject = NULL;
//        }

    pthread_mutex_unlock(&duerVisionMutex);
    DUERLOGI("DuerVison Timer Occured Unlock returned");
    return;
}


static bool isTimerStarted = false;
int DuerInitMemoryManagementServer() {

    if (isTimerStarted) {
        DUERLOGI("DuerVison Timer Already Createed, return Now");
        return 0;
    }
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    memset((char*)&duerVisionMutex, 0, sizeof(pthread_mutex_t));
    pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED); // process shared mutex lock
    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_NORMAL); // normal mutex lock

    pthread_mutex_init(&duerVisionMutex, &mutexAttr);

    // XXX int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
    // clockid--值：CLOCK_REALTIME,CLOCK_MONOTONIC,CLOCK_PROCESS_CPUTIME_ID,CLOCK_THREAD_CPUTIME_ID
    // evp--存放环境值的地址,结构成员说明了定时器到期的通知方式和处理方式等
    // timerid--定时器标识符

    struct sigevent evp;
    memset(&evp, 0, sizeof(struct sigevent));	//清零初始化

    evp.sigev_value.sival_int = 111;		//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
    evp.sigev_notify = SIGEV_THREAD;		//线程通知的方式，派驻新线程
    evp.sigev_notify_function = duerVisionTimerThread;	//线程函数地址

    if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1) {
        DUERLOGE("fail to timer_create in DuerInitMemoryManagementServer");
        return -1;
    }
    // XXX int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value,struct itimerspec *old_value);
    // timerid--定时器标识
    // flags--0表示相对时间，1表示绝对时间，通常使用相对时间
    // new_value--定时器的新初始值和间隔，如下面的it
    // old_value--取值通常为0，即第四个参数常为NULL,若不为NULL，则返回定时器的前一个值

    //第一次间隔it.it_value这么长,以后每次都是it.it_interval这么长,就是说it.it_value变0的时候会装载it.it_interval的值
    //it.it_interval可以理解为周期
    struct itimerspec it;
    it.it_interval.tv_sec = timeInterval;	//间隔1s
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = timeInterval;
    it.it_value.tv_nsec = 0;

    if (timer_settime(timerid, 0, &it, NULL) == -1)
    {
        DUERLOGE("fail to timer_settime in DuerInitMemoryManagementServer");
        return -1;
    }
    DUERLOGI("DuerVison Timer Created Success");
    isTimerStarted = true;
    return 0;
}

void DuerExitMemoryManagementServer() {
    timer_delete(timerid);
    pthread_mutex_destroy(&duerVisionMutex);
    isTimerStarted = false;
    DUERLOGI("DuerVison Timer Deleted In DuerExitMemoryManagementServer");
    return;
}

void DuerVisionServiceMangager::setMonitorInterval(uint32_t newMonitorInterval) {
    if (newMonitorInterval < 60) {
        newMonitorInterval = 60;
    }
    timeInterval = newMonitorInterval;

    struct itimerspec it;
    it.it_interval.tv_sec = timeInterval;	//间隔1s
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = timeInterval;
    it.it_value.tv_nsec = 0;

    if (timer_settime(timerid, 0, &it, NULL) == -1)
    {
        DUERLOGE("fail to timer_settime in DuerInitMemoryManagementServer");
        return ;
    }
    DUERLOGE("Update DuerVision Time Interval to %d second", timeInterval);
}

DuerVisionServiceMangager::DuerVisionServiceMangager(char *modelPath) {

    /*
    * when add a new vision functional class ,
    * second step: set obj to NULL here
    *
    */
    if (modelPath == NULL) {
        DUERLOGE("DuerVisionServiceManager Exit Because of modelPath is NULL");
        modulePath = "";
//            return;
    } else {
        modulePath = modelPath;
    }

    DIR* dirModel = NULL;
    if ((dirModel = opendir(modelPath)) == NULL) {
        DUERLOGE("DuerVisionServiceManager Exit Because of modelPath doesn't exist");
        //return;
    } else {
        closedir(dirModel);
    }

    FaceArray.clear();
    FaceArrayOld.clear();
//        /* add other class object init here later */
    resetObject();

//        initDuerVisionSettings(deviceType);
    memset((void *) &duerVisionSettings, 0, sizeof(struct DuerVisionSettings));

    pImgInterInfo = new imageInternalInfo();
    /* Create Time Monitor Thread */
    DuerInitMemoryManagementServer();
    if (modelPath != NULL) {
        DUERLOGI("DuerVisionServiceManager now Created success in PATH %s, SDK version: %s\n",
                 modulePath.c_str(),
                 DUER_VISION_SDK_VERSION.c_str());
    } else {
        DUERLOGI("DuerVisionServiceManager now Created failed in Invalid Path (NULL), SDK version: %s\n",
                 DUER_VISION_SDK_VERSION.c_str());
    }
}

DuerVisionServiceMangager::~DuerVisionServiceMangager() {
    /*
    * when add a new vision functional class ,
    * sixth step: create new obj if it is NULL
    *
    */
    unloadVisionModule(DUERVISION_MODULE_ALL);

    /* Create other object if it will be run and not created yet */
    //
    if (pImgInterInfo != NULL) {
        delete pImgInterInfo;
        pImgInterInfo = NULL;
    }
    DuerExitMemoryManagementServer(); // release unlock mutex and delete timer
    //unloadVisionModule(DUERVISION_MODULE_ALL);
    DUERLOGI("DuerVisionServiceManager Exit");
}

DuerVisionServiceMangager *DuerVisionServiceMangager::getDuerVisionInstance(char *modelPath) {
    if (mServiceManagerInstance == NULL) {
        mServiceManagerInstance = new DuerVisionServiceMangager(modelPath);
    }
    return mServiceManagerInstance;
}

void DuerVisionServiceMangager::releaseDuerVisionInstance() {
    if (mServiceManagerInstance != NULL) {
        delete mServiceManagerInstance;
        mServiceManagerInstance = NULL;
    }
}

/* just for test  */
static void initDuerVisionSettings(DeviceType deviceType) {
    int cfgSeq = 0;
    duerVisionSettings.deviceType = deviceType;
    if (deviceType == DuerZaijia_1S) {
        duerVisionSettings.checkTime = false;

        /* check what vision function to do in this fram */
        // 设备类型

        // 输入数据流格式，默认是YV12（即小度在家1S）
        duerVisionSettings.CameraDataFormat = YV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 180; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.133f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.18f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;
    } else if (deviceType == DuerZaijia_1) {
        duerVisionSettings.checkTime = false;

        /* check what vision function to do in this fram */
        // 设备类型

        // 输入数据流格式，默认是YV12（即小度在家1S）
        duerVisionSettings.CameraDataFormat = NV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 0; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.10f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.1353f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.3378f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1500f;
//            duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
//            duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;

    } else if (deviceType == DuerZaijia_1L) {
        duerVisionSettings.checkTime = false;

        /* check what vision function to do in this fram */
        // 设备类型

        // 输入数据流格式，默认是YV12（即小度在家1S）
        duerVisionSettings.CameraDataFormat = NV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 0; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.148f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.20f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;

        // 小度在家1L上，输入为NV12格式，且无需旋转
        // 需要1个线程
        // 距离检测参数：儿童为0.148， 成人为0.20
        // 亮暗检测阈值参数：极暗阈值为0.1900，极亮阈值为0.4353


    } else if (deviceType == DuerZaijia_1C) {
        duerVisionSettings.checkTime = false;

        /* check what vision function to do in this fram */
        // 设备类型

        // 输入数据流格式，默认是YV12（即小度在家1S）
        duerVisionSettings.CameraDataFormat = YV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 180; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.133f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.18f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;

        // 小度在家1C上，输入为YV12格式，需旋转180度
        // 需要1个线程
        // 距离检测参数：儿童为0.133， 成人为0.18
        // 亮暗检测阈值参数：极暗阈值为0.1900，极亮阈值为0.4353

    } else if (deviceType == DuerJavaTest) {
        duerVisionSettings.checkTime = false;

        /* check what vision function to do in this fram */
        // 设备类型

        // 输入数据流格式，默认是YV12（即小度在家1S）
        duerVisionSettings.CameraDataFormat = RGB;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 0; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.133f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.18f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;

    } else if (deviceType == DuerZaijia_X8) {
        duerVisionSettings.checkTime = false;
        duerVisionSettings.CameraDataFormat = YV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 0; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.111f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.15f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.4353f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1900f;
    }else if (deviceType == DuerZaijia_X5) {
        // 小度在家X5上，输入为YV12格式，无需旋转
        // 需要1个线程
        // 距离检测参数：儿童为， 成人为
        // 亮暗检测阈值参数：极暗阈值为0.1800，极亮阈值为0.42f
        duerVisionSettings.checkTime = false;
        duerVisionSettings.CameraDataFormat = YV12;
        // 输入图像旋转角度
        duerVisionSettings.rotateDegreeClockwise = 0; //默认旋转180度
        // 算法执行可用的线程数
        duerVisionSettings.threadNum = 1; //默认单线程

        // 人脸列表的排序策略，默认按照人脸的大小排序
        duerVisionSettings.faceRankStrategy = LARGEST;

        duerVisionSettings.gestureResultStrategy = GESTUREOK;

        // 距离检测比例参数，默认是1S设备的参数
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_CHILD = 0.143f;
        duerVisionSettings.DISTANCE_SQRTAREA_RATIO_ADULT = 0.193f;

        // 环境亮度检测阈值设定，默认是1S设备的参数
        // 修改阈值时，记得检查setEnvThreshold中的输入检查逻辑
        duerVisionSettings.ENV_LIGHT_THRESHOLD = 0.42f;
        duerVisionSettings.ENV_DARK_THRESHOLD = 0.1800f;

    }
}



void DuerVisionServiceMangager::setThreadNum(int mThreadNum) {
    if (mThreadNum <= 0 || mThreadNum > 4) {
        DUERLOGE("Error in DuerVISION setThreadNum: thread number must >0 and <=4! ");
        return;
    }
    duerVisionSettings.threadNum = mThreadNum;
    return;
}


VisionErrorCode
DuerVisionServiceMangager::facePredict(struct DuerImageInfo *duerImageInfo,
                                       struct DuerFaceResult **pDuerFaceResult, int *faceNum) {
    int threadNum = duerVisionSettings.threadNum;
    VisionErrorCode returnCode = Res_OK;

    pthread_mutex_lock(&duerVisionMutex);

    if (ultraFaceDetectObj == NULL) {
        ultraFaceDetectObj = new UltraFace(modulePath);
        DUERLOGI("ultraFaceDetectObj Create Here");
//            ultraFaceDetectObj->SetMinFace(40);
    }
    ultraFaceDetectObj->SetThreadNum(threadNum);
    if ((ultraFaceDetectObj != NULL) && (!ultraFaceDetectObj->isInitSuccess())) {
        DUERLOGI("ultraFaceDetectObj Created Failed Return Now In facePredict");
        delete ultraFaceDetectObj;
        ultraFaceDetectObj = NULL;
        pthread_mutex_unlock(&duerVisionMutex);
        return Res_DetectObject_NULL;
    }

    if ((faceNum == NULL) || (pDuerFaceResult == NULL)) {
        DUERLOGE("Return Pointer is NULL in facePredict!!!");
        pthread_mutex_unlock(&duerVisionMutex);
        return Res_ReturnPtr_Invalid;
    }
    *faceNum = 0;
    *pDuerFaceResult = NULL;
    if ((duerImageInfo == NULL) || (duerImageInfo->imageData == NULL) || (duerImageInfo->imageHeight < MIN_IMAGE_HEIGHT) || (duerImageInfo->imageWidth < MIN_IMAGE_WIDTH)) {
        DUERLOGE("Input Image Info is NULL in facePredict!!!");
        pthread_mutex_unlock(&duerVisionMutex);
        return Res_InputImage_Invalid;
    }

    if (menvLightObj == NULL) {
        menvLightObj = new get_envlightstatus();
    }

    /* put image buffer size to duerVisionSetting */
    duerVisionSettings.image_width = duerImageInfo->imageWidth;
    duerVisionSettings.image_height = duerImageInfo->imageHeight;
    unsigned char *imageData = duerImageInfo->imageData;
    pImgInterInfo->restartNewFrame(duerImageInfo);
    if (pImgInterInfo->isImageAlreadyFacePredicted(duerImageInfo)) {
        DUERLOGI("This Image Already Face Detected");
        if (isFaceAlreayDetected(duerImageInfo) == 0) {
            /* result stored in  FaceArray */
            struct DuerFaceResult *pDuerFaceResList = NULL;
            int num_face = FaceArray.size();
            if (num_face > 0) {
                pDuerFaceResList = (struct DuerFaceResult *) malloc(
                        num_face * sizeof(struct DuerFaceResult));
                if (pDuerFaceResList != NULL) {
                    memset((void*)pDuerFaceResList, 0, num_face * sizeof(struct DuerFaceResult));
                }
            }
            float faceBrightNess = -1.0f;
            for (int seq = 0; seq < num_face; seq++) {
                FaceArray[seq].timeStamp = duerImageInfo->timeStamp;
                pDuerFaceResList[seq].index = seq;
                pDuerFaceResList[seq].confidence = FaceArray[seq].confidence;
                pDuerFaceResList[seq].faceQualityConfidence = FaceArray[seq].faceQualityConfidence;
                pDuerFaceResList[seq].rect.x = FaceArray[seq].rect.x;
                pDuerFaceResList[seq].rect.y = FaceArray[seq].rect.y;
                pDuerFaceResList[seq].rect.width = FaceArray[seq].rect.width;
                pDuerFaceResList[seq].rect.height = FaceArray[seq].rect.height;
                pDuerFaceResList[seq].isGoodFace = FaceArray[seq].isGoodFace;
                pDuerFaceResList[seq].isFrontalFace = FaceArray[seq].isFrontalFace;
                faceBrightNess = menvLightObj->getFaceBrightness(&FaceArray[seq]);
                pDuerFaceResList[seq].faceBrightness = faceBrightNess;
                pDuerFaceResList[seq].distance = -1.0f;
                pDuerFaceResList[seq].age_exact = -1.0f;
                pDuerFaceResList[seq].age = -1.0f;
                pDuerFaceResList[seq].mouthscore = -1.0f;
                pDuerFaceResList[seq].isMouthOpened = false;
                pDuerFaceResList[seq].liveScores = -1.0f;
                pDuerFaceResList[seq].isAlive = false;
                pDuerFaceResList[seq].isGazing = false;
                DUERLOGI("Face Seq %d, face brightness value %f, confidence %f, %f", seq, faceBrightNess, pDuerFaceResList[seq].confidence, pDuerFaceResList[seq].faceQualityConfidence);
                /* store image timestamp to FaceArray. timestamp is unique image info identification */
            }
            *faceNum = num_face;
            *pDuerFaceResult = pDuerFaceResList;
            DUERLOGI("Find %d faces in this frame", num_face);
            pthread_mutex_unlock(&duerVisionMutex);
            return Res_OK;
        } else if (isFaceAlreayDetected(duerImageInfo) == 1) {
            /* result stored in  FaceArrayOld */
            struct DuerFaceResult *pDuerFaceResList = NULL;
            int num_face = FaceArrayOld.size();
            if (num_face > 0) {
                pDuerFaceResList = (struct DuerFaceResult *) malloc(
                        num_face * sizeof(struct DuerFaceResult));
                if (pDuerFaceResList != NULL) {
                    memset((void*)pDuerFaceResList, 0, num_face * sizeof(struct DuerFaceResult));
                }
            }
            float faceBrightNess = -1.0f;
            for (int seq = 0; seq < num_face; seq++) {
                FaceArrayOld[seq].timeStamp = duerImageInfo->timeStamp;
                pDuerFaceResList[seq].index = seq;
                pDuerFaceResList[seq].confidence = FaceArrayOld[seq].confidence;
                pDuerFaceResList[seq].faceQualityConfidence = FaceArrayOld[seq].faceQualityConfidence;
                pDuerFaceResList[seq].rect.x = FaceArrayOld[seq].rect.x;
                pDuerFaceResList[seq].rect.y = FaceArrayOld[seq].rect.y;
                pDuerFaceResList[seq].rect.width = FaceArrayOld[seq].rect.width;
                pDuerFaceResList[seq].rect.height = FaceArrayOld[seq].rect.height;
                pDuerFaceResList[seq].isGoodFace = FaceArrayOld[seq].isGoodFace;
                pDuerFaceResList[seq].isFrontalFace = FaceArrayOld[seq].isFrontalFace;
                faceBrightNess = menvLightObj->getFaceBrightness(&FaceArrayOld[seq]);
                pDuerFaceResList[seq].faceBrightness = faceBrightNess;
                pDuerFaceResList[seq].distance = -1.0f;
                pDuerFaceResList[seq].age_exact = -1.0f;
                pDuerFaceResList[seq].age = -1.0f;
                pDuerFaceResList[seq].mouthscore = -1.0f;
                pDuerFaceResList[seq].isMouthOpened = false;
                pDuerFaceResList[seq].liveScores = -1.0f;
                pDuerFaceResList[seq].isAlive = false;
                pDuerFaceResList[seq].isGazing = false;
                DUERLOGI("Face Seq %d, face brightness value %f, confidence %f, %f", seq, faceBrightNess, pDuerFaceResList[seq].confidence, pDuerFaceResList[seq].faceQualityConfidence);
                /* store image timestamp to FaceArray. timestamp is unique image info identification */
            }
            *faceNum = num_face;
            *pDuerFaceResult = pDuerFaceResList;
            DUERLOGI("Find %d faces in this frame", num_face);

            pthread_mutex_unlock(&duerVisionMutex);
            return Res_OK;
        } else {
            *pDuerFaceResult = NULL;
            pthread_mutex_unlock(&duerVisionMutex);
            return Res_NoFace_Detected;
        }
    }

//        DUERLOGI("In FacePredict Orig Addr is 0x%x, size is %d %d", imageData, duerImageInfo->imageWidth, duerImageInfo->imageHeight);
    unsigned char *rgbData = pImgInterInfo->getRgbData(duerImageInfo,
                                                       duerVisionSettings.CameraDataFormat,
                                                       duerVisionSettings.rotateDegreeClockwise);

    pImgInterInfo->imageFacePredictStatusChange(duerImageInfo);

    if (FaceArray.size() > 0) {
        releaseFaceInfo(FaceArrayOld); // release ncnn::mat in FaceArrayOld vector
        FaceArrayOld = FaceArray;
        /* if there is Vision Detect Result already, store in FaceArrayOld Vector */
    }
    FaceArray.clear();
    if (ultraFaceDetectObj != NULL) {
        returnCode = ultraFaceDetectObj->predict(static_cast<unsigned char *>(imageData), rgbData,
                                                 &duerVisionSettings,
                                                 FaceArray);

    }

    if (returnCode != Res_OK) {
        pthread_mutex_unlock(&duerVisionMutex);
        return returnCode;
    }

    struct DuerFaceResult *pDuerFaceResList = NULL;
    int num_face = FaceArray.size();
    if (num_face > 0) {
        pDuerFaceResList = (struct DuerFaceResult *) malloc(
                num_face * sizeof(struct DuerFaceResult));
        if (pDuerFaceResList != NULL) {
            memset((void*)pDuerFaceResList, 0, num_face * sizeof(struct DuerFaceResult));
        }
    }
    float faceBrightNess = -1.0f;
    for (int seq = 0; seq < num_face; seq++) {
        FaceArray[seq].timeStamp = duerImageInfo->timeStamp;
        pDuerFaceResList[seq].index = seq;
        pDuerFaceResList[seq].confidence = FaceArray[seq].confidence;
        pDuerFaceResList[seq].faceQualityConfidence = FaceArray[seq].faceQualityConfidence;
        pDuerFaceResList[seq].rect.x = FaceArray[seq].rect.x;
        pDuerFaceResList[seq].rect.y = FaceArray[seq].rect.y;
        pDuerFaceResList[seq].rect.width = FaceArray[seq].rect.width;
        pDuerFaceResList[seq].rect.height = FaceArray[seq].rect.height;
        pDuerFaceResList[seq].isGoodFace = FaceArray[seq].isGoodFace;
        pDuerFaceResList[seq].isFrontalFace = FaceArray[seq].isFrontalFace;
        faceBrightNess = menvLightObj->getFaceBrightness(&FaceArray[seq]);
        pDuerFaceResList[seq].faceBrightness = faceBrightNess;
        pDuerFaceResList[seq].distance = -1.0f;
        pDuerFaceResList[seq].age_exact = -1.0f;
        pDuerFaceResList[seq].age = -1.0f;
        pDuerFaceResList[seq].mouthscore = -1.0f;
        pDuerFaceResList[seq].isMouthOpened = false;
        pDuerFaceResList[seq].liveScores = -1.0f;
        pDuerFaceResList[seq].isAlive = false;
        pDuerFaceResList[seq].isGazing = false;
        DUERLOGI("Face Seq %d, face brightness value %f, confidence %f, %f", seq, faceBrightNess, pDuerFaceResList[seq].confidence, pDuerFaceResList[seq].faceQualityConfidence);
        /* store image timestamp to FaceArray. timestamp is unique image info identification */
    }
    *faceNum = num_face;
    *pDuerFaceResult = pDuerFaceResList;
    DUERLOGI("Find %d faces in this frame", num_face);

    pthread_mutex_unlock(&duerVisionMutex);
    return Res_OK;
}



}

