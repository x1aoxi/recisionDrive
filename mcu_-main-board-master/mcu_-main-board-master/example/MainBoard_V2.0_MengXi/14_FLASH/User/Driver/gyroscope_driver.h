#ifndef __GYROSCOPE_DRIVER_H__
#define __GYROSCOPE_DRIVER_H__

#include "MyDefine.h"

float convert_to_continuous_yaw(float current_yaw);
void PreprocessForMadgwick(Vector3f *gyro, Vector3f *accel);

#endif

