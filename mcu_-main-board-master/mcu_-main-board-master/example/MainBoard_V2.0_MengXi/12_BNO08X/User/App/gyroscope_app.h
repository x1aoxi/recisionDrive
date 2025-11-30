#ifndef __GYROSCOPE_APP_H__
#define __GYROSCOPE_APP_H__

#include "MyDefine.h"

#define BNO08x_ON 1 // 是否启用 BNO08x 九轴姿态传感器，默认使用 ICM20608-G 六轴传感器

#if BNO08x_ON == 0

  typedef struct ICM20608 {
    Vector3f gyro;
    Vector3f accel;
    float Roll;
    float Pitch;
    float Yaw;
    float temperature;
  } ICM20608;

  extern ICM20608 icm20608;

#else

  typedef struct BNO08x {
    float Roll;
    float Pitch;
    float Yaw;
  } BNO08x;

  extern BNO08x bno08x;

#endif

void Gyroscope_Init(void);
void Gyroscope_Task(void);
  
extern uint8_t first_gyroscope_flag;

#endif
