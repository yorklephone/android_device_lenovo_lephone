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

#ifndef ANDROID_LIGHT_SENSOR_H
#define ANDROID_LIGHT_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "nusensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

#define LIGHT_DEVICE_NAME  "/dev/input/event2"
#define LIGHT_ACTIVE_FILE  "/sys/module/cm3602/parameters/active_light_sensor"
#define LIGHT_DELAY_FILE   "/sys/module/cm3602/parameters/light_sensor_period_s"
#define LIGHT_WAKE_FILE    "/sys/module/cm3602/parameters/light_sensor_wake"
#define REL_ADC		0x0a
/*****************************************************************************/
struct input_event;

class LightSensor : public SensorBase {
public:
            LightSensor();
    virtual ~LightSensor();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);

private:
    int mEnabled;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvent;
    bool mHasPendingEvent;

    float indexToValue(size_t index) const;
    int setInitialState();
    int isEnabled();
};

/*****************************************************************************/

#endif  // ANDROID_LIGHT_SENSOR_H
