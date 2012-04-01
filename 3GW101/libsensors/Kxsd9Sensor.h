/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_KXSD9_SENSOR_H
#define ANDROID_KXSD9_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "nusensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

#define KXSD9_DEVICE_NAME "/dev/input/event4"
#define KXSD9_ACTIVE_FILE "/sys/module/kxsd9/parameters/active_accelerometer_sensor"
#define KXSD9_DELAY_FILE  "/sys/module/kxsd9/parameters/accelerometer_sensor_period_ms"
#define KXSD9_WAKE_FILE   "/sys/module/kxsd9/parameters/accelerometer_sensor_wake"

#define G_SENSOR_ZERO_G 2048 
#define ONE_G_PER_ADC 820
/*****************************************************************************/

struct input_event;

class Kxsd9Sensor : public SensorBase {
public:
            Kxsd9Sensor();
    virtual ~Kxsd9Sensor();
    virtual int setDelay(int32_t handle, int64_t ns);
    virtual int enable(int32_t handle, int enabled);
    virtual int readEvents(sensors_event_t* data, int count);
    void processEvent(int code, int value);

private:
    int mEnabled;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvent;

    int isEnabled();
	float transferData(int value);
};

/*****************************************************************************/

#endif  // ANDROID_KXSD9_SENSOR_H
